#pragma once
#include "Globals.h"
#include "ThePlayer.h"
#include "TheICBMManager.h"
#include "TheFlier.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	unsigned Wave = 0;
	float FlierCooldown[7] = {};

	TheICBMManager *ICBMControl = nullptr;
	TheFlier *Flier = nullptr;

	void SetPlayer(ThePlayer* player);
	void SetICBMModel(Model &missileModel);
	void SetBomberModel(Model &bomberModel);
	void SetSateliteModel(Model &sateliteModel, Model &sateliteInnerModel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void ResetLaunchTimer();
	void NextWave(unsigned wave, Color mainColor, Color innerColor);
	void NewGame();

private:

	size_t FlierLaunchTimerID = 0;

	ThePlayer *Player = nullptr;

	Model BomberModel = {};
	Model SateliteModel = {};

	void SpawnFlier();
	void Reset();
};