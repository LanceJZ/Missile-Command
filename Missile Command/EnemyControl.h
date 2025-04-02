#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "MissileManager.h"
#include "Colors.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	void SetPlayer(ThePlayer* player);
	void SetICBMModel(Model &missileModel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void NewGame();

private:

	ThePlayer *Player = nullptr;
	MissileManager *ICBMControl = nullptr;

	void Reset();
};