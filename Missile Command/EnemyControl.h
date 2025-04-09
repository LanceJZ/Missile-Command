#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheICBMManager.h"
#include "TheFlier.h"
#include "TheSmartBomb.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	unsigned Wave = 0;
	float FlierCooldown[7] = {};

	TheICBMManager *ICBMControl = nullptr;
	TheFlier *Flier = nullptr;
	TheSmartBomb* SmartBombs[3] = {};

	void SetPlayer(ThePlayer* player);
	void SetICBMModel(Model &missileModel);
	void SetBomberModel(Model &bomberModel);
	void SetSateliteModel(Model &sateliteModel, Model &sateliteInnerModel);
	void SetSmartBombModel(Model &smartBombMainModel, Model &smartBombEdgeModel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void ResetLaunchTimer();
	void NextWave(unsigned wave, Color mainColor, Color innerColor, Color edgeColor);
	void NewGame();
	void Reset();

private:

	size_t FlierLaunchTimerID = 0;

	ThePlayer *Player = nullptr;

	Model BomberModel = {};
	Model SateliteModel = {};

	void SpawnFlier();
};