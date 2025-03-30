#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "MissileManager.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	void SetPlayer(ThePlayer* player);
	void SetMissileModels(Model &missileModel, Model &trailmodel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void NewGame();

private:

	ThePlayer *Player = nullptr;
	MissileManager *Missiles = nullptr;

	void Reset();
};