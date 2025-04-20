#include "GameLogic.h"

GameLogic::GameLogic()
{
	EM.AddOnScreenText(GameOverText = DBG_NEW GameOverScreen());
	EM.AddOnScreenText(BonusText = DBG_NEW TheBonusScreen());

	BonusAmmoCountDelayTimerID = EM.AddTimer(0.15f);
	BonusCityCountDelayTimerID = EM.AddTimer(0.25f);
	BonusCityAnimationDelayTimerID = EM.AddTimer(0.5f);
	BonusDoneCountingTimerID = EM.AddTimer(1.0f);
	BonusCityAwardedTimerID = EM.AddTimer(3.0f);
	BonusPointsBlankDelayTimerID = EM.AddTimer(0.5f);
	WaveCrosshairDelayTimerID = EM.AddTimer(2.0f);
	ScoreMultiplierDelayTimerID = EM.AddTimer(2.2f);
	WaveStartDelayTimerID =	EM.AddTimer(3.0f);
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

	NewGame();
	IsOver();

	return true;
}

bool GameLogic::Load()
{
	return false;
}

void GameLogic::FixedUpdate()
{
	Common::FixedUpdate();

	GameStateSwitch();
}

void GameLogic::Update()
{
	Common::Update();

}

void GameLogic::Input()
{
	if (State == TheMainMenu)
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
	else if (State == Ended)
	{
	}

#ifdef _DEBUG
	if (State == InPlay)
	{
		if (IsKeyPressed(KEY_W))
		{
			Reset();
			NextWave();

			const std::string wave = "Advanced to Wave " + std::to_string(Wave + 1);

			TraceLog(LOG_INFO, wave.c_str());
		}

		if (IsKeyPressed(KEY_O))
		{
			CityManager->Clear();
		}
	}
#endif
}

void GameLogic::NewGame()
{
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

	Wave = 0;
	GameOverText->Enabled = false;
	Score.ClearScore();
	Score.SetColor(Red);
	HighScore.SetColor(Red);
	Background->WaveColor(Yellow, BLACK);
	Player->NewGame();
	Enemies->NewGame();
	CityManager->NewGame();
	ABMBaseManager->Reset(Blue);
	State = InPlay;
}

void GameLogic::MakeExplosion(Vector3 position, bool playerMade)
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

	Explosions.at(explosionNumber)->PlayerMade = playerMade;

	for (const auto& sBomb : Enemies->ICBMControl->SmartBombs)
	{
		sBomb->SetExplosionRefs(Explosions);
	}
}

void GameLogic::InGame()
{
	CheckABMs();
	CheckICBMs();

	if (ReadyForNextWave) if (CheckExplosionsActive()) State = TheWaveEnded;

	for (const auto& sBomb : Enemies->ICBMControl->SmartBombs)
	{
		sBomb->SetTargetRefs(Player->Targets);
	}

	if (OutofAmmo)
	{
		if (CheckExplosionsActive()) GetToEndofWaveFast = true;

		if (GetToEndofWaveFast && Enemies->ICBMControl->CityTargetsDestroyed)
			JustEndIt = true;
	}
}

void GameLogic::InMainMenu()
{

}

void GameLogic::InGameOver()
{
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
				MakeExplosion(missile->Position, true);
			}
		}
	}

	if (ABMBaseManager->OutOfAmmo)
	{
		OutofAmmo = true;
	}
}

