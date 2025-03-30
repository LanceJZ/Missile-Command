#pragma once
#include "Globals.h"
#include "TheCity.h"
#include "Colors.h"

class CityManager : public Common
{
public:
	CityManager();
	virtual ~CityManager();

	TheCity* Cities[6];

	bool Initialize();
	bool BeginRun();

	void SetCityModels(Model& InnerModel, Model& OuterModel);

	void Update();

private:


};

