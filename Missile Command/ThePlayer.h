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

	std::vector<Shot*> ABMs = {};
	std::vector<Model3D*> Targets = {};

	void SetTheABMBaseManager(TheABMBaseManager* baseManager);

	void SetABMLaunchSound(Sound sound);
	void SetLowOnAmmoSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void SetTargetandShotModel(Model& targetModel, Model& shotModel);

	void Input();
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Draw3D();

	void Reset();
	void NextWave(Color color);
	void Spawn(Vector3 position);
	void NewGame();

private:
	size_t TargetColorTimerID = 0;
	float ShotSpeed = 5.0f * 60.0f * 2.15f;

	Color CurrentColor = Blue;

	Model ShotModel = { 0 };
	Model TargetModel = { 0 };

	Colors GameColors;

	Sound ABMLaunchSound = { 0 };
	Sound LowOnAmmoSound = { 0 };

	TheABMBaseManager* ABMBaseManager = nullptr;

	void FireABM();
	void CrosshairUpdate();
	void TargetUpdate();
	size_t SetTarget();
	void Gamepad();
	void Keyboard();
	void Mouse();
};