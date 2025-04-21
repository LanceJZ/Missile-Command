#pragma once
#include "Globals.h"
#include "TheCity.h"
#include "Colors.h"

class TheCityManager : public Common
{
public:
	TheCityManager();
	virtual ~TheCityManager();

	TheCity* Cities[6];

	bool Initialize();
	bool BeginRun();

	void SetCityModels(Model& innerModel, Model& mainModel);

	void Update();

	void SetColors(Color mainColor, Color innerColor, Color bonusColor);
	void NewGame();
	void Clear();
	void BonusCitiesUsed();
	void ReturnActiveCities();
	void CalculateActiveCityCount();
	bool BonusCityAwarded();
	void ShowNextCountedCity(size_t cityCount);

	bool CityCounted();

	size_t GetCityCount();

private:
	size_t ActiveCityCount = 0;
	unsigned BonusCities = 0;
	unsigned NextBonusCityAmount = 8000;

	TheCity* CityCount[6];
};

