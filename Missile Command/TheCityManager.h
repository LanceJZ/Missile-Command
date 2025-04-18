#pragma once
#include "Globals.h"
#include "TheCity.h"
#include "Colors.h"
#include <vox_loader.h>

class TheCityManager : public Common
{
public:
	TheCityManager();
	virtual ~TheCityManager();

	unsigned BonusCities = 0;
	unsigned NextBonusCityAmount = 8000;

	TheCity* Cities[6];

	bool Initialize();
	bool BeginRun();

	void SetCityModels(Model& innerModel, Model& mainModel);

	void Update();

	void NewWave(Color mainColor, Color innerColor);
	void NewGame();
	void Clear();
	bool CityCounted();

private:


};

