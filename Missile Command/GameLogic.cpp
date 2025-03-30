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
	MissileBases = missileBases;
	Player->SetMissileManager(missileBases);
}

bool GameLogic::Initialize()
{
	Common::Initialize();

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	State = MainMenu;

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

void GameLogic::GamePlay()
{
}

void GameLogic::CheckPlayerMissiles()
{
	for (auto missile : Player->Missiles)
	{
		if (missile->Enabled)
		{
			if (missile->CirclesIntersect(missile->TargetPosition, 5.0f))
			{
				missile->Destroy();
				Player->Targets.at(missile->TargetIndex)->Destroy();
			}
		}
	}
}

void GameLogic::IsOver()
{
}
