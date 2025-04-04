#pragma once
#include "Globals.h"
#include "Colors.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "TheCityManager.h"
#include "TheABMBaseManager.h"
#include "TheExplosion.h"

enum GameState
{
	Ended,
	InPlay,
	Bonus,
	BonusPoints,
	BonusCity,
	BonusCityAwarded,
	Pause,
	HighScores,
	MainMenu,
	Attract,
	StartNewWave
};

struct WaveColorData
{
	Color Background = {};
	Color Ground = {};
	Color CityandABM = {};
	Color CityInner = {};
	Color ICBM = {};
};

class GameLogic : public Common
{
public:
	GameLogic();
	virtual ~GameLogic();

	bool PlayBackgroundMusic = true;

	GameState State = InPlay;

	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);
	void SetTheCityManager(TheCityManager* cityManager);
	void SetMissileBases(TheABMBaseManager* missileBases);
	void SetExplosionModel(Model& model);

	bool Initialize();
	bool BeginRun();
	bool Load();

	void FixedUpdate();
	void Input();
	void NewGame();

private:
	bool GameEnded = false;
	bool Paused = false;
	bool ReadyForNextWave = false;

	unsigned Wave = 0;
	unsigned NextNewCityScore = 10000;

	Vector2 AdjustedFieldSize = {};

	Model ExplosionModel = { 0 };

	WaveColorData WaveColors[10] = {};

	Colors GameColors;

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	TheCityManager* CityManager = {};
	TheABMBaseManager* ABMBaseManager = {};

	std::vector<TheExplosion*> Explosions = {};

	void MakeExplosion(Vector3 position);
	void InGame();
	void InMainMenu();
	void CheckABMs();
	void CheckICBMs();
	void CheckExplosionsActive();
	void NextWave();
	void IsOver();
	void GameStateSwitch();
};