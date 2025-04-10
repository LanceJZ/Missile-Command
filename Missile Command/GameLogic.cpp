#include "GameLogic.h"

GameLogic::GameLogic()
{
}

GameLogic::~GameLogic()
{
}

void GameLogic::SetBackground(TheBackground* background)
{
	Background = background;
}

void GameLogic::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void GameLogic::SetEnemies(EnemyControl* enemies)
{
	Enemies = enemies;
}

void GameLogic::SetTheCityManager(TheCityManager* cityManager)
{
	CityManager = cityManager;
}

void GameLogic::SetMissileBases(TheABMBaseManager* missileBases)
{
	ABMBaseManager = missileBases;
	Player->SetTheABMBaseManager(missileBases);
}

void GameLogic::SetExplosionModel(Model& model)
{
	ExplosionModel = model;
}

bool GameLogic::Initialize()
{
	Common::Initialize();

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	WaveColors[0].Background = BLACK;
	WaveColors[0].Ground = YELLOW;
	WaveColors[0].CityMainABM = Blue;
	WaveColors[0].CityInner = Aqua;
	WaveColors[0].ICBM = RED;

	WaveColors[1].Background = BLACK;
	WaveColors[1].Ground = YELLOW;
	WaveColors[1].CityMainABM = Blue;
	WaveColors[1].CityInner = Aqua;
	WaveColors[1].ICBM = LightGreen;

	WaveColors[2].Background = BLACK;
	WaveColors[2].Ground = Blue;
	WaveColors[2].CityMainABM = LightGreen;
	WaveColors[2].CityInner = Yellow;
	WaveColors[2].ICBM = Red;

	WaveColors[3].Background = BLACK;
	WaveColors[3].Ground = Red;
	WaveColors[3].CityMainABM = Blue;
	WaveColors[3].CityInner = Yellow;
	WaveColors[3].ICBM = Yellow;

	WaveColors[4].Background = Blue;
	WaveColors[4].Ground = Yellow;
	WaveColors[4].CityMainABM = BLACK;
	WaveColors[4].CityInner = Magenta;
	WaveColors[4].ICBM = Red;

	WaveColors[5].Background = Aqua;
	WaveColors[5].Ground = Yellow;
	WaveColors[5].CityMainABM = Blue;
	WaveColors[5].CityInner = BLACK;
	WaveColors[5].ICBM = Red;

	WaveColors[6].Background = Magenta;
	WaveColors[6].Ground = LightGreen;
	WaveColors[6].CityMainABM = Yellow;
	WaveColors[6].CityInner = BLACK;
	WaveColors[6].ICBM = BLACK;

	WaveColors[7].Background = Yellow;
	WaveColors[7].Ground = LightGreen;
	WaveColors[7].CityMainABM = Red;
	WaveColors[7].CityInner = WHITE;
	WaveColors[7].ICBM = BLACK;

	WaveColors[8].Background = WHITE;
	WaveColors[8].Ground = Red;
	WaveColors[8].CityMainABM = LightGreen;
	WaveColors[8].CityInner = Yellow;
	WaveColors[8].ICBM = Magenta;

	WaveColors[9].Background = Red;
	WaveColors[9].Ground = Yellow;
	WaveColors[9].CityMainABM = Blue;
	WaveColors[9].CityInner = LightGreen;
	WaveColors[9].ICBM = BLACK;

	return true;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	//State = MainMenu;
	//GameEnded = true;

	for (int i = 0; i < 6; i++)
	{
		Enemies->ICBMControl->Cities[i].Position =
			CityManager->Cities[i]->Position;
		Enemies->ICBMControl->Cities[i].Active = true;
		Enemies->ICBMControl->Cities[i].Targeted = false;
	}

	for (int i = 0; i < 3; i++)
	{
		Enemies->ICBMControl->ABMBases[i].Position =
			ABMBaseManager->ABMBases[i]->Position;
		Enemies->ICBMControl->ABMBases[i].Active = true;
		Enemies->ICBMControl->ABMBases[i].Targeted = false;
	}

	NewGame();

	return false;
}

