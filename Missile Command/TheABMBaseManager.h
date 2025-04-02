#pragma once
#include "Globals.h"
#include "TheABMBase.h"
#include "Colors.h"

class TheABMBaseManager : public Common
{
public:
	TheABMBaseManager();
	virtual ~TheABMBaseManager();

	TheABMBase* ABMBases[3];

	bool Initialize();
	bool BeginRun();

	void SetMissileModel(Model& model);

	void Update();

	void Reset();
	bool MissileFired(size_t baseNumber);

private:

};

