#pragma once
#include "Globals.h"
#include "Shot.h"
#include "Colors.h"
#include "TheFlier.h"

struct TargetData
{
	Vector3 Position;
	bool Active;
	bool Targeted;
};

class TheICBMManager : public Common
{
public:
	TheICBMManager();
	virtual ~TheICBMManager();

	bool OutOfMissiles = false;
	unsigned Wave = 0;

	Shot* ICBMs[8];
	TargetData Cities[6] = {};
	TargetData ABMBases[3] = {};

	void SetMissileModels(Model& missileModel);
	void SetBomberReference(TheFlier* bomber);

	bool Initialize();
	bool BeginRun();

	void Update();

	void ResetFlierFireTimer();
	void NewWave(Color waveColor);
	void EndWave();
	void Reset();
	void NewGame();

private:
	bool WaveEnded = false;

	size_t LaunchCheckTimerID = 0;
	size_t FlierFireTimerID = 0;

	unsigned ICBMsFiredMax = 12;
	unsigned ICBMsFiredThisWave = 0;
	unsigned CitiesTargeted = 0;
	const unsigned CitiesTargetedMax = 3;

	float MissileSpeed = 20.15f;
	float LaunchCealing = 0.0f;
	float CealingPercent = 0.87f;
	const float MinimumCleaingPercent = 0.58f;

	float FlierFireRate[7] = {};

	unsigned NumberOfICBMsEachWave[19] = {};
	float ICBMSpeedonWave[19] = {};

	Color CurrentColor = {};

	Colors GameColors;

	TheFlier* Flier = nullptr;

	bool IsItTimeForAnotherSalvo();
	void FireSalvo();
	bool FlierFires();
	void FireICBM(Shot* missile, Vector3& position);
	void CitiesToTarget();
	float GetLaunchCealing();
};
