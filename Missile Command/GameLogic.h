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
#include "TheBonusScreen.h"

enum GameState
{
	Ended,
	InPlay,
	Pause,
	TheHighScores,
	TheMainMenu,
	TheSettingsMenu,
	GameOverHighScoreScreen,
	GameOverExplodeAnimation,
	TheAttractAnimation,
	TheWaveEnded,
	TheBonusPoints,
	TheBonusCityAwarded,
	TheBlankTheScreen,
	TimeForNewWave,
	TheDisplayScoreMultiplier,
	TimeToStartNewWave
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
	bool CountingAmmo = false;
	bool CountingCities = false;
	bool CityAnimationDone = false;
	bool AllCitiesCounted = false;
	bool DisplayBonusCityText = false;

	size_t AmmoCounted = 0;
	size_t TotalAmnoAtEnd = 0;
	size_t CityAnimationOnCity = 0;
	size_t BonusAmmoCountDelayTimerID = 0;
	size_t BonusCityCountDelayTimerID = 0;
	size_t BonusDoneCountingTimerID = 0;
	size_t BonusCityTextDisplayTimerID = 0;
	size_t BonusCityTextDoneTimerID = 0;
	size_t BonusCityAnimationDelayTimerID = 0;
	size_t BonusPointsBlankDelayTimerID = 0;
	size_t ScoreMultiplierDelayTimerID = 0;
	size_t WaveCrosshairDelayTimerID = 0;
	size_t WaveStartDelayTimerID = 0;

	unsigned Wave = 0;
	int ScoreMultiplier = 1;

	Vector2 AdjustedFieldSize = {};

	Model ExplosionModel = { 0 };

	WaveColorData WaveColors[10] = {};

	unsigned WaveMultiplier[8] = {};

	Colors GameColors = {};

	TheBackground* Background = {};
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	TheCityManager* CityManager = {};
	TheABMBaseManager* ABMBaseManager = {};
	GameOverScreen* GameOverText = {};
	TheBonusScreen* BonusText = {};

	std::vector<TheExplosion*> Explosions = {};

	void MakeExplosion(Vector3 position, bool playerMade = false);
	void InGame();
	void InMainMenu();
	void InGameOver();
	void CheckABMs();
	void CheckICBMs();
	bool CheckExplosionsActive();
	void WaveEnded();
	void DisplayBonusPoints();
	void BonusCityAwarded();
	void ClearScreen();
	void GoBlank();
	void NextWave();
	void DisplayScoreMultiplier();
	void StartTheNewWave();
	void IsOver();
	void GameStateSwitch();
	void Reset();
};