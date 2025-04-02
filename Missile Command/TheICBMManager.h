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

	std::vector<Shot*> ICBMs = {};
	TargetData Citys[6] = {};
	TargetData ABMBases[3] = {};

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