void GameLogic::CheckICBMs()
{
	int missileCount = 0;

	for (int i = 0; i < Explosions.size(); i++)
	{
		if (Enemies->Flier->CirclesIntersect(*Explosions[i]))
		{
			MakeExplosion(Enemies->Flier->Position, true);
			if (Explosions[i]->PlayerMade) Score.AddToScore(100 * ScoreMultiplier);
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
					MakeExplosion(sBomb->Position, Explosions[i]->PlayerMade);
					if (Explosions[i]->PlayerMade)
						Score.AddToScore(125 * ScoreMultiplier);
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
				if (sBomb->CirclesIntersect(city->Position, city->Radius))
				{
					sBomb->Destroy();
					MakeExplosion(sBomb->Position, false);
					city->Destroy();
				}
			}

			for (const auto &base : ABMBaseManager->ABMBases)
			{
				if (sBomb->CirclesIntersect(base->Position, base->Radius))
				{
					sBomb->Destroy();
					MakeExplosion(sBomb->Position, false);
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
						MakeExplosion(missile->Position, Explosions[i]->PlayerMade);
						missile->Destroy();
						if (Explosions[i]->PlayerMade)
							Score.AddToScore(25 * ScoreMultiplier);
					}
				}
			}

			for (const auto &base : ABMBaseManager->ABMBases)
			{
				if (missile->CirclesIntersect(base->Position, base->Radius))
				{
					missile->Destroy();
					MakeExplosion(missile->Position, false);
					base->Clear();
				}
			}

			for (const auto &city : CityManager->Cities)
			{
				if (missile->CirclesIntersect(city->Position, city->Radius))
				{
					missile->Destroy();
					MakeExplosion(city->Position, false);
					city->Destroy();
				}
			}
		}
	}

	if (missileCount == 0 && Enemies->ICBMControl->OutOfMissiles)
		ReadyForNextWave = true;
}

bool GameLogic::CheckExplosionsActive()
{
	bool done = true;

	for (const auto &explosion : Explosions)
	{
		if (explosion->Enabled)
		{
			done = false;
		}
	}

	return done;
}

void GameLogic::WaveEnded()
{
	State = TheBonusPoints;

	BonusText->Enabled = true;
	BonusText->ShowBonusPoints();

	CountingAmmo = true;
	CountingCities = false;
	AmmoCounted = 0;

	Player->Enabled = false;

	TotalAmnoAtEnd = 0;
	CityAnimationOnCity = -1;

	for (const auto &abmBase : ABMBaseManager->ABMBases)
	{
		TotalAmnoAtEnd += abmBase->GetMissileCount();
	}
}

void GameLogic::DisplayBonusPoints()
{
	if (CountingAmmo)
	{
		for (const auto &abmBase : ABMBaseManager->ABMBases)
		{
			size_t ammoCount = abmBase->GetMissileCount();

			for (int i = 0; i < ammoCount; i++)
			{
				if (EM.TimerElapsed(BonusAmmoCountDelayTimerID))
				{
					EM.ResetTimer(BonusAmmoCountDelayTimerID);

					if (abmBase->MissileCounted(ammoCount - 1, AmmoCounted))
					{
						AmmoCounted++;
						BonusText->SetBonusAmmo(AmmoCounted * 5 * ScoreMultiplier);
					}

					if (AmmoCounted == TotalAmnoAtEnd)
					{
						Score.AddToScore((TotalAmnoAtEnd * 5) * ScoreMultiplier);
						CountingAmmo = false;
						CountingCities = true;
						CityAnimationDone = true;
						CityActiveAtEndOfWave = CityManager->GetCityCount();
					}
				}
				else return;
			}
		}

	}
	else if (CountingCities)
	{
		if (EM.TimerElapsed(BonusCityAnimationDelayTimerID) && CityAnimationDone)
		{
			EM.ResetTimer(BonusCityCountDelayTimerID);

			for (const auto &city : CityManager->Cities)
			{
				if (city->Enabled)
				{
					city->Destroy();
					CityAnimationOnCity++;
					CityAnimationDone = false;
					break;
				}
			}
		}

		if (EM.TimerElapsed(BonusCityCountDelayTimerID) && !CityAnimationDone)
		{
			EM.ResetTimer(BonusCityAnimationDelayTimerID);

			CityManager->CityCount[CityAnimationOnCity]->Return();
			CityManager->CityCount[CityAnimationOnCity]->
				SetCountColor(BonusText->GetTextColor());
			BonusText->SetBonusCities((CityAnimationOnCity + 1)
				* 100 * ScoreMultiplier);

			if (CityAnimationOnCity + 1 == CityActiveAtEndOfWave)
			{
				CountingCities = false;
				Score.AddToScore((CityActiveAtEndOfWave * 100) * ScoreMultiplier);
			}
			else CityAnimationDone = true;

		}
	}
}