bool GameLogic::Load()
{
	return false;
}

void GameLogic::FixedUpdate()
{
	Common::FixedUpdate();

	GameStateSwitch();

	for (const auto& sBomb : Enemies->ICBMControl->SmartBombs)
	{
		sBomb->SetTargetRefs(Player->Targets);
	}
}

void GameLogic::Input()
{
	if (State == MainMenu)
	{
		if (GameEnded)
		{
		}
		else
		{
		}
	}
	else if (State == Ended)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 15))//Start button
			{
				NewGame();
			}
		}
		else if (IsKeyPressed(KEY_N))
		{
			NewGame();
		}
	}

#ifdef _DEBUG
	if (IsKeyPressed(KEY_W))
	{
		Reset();
		NextWave();

		const std::string wave = "Advanced to Wave " + std::to_string(Wave + 1);

		TraceLog(LOG_INFO, wave.c_str());
	}
#endif
}

void GameLogic::NewGame()
{
	Score.ClearScore();
	NextNewCityScore = 10000;
	Background->WaveColor(Yellow, BLACK);
	Player->NewGame();
	Enemies->NewGame();
	CityManager->NewGame();
	ABMBaseManager->Reset(Blue);
}

void GameLogic::MakeExplosion(Vector3 position)
{
	bool spawnExplosion = true;
	size_t explosionNumber = Explosions.size();

	for (size_t check = 0; check < explosionNumber; check++)
	{
		if (!Explosions[check]->Enabled)
		{
			spawnExplosion = false;
			explosionNumber = check;
			break;
		}
	}

	if (spawnExplosion)
	{
		Explosions.push_back(DBG_NEW TheExplosion());
		FM.Model3DFactory(Explosions.at(explosionNumber), ExplosionModel,
			WHITE, position, {0});
	}
	else Explosions.at(explosionNumber)->Spawn(position);

	for (const auto& sBomb : Enemies->ICBMControl->SmartBombs)
	{
		sBomb->SetExplosionRefs(Explosions);
	}
}

void GameLogic::InGame()
{
	CheckABMs();
	CheckICBMs();

	if (ReadyForNextWave) CheckExplosionsActive();
}

void GameLogic::InMainMenu()
{
	if (GameEnded)
	{
	}
	else
	{
	}
}

void GameLogic::CheckABMs()
{
	for (const auto &missile : Player->ABMs)
	{
		if (missile->Enabled)
		{
			if (missile->CheckHitTarget())
			{
				Player->Targets.at(missile->TargetIndex)->Destroy();
				MakeExplosion(missile->Position);
			}
		}
	}
}

void GameLogic::CheckICBMs()
{
	int missileCount = 0;

	for (int i = 0; i < Explosions.size(); i++)
	{
		if (Enemies->Flier->CirclesIntersect(*Explosions[i]))
		{
			MakeExplosion(Enemies->Flier->Position);
			Score.AddToScore(100 * ScoreMultiplier);
			Enemies->Flier->Destroy();
			Enemies->ResetLaunchTimer();
			break;
		}

		for (const auto &sBomb : Enemies->ICBMControl->SmartBombs)
		{
			if (sBomb->Enabled)
			{
				if (sBomb->CirclesIntersect(*Explosions[i]))
				{
					sBomb->Destroy();
					MakeExplosion(sBomb->Position);
				}
			}
		}
	}

		for (const auto &sBomb : Enemies->ICBMControl->SmartBombs)
		{
			if (sBomb->Enabled)
			{
				for (const auto &city : CityManager->Cities)
				{
					if (sBomb->CirclesIntersect(*city))
					{
						sBomb->Destroy();
						MakeExplosion(sBomb->Position);
						city->Destroy();
					}
				}

				for (const auto &base : ABMBaseManager->ABMBases)
				{
					if (sBomb->CirclesIntersect(base->Position, base->Radius))
					{
						sBomb->Destroy();
						MakeExplosion(sBomb->Position);
						base->Clear();
					}
				}
			}
		}

	for (auto missile : Enemies->ICBMControl->ICBMs)
	{
		if (missile->Enabled)
		{
			missileCount++;

			for (int i = 0; i < Explosions.size(); i++)
			{
				if (Explosions[i]->Enabled)
				{
					if (missile->CirclesIntersect(*Explosions[i]))
					{
						MakeExplosion(missile->Position);
						missile->Destroy();
						Score.AddToScore(25 * ScoreMultiplier);
					}
				}
			}

			for (const auto &base : ABMBaseManager->ABMBases)
			{
				if (missile->CirclesIntersect(base->Position, base->Radius))
				{
					missile->Destroy();
					MakeExplosion(missile->Position);
					base->Clear();
				}
			}

			for (const auto &city : CityManager->Cities)
			{
				if (missile->CirclesIntersect(*city))
				{
					missile->Destroy();
					MakeExplosion(city->Position);
					city->Destroy();
				}
			}
		}
	}

	if (missileCount == 0 && Enemies->ICBMControl->OutOfMissiles)
		ReadyForNextWave = true;
}

