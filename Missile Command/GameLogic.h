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
	NewGameStart,
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

	void SetWaveStartSound(Sound sound);
	void SetExplosionSound(Sound sound);
	void SetFlierSound(Sound sound);
	void SetSmartBombSound(Sound sound);
	void SetAmmoCityCountSound(Sound ammo, Sound city);
	void SetBonusCitySound(Sound sound);

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
	size_t FlierSoundDelayTimerID = 0;
	size_t SmartBombSoundDelayTimerID = 0;

	unsigned Wave = 0;
	int ScoreMultiplier = 1;

	Vector2 AdjustedFieldSize = {};

	Model ExplosionModel = { 0 };

	WaveColorData WaveColors[10] = {};

	unsigned WaveMultiplier[8] = {};

	Colors GameColors = {};

	Sound ExplosionSound = { 0 };
	Sound CityAwardedSound = { 0 };
	Sound AmmoCountedSound = { 0 };
	Sound CityCountedSound = { 0 };
	Sound WaveStartSound = { 0 };
	Sound FlierSound = { 0 };
	Sound SmartBombSound = { 0 };
	Sound BonusCitySound = { 0 };

	TheBackground* Background = nullptr;
	ThePlayer* Player = nullptr;
	EnemyControl* Enemies = nullptr;
	TheCityManager* CityManager = nullptr;
	TheABMBaseManager* ABMBaseManager = nullptr;
	GameOverScreen* GameOverText = nullptr;
	TheBonusScreen* BonusText = nullptr;

	std::vector<TheExplosion*> Explosions = {};

	void MakeExplosion(Vector3 position, bool playerMade = false);
	void StartNewGame();
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