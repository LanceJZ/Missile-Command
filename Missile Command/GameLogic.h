#pragma once
#include "Globals.h"
#include "Colors.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "CityManager.h"
#include "MissileBaseManager.h"
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
	Attract
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

	GameState State = MainMenu;

	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);
	void SetCityManager(CityManager* cityManager);
	void SetMissileBases(MissileBaseManager* missileBases);
	void SetExplosionModel(Model& model);

	bool Initialize();
	bool BeginRun();
	bool Load();

	void FixedUpdate();
	void Input();
	void NewGame();

private:
	bool GameEnded = false;
	unsigned Wave = 0;

	Vector2 AdjustedFieldSize = {};

	Model ExplosionModel = { 0 };

	WaveColorData WaveColors[10] = {};

	Colors GameColors;

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	CityManager* Cities = {};
	MissileBaseManager* ABMBases = {};

	std::vector<TheExplosion*> Explosions = {};

	void MakeExplosion(Vector3 position);
	void GamePlay();
	void CheckPlayerMissiles();
	void IsOver();
};