#include "GameLogic.h"

GameLogic::GameLogic()
{

}

GameLogic::~GameLogic()
{
}

void GameLogic::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void GameLogic::SetEnemies(EnemyControl* enemies)
{
	Enemies = enemies;
}

void GameLogic::SetCityManager(CityManager* cityManager)
{
	Cities = cityManager;
}

void GameLogic::SetMissileBases(MissileBaseManager* missileBases)
{
	ABMBases = missileBases;
	Player->SetMissileBaseManager(missileBases);
}

void GameLogic::SetExplosionModel(Model& model)
{
	ExplosionModel = model;
}

bool GameLogic::Initialize()
{
	Common::Initialize();

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	State = MainMenu;

	WaveColors[0].Background = BLACK;
	WaveColors[0].Ground = YELLOW;
	WaveColors[0].CityandABM = Blue;
	WaveColors[0].CityInner = Aqua;
	WaveColors[0].ICBM = RED;

	WaveColors[1].Background = BLACK;
	WaveColors[1].Ground = YELLOW;
	WaveColors[1].CityandABM = Blue;
	WaveColors[1].CityInner = Aqua;
	WaveColors[1].ICBM = LightGreen;

	WaveColors[2].Background = BLACK;
	WaveColors[2].Ground = Blue;
	WaveColors[2].CityandABM = LightGreen;
	WaveColors[2].CityInner = Yellow;
	WaveColors[2].ICBM = Red;

	WaveColors[3].Background = BLACK;
	WaveColors[3].Ground = Red;
	WaveColors[3].CityandABM = Blue;
	WaveColors[3].CityInner = Yellow;
	WaveColors[3].ICBM = Yellow;

	WaveColors[4].Background = Blue;
	WaveColors[4].Ground = Yellow;
	WaveColors[4].CityandABM = BLACK;
	WaveColors[4].CityInner = Magenta;
	WaveColors[4].ICBM = Red;

	WaveColors[5].Background = Aqua;
	WaveColors[5].Ground = Yellow;
	WaveColors[5].CityandABM = Blue;
	WaveColors[5].CityInner = BLACK;
	WaveColors[5].ICBM = Red;

	WaveColors[6].Background = Magenta;
	WaveColors[6].Ground = LightGreen;
	WaveColors[6].CityandABM = Yellow;
	WaveColors[6].CityInner = BLACK;
	WaveColors[6].ICBM = BLACK;

	WaveColors[7].Background = Yellow;
	WaveColors[7].Ground = LightGreen;
	WaveColors[7].CityandABM = Red;
	WaveColors[7].CityInner = WHITE;
	WaveColors[7].ICBM = BLACK;

	WaveColors[8].Background = WHITE;
	WaveColors[8].Ground = Red;
	WaveColors[8].CityandABM = LightGreen;
	WaveColors[8].CityInner = Yellow;
	WaveColors[8].ICBM = Magenta;

	WaveColors[9].Background = Red;
	WaveColors[9].Ground = Yellow;
	WaveColors[9].CityandABM = Blue;
	WaveColors[9].CityInner = LightGreen;
	WaveColors[9].ICBM = BLACK;

	return true;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	GameEnded = true;

	return false;
}

bool GameLogic::Load()
{
	return false;
}

void GameLogic::FixedUpdate()
{
	Common::FixedUpdate();

	if (State == Pause)
	{
		if (IsKeyPressed(KEY_P) || (IsGamepadAvailable(0)
			&& IsGamepadButtonPressed(0, 13)))
		{
			State = InPlay;
			Player->Paused = false;
		}

		return;
	}
	if (State == MainMenu)
	{
		if (!GameEnded)
		{
		}
		else
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
	}
	else if (State == Player->GameOver)
	{
		IsOver();
	}
	else if (State == InPlay)
	{
		GamePlay();
	}


	CheckPlayerMissiles();
}

void GameLogic::Input()
{
}

void GameLogic::NewGame()
{
	Player->NewGame();
	Enemies->NewGame();
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
}

void GameLogic::GamePlay()
{
}

void GameLogic::CheckPlayerMissiles()
{
	for (auto missile : Player->Missiles)
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

void GameLogic::IsOver()
{
}
