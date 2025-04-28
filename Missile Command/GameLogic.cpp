#include "GameLogic.h"

GameLogic::GameLogic()
{
	EM.AddOnScreenText(GameOverText = DBG_NEW GameOverScreen());
	EM.AddOnScreenText(BonusText = DBG_NEW TheBonusScreen());

	BonusAmmoCountDelayTimerID = EM.AddTimer(0.15f);
	BonusCityCountDelayTimerID = EM.AddTimer(0.25f);
	BonusCityAnimationDelayTimerID = EM.AddTimer(0.5f);
	BonusDoneCountingTimerID = EM.AddTimer(1.0f);
	BonusCityTextDisplayTimerID = EM.AddTimer(2.0f);
	BonusCityTextDoneTimerID = EM.AddTimer(2.0f);
	BonusPointsBlankDelayTimerID = EM.AddTimer(1.5f);
	WaveCrosshairDelayTimerID = EM.AddTimer(2.0f);
	ScoreMultiplierDelayTimerID = EM.AddTimer(2.25f);
	WaveStartDelayTimerID =	EM.AddTimer(3.0f);
	FlierSoundDelayTimerID = EM.AddTimer(0.363f);
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

void GameLogic::SetWaveStartSound(Sound sound)
{
	WaveStartSound = sound;
}

void GameLogic::SetExplosionSound(Sound sound)
{
	ExplosionSound = sound;
}

void GameLogic::SetFlierSound(Sound sound)
{
	FlierSound = sound;
}

void GameLogic::SetAmmoCityCountSound(Sound ammo, Sound city)
{
	AmmoCountedSound = ammo;
	CityCountedSound = city;
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

	WaveMultiplier[0] = 2;
	WaveMultiplier[1] = 2;
	WaveMultiplier[2] = 3;
	WaveMultiplier[3] = 3;
	WaveMultiplier[4] = 4;
	WaveMultiplier[5] = 4;
	WaveMultiplier[6] = 5;
	WaveMultiplier[7] = 5;

	return true;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	SetSoundVolume(FlierSound, 0.5f);
	SetSoundVolume(CityCountedSound, 0.5f);

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

	if (Enemies->Flier->Enabled && !JustEndIt && !GetToEndofWaveFast)
	{
		if (EM.TimerElapsed(FlierSoundDelayTimerID))
		{
			EM.ResetTimer(FlierSoundDelayTimerID);
			PlaySound(FlierSound);
		}
	}
}

void GameLogic::Input()
{
	if (State == TheMainMenu)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 15))//Start button
			{
				PlaySound(WaveStartSound);
				NewGame();
			}
		}
		else if (IsKeyPressed(KEY_N))
		{
			PlaySound(WaveStartSound);
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
			GetToEndofWaveFast = true;
			JustEndIt = true;
			Player->Enabled = false;
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
	CountingAmmo = false;
	CountingCities = false;
	OutofAmmo = false;
	ReadyForNextWave = false;
	DisplayBonusCityText = false;
	GameOverText->Enabled = false;
	Score.ClearScore();
	Score.SetColor(Red);
	HighScore.SetColor(Red);
	Background->WaveColor(Yellow, BLACK);
	Player->NewGame();
	CityManager->NewGame();
	ABMBaseManager->Reset(Blue);
	EM.ResetTimer(WaveStartDelayTimerID);
	State = NewGameStart;
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

	PlaySound(ExplosionSound);
	Explosions.at(explosionNumber)->PlayerMade = playerMade;

	for (const auto& sBomb : Enemies->ICBMControl->SmartBombs)
	{
		sBomb->SetExplosionRefs(Explosions);
	}
}

void GameLogic::StartNewGame()
{
	if (EM.TimerElapsed(WaveStartDelayTimerID))
	{
		Enemies->NewGame();
		State = InPlay;
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

	GetToEndofWaveFast = false;
	JustEndIt = false;
	CountingAmmo = true;
	CountingCities = false;
	AmmoCounted = 0;
	Player->Enabled = false;
	CityAnimationDone = true;
	AllCitiesCounted = false;
	TotalAmnoAtEnd = 0;
	CityAnimationOnCity = 0;
	CityManager->CalculateActiveCityCount();
	Enemies->Reset();

	for (const auto &abmBase : ABMBaseManager->ABMBases)
	{
		TotalAmnoAtEnd += abmBase->GetMissileCount();
	}

	if (TotalAmnoAtEnd < 1)
	{
		CountingAmmo = false;
		CountingCities = true;
	}
}

void GameLogic::DisplayBonusPoints()
{
	if (CountingAmmo)
	{
		BonusText->ShowBonusPoints();

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
						PlaySound(AmmoCountedSound);
						AmmoCounted++;
						BonusText->SetBonusAmmo((AmmoCounted * 5) * ScoreMultiplier);
					}

					if (AmmoCounted == TotalAmnoAtEnd)
					{
						Score.AddToScore((TotalAmnoAtEnd * 5) * ScoreMultiplier);
						CountingAmmo = false;
						CountingCities = true;
					}
				}
				else return;
			}
		}

	}
	else if (CountingCities)
	{
		EM.ResetTimer(BonusPointsBlankDelayTimerID);

		if (EM.TimerElapsed(BonusCityAnimationDelayTimerID) && CityAnimationDone)
		{
			EM.ResetTimer(BonusCityCountDelayTimerID);

			CityAnimationDone = false;

			for (const auto &city : CityManager->Cities)
			{
				if (city->Enabled)
				{
					city->Destroy();
					AllCitiesCounted = false;
					return;
				}
			}

			AllCitiesCounted = true;
		}
		else if (EM.TimerElapsed(BonusCityCountDelayTimerID) && !CityAnimationDone)
		{
			BonusText->ShowBonusPoints();

			EM.ResetTimer(BonusCityAnimationDelayTimerID);

			if (!AllCitiesCounted)
			{
				PlaySound(CityCountedSound);
				CityManager->ShowNextCountedCity(CityAnimationOnCity);
				CityAnimationOnCity++;

				BonusText->SetBonusCities(((CityAnimationOnCity)
					* 100) * ScoreMultiplier);

				CityAnimationDone = true;
				return;
			}
			else
			{
				CountingCities = false;
				Score.AddToScore((CityManager->GetCityCount() * 100)
					* ScoreMultiplier);

				if (CityManager->GetCityCount() < 6
					&& CityManager->BonusCityAwarded())
				{
					State = TheBonusCityAwarded;
					EM.ResetTimer(BonusCityTextDisplayTimerID);
					DisplayBonusCityText = false;
					return;
				}

				if (CityManager->GetCityCount() < 1)
				{
					State = Ended;
					BonusText->ClearPoints();
					return;
				}
			}
		}
	}
	else
	{
		if (EM.TimerElapsed(BonusPointsBlankDelayTimerID))
		{
			ClearScreen();
			State = TheBlankTheScreen;
			EM.ResetTimer(BonusPointsBlankDelayTimerID);
		}
	}
}

