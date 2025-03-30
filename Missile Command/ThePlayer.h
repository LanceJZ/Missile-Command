#pragma once
#include "Globals.h"
#include "Colors.h"
#include "Model3D.h"
#include "Shot.h"
#include "MissileBaseManager.h"

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

	std::vector<Shot*> Missiles = {};
	std::vector<Model3D*> Targets = {};

	void SetMissileManager(MissileBaseManager* baseManager);

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
	int NextNewLifeScore = 10000;
	float ShotSpeed = 200.0f;

	Color TargetColor = Blue;

	Model ShotModel = { 0 };
	Model TargetModel = { 0 };

	Colors GameColors;

	MissileBaseManager* BaseManager = nullptr;

	void FireMissile();
	void CrosshairUpdate();
	void TargetUpdate();
	size_t SetTarget();
	void EntityFactory(Model3D* entity, Model& model, Color color,
	Vector3 position, Vector3 velocity);
	void Gamepad();
	void Keyboard();
	void Mouse();
};
