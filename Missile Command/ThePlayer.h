#pragma once
#include "Globals.h"
#include "Colors.h"
#include "Model3D.h"
#include "Shot.h"
#include "TheABMBaseManager.h"

class ThePlayer : public Model3D
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;
	bool Paused = false;

	int Score { 0 };
	int HighScore { 0 };
	int Lives { 0 };

	std::vector<Shot*> ABMs = {};
	std::vector<Model3D*> Targets = {};

	void SetTheABMBaseManager(TheABMBaseManager* baseManager);

	bool Initialize();
	bool BeginRun();

	void SetTargetandShotModel(Model& targetModel, Model& shotModel);

	void Input();
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Hit();
	void Hit(Vector3 location, Vector3 velocity);
	void ScoreUpdate(int addToScore);
	void Reset();
	void Spawn(Vector3 position);
	void NewGame();
	void SetHighScore(int highScore);

	int GetScore();

private:
	size_t TargetColorTimerID = 0;
	unsigned NextNewCityScore = 10000;
	float ShotSpeed = 200.0f;

	Color CurrentColor = Blue;

	Model ShotModel = { 0 };
	Model TargetModel = { 0 };

	Colors GameColors;

	TheABMBaseManager* ABMBaseManager = nullptr;

	void FireABM();
	void CrosshairUpdate();
	void TargetUpdate();
	size_t SetTarget();
	void Gamepad();
	void Keyboard();
	void Mouse();
};