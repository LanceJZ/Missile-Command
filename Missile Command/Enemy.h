#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

class Enemy : public LineModel
{
public:
	Enemy();
	virtual ~Enemy();

	float ShotTimerAmount = 0.0f;

	std::vector<Shot*> Shots;

	void SetPlayer(ThePlayer* player);

	void SetShotModel(LineModelPoints model);

	void SetFireSound(Sound fireSound);
	void SetExplodeSound(Sound explodeSound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	virtual void CheckShotsHitPlayer();

	virtual void Spawn();
	virtual void Spawn(Vector3 position);
	virtual void Destroy();
	virtual void Hit();
	virtual void Reset();

protected:
	size_t ShotTimerID = 0;

	int Points = 0;
	float Speed = 200.0f;
	float TurnSpeed = 1.45f;
	float RotateMagnitude = PI / 2;
	float ShotLifeTime = 0.0f;

	ThePlayer* Player = nullptr;
	Sound FireSound = {};
	Sound ExplodeSound = {};
	Sound OnSound = {};
	Sound SpawnSound = {};
	LineModelPoints ShotModel = {};

	virtual void Shoot();
	virtual void Shoot(Vector3 velocity);
	virtual void ChasePlayer();
	virtual void Explode();
	virtual bool CheckCollisions();

private:
};