void GameLogic::CheckExplosionsActive()
{
	bool done = true;

	for (const auto &explosion : Explosions)
	{
		if (explosion->Enabled)
		{
			done = false;
		}
	}

	if (done) State = StartNewWave;
}

void GameLogic::NextWave()
{
	Wave++;
	ReadyForNextWave = false;
	int cityCount = 0;

	size_t waveColor = (Wave / 2);

	if (waveColor > 9) waveColor = 0;

	const Color backgroundColor = WaveColors[waveColor].Background;
	const Color groundColor = WaveColors[waveColor].Ground;
	const Color cityMainABMColor = WaveColors[waveColor].CityMainABM;
	const Color cityInnerColor = WaveColors[waveColor].CityInner;
	const Color icbmColor = WaveColors[waveColor].ICBM;

	if (Wave < 11)	ScoreMultiplier = (int)(Wave / 2) + 1;

	for (int i = 0; i < 6; i++)
	{
		if (CityManager->Cities[i]->Enabled) cityCount++;
	}

	Score.AddToScore((cityCount * 100) * ScoreMultiplier);

	int missileBonus = 0;

	for (const auto &abmBase : ABMBaseManager->ABMBases)
	{
		missileBonus += abmBase->GetMissileCount();
	}

	Score.AddToScore((missileBonus * 5) * ScoreMultiplier);

	for (int i = 0; i < 3; i++)
	{
		if (Score.GetScore() > CityManager->NextBonusCityAmount)
		{
			CityManager->BonusCities++;
			CityManager->NextBonusCityAmount += 8000;
		}
	}

	CityManager->NewWave(cityMainABMColor, cityInnerColor);

	if (cityCount < 1)
	{
		State = Ended;
		return;
	}

	for (int i = 0; i < 6; i++)
	{
		Enemies->ICBMControl->Cities[i].Active = CityManager->Cities[i]->Enabled;
		Enemies->ICBMControl->Cities[i].Targeted = false;
	}

	Background->WaveColor(groundColor, backgroundColor);
	Player->NextWave(cityMainABMColor);
	ABMBaseManager->Reset(cityMainABMColor);
	Enemies->NextWave(Wave, icbmColor, cityMainABMColor, groundColor);
	Score.SetColor(icbmColor);

	State = InPlay;
}

void GameLogic::IsOver()
{
	GameEnded = true;
	Player->GameOver = true;
	Player->Destroy();
}

void GameLogic::GameStateSwitch()
{
	switch (State)
	{
	case MainMenu:
		InMainMenu();
		break;
	case InPlay:
		InGame();
		break;
	case Ended:
		IsOver();
		break;
	case Bonus:
		break;
	case BonusCity:
		break;
	case BonusCityAwarded:
		break;
	case Attract:
		break;
	case HighScores:
		break;
	case StartNewWave:
		NextWave();
		break;
	default:
		break;
	}
}

void GameLogic::Reset()
{
	for (const auto &explosion : Explosions)
	{
		explosion->Destroy();
	}

	Enemies->Reset();
	Player->Reset();
}
