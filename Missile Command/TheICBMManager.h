#pragma once
#include "Globals.h"
#include "Shot.h"
#include "Colors.h"

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

	unsigned Wave = 0;

	Shot* ICBMs[8];
	TargetData Citys[6] = {};
	TargetData ABMBases[3] = {};

	void SetMissileModels(Model& missileModel);

	bool Initialize();
	bool BeginRun();

	void Update();

	void FireIICBM(Vector3& position, Vector3& target);
	void NewWave();
	void EndWave();
	void Reset();

private:
	bool WaveEnded = false;

	size_t LaunchCheckTimerID = 0;

	unsigned ICBMsFiredMax = 12;
	unsigned ICBMsFiredThisWave = 0;

	float MissileSpeed = 20.15f;
	float LaunchCealing = 0.0f;
	float CealingPercent = 0.87f;
	const float MinimumCleaingPercent = 0.68f;

	unsigned NumberOfICBMsEachWave[19] = {};

	Color CurrentColor = Red;

	Colors GameColors;

	bool IsItTimeForAnotherSalvo();
	void FireSalvo();
};
