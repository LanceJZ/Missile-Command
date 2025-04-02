#pragma once
#include "Globals.h"
#include "TheMissileBase.h"
#include "Colors.h"

class MissileBaseManager : public Common
{
public:
	MissileBaseManager();
	virtual ~MissileBaseManager();

	TheMissileBase* ABMBases[3];

	bool Initialize();
	bool BeginRun();

	void SetMissileModel(Model& model);

	void Update();

	void Reset();
	bool MissileFired(size_t baseNumber);

private:

};

