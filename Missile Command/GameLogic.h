#pragma once
#include "Globals.h"
#include "Colors.h"
#include "TheBackground.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "TheCityManager.h"
#include "TheABMBaseManager.h"
#include "TheExplosion.h"
#include "GameOverScreen.h"

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
	SettingsMenu,
	GameOverHighScoreScreen,
	GameOverExplodeAnimation,
	AttractAnimation,
	StartNewWave
};

struct WaveColorData
{
	Color Background = {};
	Color Ground = {};
	Color CityMainABM = {};
	Color CityInner = {};
	Color ICBM = {};
};

class GameLogic : public Common
{
public:
	GameLogic();
	virtual ~GameLogic();

	bool PlayBackgroundMusic = true;
	bool GetToEndofWaveFast = false;
	bool JustEndIt = false;

	GameState State = InPlay;

	void SetBackground(TheBackground* background);
	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);
	void SetTheCityManager(TheCityManager* cityManager);
	void SetMissileBases(TheABMBaseManager* missileBases);
	void SetExplosionModel(Model& model);

	bool Initialize();
	bool BeginRun();
	bool Load();

	void FixedUpdate();
	void Update();
	void Input();
	void NewGame();

private:
	bool Paused = false;
	bool ReadyForNextWave = false;
	bool OutofAmmo = false;

	size_t WaveColorNumber = 0;
	unsigned Wave = 0;
	unsigned NextNewCityScore = 10000;
	int ScoreMultiplier = 1;

	Vector2 AdjustedFieldSize = {};

	Model ExplosionModel = { 0 };

	WaveColorData WaveColors[10] = {};

	Colors GameColors;

	TheBackground* Background = {};
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	TheCityManager* CityManager = {};
	TheABMBaseManager* ABMBaseManager = {};
	GameOverScreen* GameOverText = {};

	std::vector<TheExplosion*> Explosions = {};

	void MakeExplosion(Vector3 position, bool playerMade = false);
	void InGame();
	void InMainMenu();
	void InGameOver();
	void CheckABMs();
	void CheckICBMs();
	bool CheckExplosionsActive();
	void NextWave();
	void IsOver();
	void GameStateSwitch();
	void Reset();
};