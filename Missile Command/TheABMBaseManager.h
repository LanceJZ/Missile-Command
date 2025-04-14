#pragma once
#include "Globals.h"
#include "TheABMBase.h"
#include "Colors.h"

class TheABMBaseManager : public Common
{
public:
	TheABMBaseManager();
	virtual ~TheABMBaseManager();

	bool OutOfAmmo = false;

	TheABMBase* ABMBases[3];

	bool Initialize();
	bool BeginRun();

	void SetMissileModel(Model& model);

	void Update();

	void Clear();
	void Reset(Color color);
	bool MissileFired(size_t baseNumber);

private:

};

