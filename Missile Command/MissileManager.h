#pragma once
#include "Globals.h"
#include "TheMissile.h"
#include "Colors.h"

class MissileManager : public Common
{
public:
	MissileManager();
	virtual ~MissileManager();

	std::vector<TheMissile*> Missiles;

	void SetMissileModels(Model& missileModel, Model& trailmodel);

	bool Initialize();
	bool BeginRun();

	void Update();

private:

	Model MissileModel = { 0 };
	Model MissileTrailModel = { 0 };

};

