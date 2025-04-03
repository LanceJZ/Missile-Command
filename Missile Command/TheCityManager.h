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

	void SetCityModels(Model& InnerModel, Model& OuterModel);

	void Update();

private:


};

