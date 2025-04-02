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

class MissileManager : public Common
{
public:
	MissileManager();
	virtual ~MissileManager();

	unsigned Wave = 0;

	std::vector<Shot*> ICBMs;
	TargetData Citys[6];
	TargetData ABMs[3];

	void SetMissileModels(Model& missileModel);

	bool Initialize();
	bool BeginRun();

	void Update();

private:

	float NewSalvoHieght = 0.0f;

	Color CurrentColor = Red;
	Model ICBMModel = { 0 };

	Colors GameColors;

};