void GameLogic::BonusCityAwarded()
{
	if (EM.TimerElapsed(BonusCityTextDisplayTimerID) && !DisplayBonusCityText)
	{
		EM.ResetTimer(BonusCityTextDoneTimerID);
		DisplayBonusCityText = true;
		BonusText->ShowBonusCityText();
		CityManager->BonusCitiesUsed();
	}

	if (DisplayBonusCityText)
	{
		if (EM.TimerElapsed(BonusCityTextDoneTimerID))
		{
			EM.ResetTimer(BonusPointsBlankDelayTimerID);
			ClearScreen();
			State = TheBlankTheScreen;
		}
	}
}

void GameLogic::ClearScreen()
{
	Background->Clear();
	CityManager->Clear();
	BonusText->ClearPoints();
	ABMBaseManager->Clear();
	Score.Enabled = false;
	HighScore.Enabled = false;
}

void GameLogic::GoBlank()
{
	if (EM.TimerElapsed(BonusPointsBlankDelayTimerID))
	{
		EM.ResetTimer(ScoreMultiplierDelayTimerID);
		NextWave();
		State = TheDisplayScoreMultiplier;
	}
}

void GameLogic::NextWave()
{
	Wave++;
	ReadyForNextWave = false;
	GetToEndofWaveFast = false;
	OutofAmmo = false;
	Score.Enabled = true;
	HighScore.Enabled = true;

	Enemies->Reset();

	size_t waveColorSetNumber = (Wave / 2);

	if (waveColorSetNumber > 9) waveColorSetNumber = 0;

	const Color backgroundColor = WaveColors[waveColorSetNumber].Background;
	const Color groundColor = WaveColors[waveColorSetNumber].Ground;
	const Color cityMainABMColor = WaveColors[waveColorSetNumber].CityMainABM;
	const Color cityInnerColor = WaveColors[waveColorSetNumber].CityInner;
	const Color icbmColor = WaveColors[waveColorSetNumber].ICBM;

	CityManager->ReturnActiveCities();
	CityManager->SetColors(cityMainABMColor, cityInnerColor, icbmColor);

	for (int i = 0; i < 6; i++)
	{
		Enemies->ICBMControl->Cities[i].Active = CityManager->Cities[i]->Enabled;
		Enemies->ICBMControl->Cities[i].Targeted = false;
	}

	Score.SetColor(icbmColor);
	HighScore.SetColor(icbmColor);
	Background->WaveColor(groundColor, backgroundColor);
	Player->NextWave(cityMainABMColor);
	GameOverText->BottomTextColor = backgroundColor;
	GameOverText->TopTextColor = icbmColor;
	BonusText->SetColors(cityMainABMColor, icbmColor);
	PlaySound(WaveStartSound);
}

void GameLogic::DisplayScoreMultiplier()
{
	unsigned multiplier = 1;

	if (Wave > 1 && Wave < 10)
	{
		multiplier = WaveMultiplier[Wave - 2];
	}
	else if (Wave > 10)
	{
		multiplier = 6;
	}

	ScoreMultiplier = multiplier;

	BonusText->ShowScoreMultiplier(multiplier);

	if (EM.TimerElapsed(ScoreMultiplierDelayTimerID))
	{
		EM.ResetTimer(WaveStartDelayTimerID);
		BonusText->ClearPoints();
		State = TimeToStartNewWave;
	}
}

void GameLogic::StartTheNewWave()
{
	Player->Enabled = true;

	size_t waveColorSetNumber = (Wave / 2);

	if (waveColorSetNumber > 9) waveColorSetNumber = 0;

	const Color icbmColor = WaveColors[waveColorSetNumber].ICBM;
	const Color cityMainABMColor = WaveColors[waveColorSetNumber].CityMainABM;
	const Color groundColor = WaveColors[waveColorSetNumber].Ground;

	if (EM.TimerElapsed(WaveStartDelayTimerID))
	{
		//Delay before showing ammo.
		ABMBaseManager->Reset(WaveColors[waveColorSetNumber].CityMainABM);
		//Delay before enemies start to drop.
		Enemies->NextWave(Wave, icbmColor, cityMainABMColor, groundColor);
		BonusText->ClearPoints();
		Enemies->StartNewWave();
		State = InPlay;
	}
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
	case NewGameStart:
		StartNewGame();
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
