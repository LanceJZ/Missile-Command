#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheICBMManager.h"
#include "Colors.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	TheICBMManager *ICBMControl = nullptr;

	void SetPlayer(ThePlayer* player);
	void SetICBMModel(Model &missileModel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void NewGame();

private:

	ThePlayer *Player = nullptr;

	void Reset();
};