void GameLogic::BonusCityAwarded()
{
}

void GameLogic::GoBlank()
{
}

void GameLogic::NextWave()
{
	Wave++;
	ReadyForNextWave = false;
	GetToEndofWaveFast = false;
	OutofAmmo = false;

	Enemies->Reset();

	size_t waveColorSetNumber = (Wave / 2);

	if (waveColorSetNumber > 9) waveColorSetNumber = 0;

	const Color backgroundColor = WaveColors[waveColorSetNumber].Background;
	const Color groundColor = WaveColors[waveColorSetNumber].Ground;
	const Color cityMainABMColor = WaveColors[waveColorSetNumber].CityMainABM;
	const Color cityInnerColor = WaveColors[waveColorSetNumber].CityInner;
	const Color icbmColor = WaveColors[waveColorSetNumber].ICBM;

	if (Wave < 11)	ScoreMultiplier = (int)(Wave / 2) + 1;

	//int missileBonus = 0;

	////This to be moved to missile count.
	//for (const auto &abmBase : ABMBaseManager->ABMBases)
	//{
	//	missileBonus += abmBase->GetMissileCount();
	//}
	//
	//Score.AddToScore((missileBonus * 5) * ScoreMultiplier);

	//This to be moved to city count.
	//int cityCount = 0;
	//for (const auto &city : CityManager->Cities)
	//{
	//	if (city->Enabled) cityCount++;
	//}

	//Score.AddToScore((cityCount * 100) * ScoreMultiplier);

	for (int i = 0; i < 3; i++)
	{
		if (Score.GetScore() > CityManager->NextBonusCityAmount)
		{
			CityManager->BonusCities++;
			CityManager->NextBonusCityAmount += 8000;
		}
	}

	CityManager->NewWave(cityMainABMColor, cityInnerColor);

	bool noCities = true;

	for (const auto &city : CityManager->Cities)
	{
		if (city->Enabled) noCities = false;
	}

	if (noCities)
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
	HighScore.SetColor(icbmColor);
	GameOverText->BottomTextColor = backgroundColor;
	GameOverText->TopTextColor = icbmColor;
	BonusText->SetColors(cityMainABMColor, icbmColor);
}

void GameLogic::DisplayScoreMultiplier()
{
}

void GameLogic::StartTheNewWave()
{
	State = InPlay;
	Enemies->StartNewWave();
}

void GameLogic::IsOver()
{
	Enemies->ICBMControl->GameOver = true;
	Player->Destroy();
	Enemies->Reset();
	ABMBaseManager->Clear();
	CityManager->Clear();
	GameOverText->Enabled = true;

	State = TheMainMenu;
}

void GameLogic::GameStateSwitch()
{
	switch (State)
	{
	case TheMainMenu:
		InMainMenu();
		break;
	case InPlay:
		InGame();
		break;
	case TheWaveEnded:
		WaveEnded();
		break;
	case TheBonusPoints:
		DisplayBonusPoints();
		break;
	case TheBonusCityAwarded:
		BonusCityAwarded();
		break;
	case TheBlankTheScreen:
		GoBlank();
		break;
	case TimeForNewWave:
		NextWave();
		break;
	case TheDisplayScoreMultiplier:
		DisplayScoreMultiplier();
		break;
	case Ended:
		IsOver();
		break;
	case GameOverExplodeAnimation:
		break;
	case GameOverHighScoreScreen:
		break;
	case TheAttractAnimation:
		break;
	case TheHighScores:
		break;
	case TimeToStartNewWave:
		StartTheNewWave();
		break;
	default:
		State = TheMainMenu;
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
