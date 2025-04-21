#include "TheCityManager.h"

TheCityManager::TheCityManager()
{
	for (int i = 0; i < 6; i++)
	{
		EM.AddModel3D(Cities[i] = DBG_NEW TheCity());
		Cities[i]->Destroy();
		EM.AddModel3D(CityCount[i] = DBG_NEW TheCity());
		CityCount[i]->Destroy();
	}
}

TheCityManager::~TheCityManager()
{
}

bool TheCityManager::Initialize()
{
	Common::Initialize();

	// 256 is the X axis width of the ground model.
	// 5 is the scale of the ground model.

	Cities[0]->Position = { (45.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 14.15f * 5.0f, 0 };
	Cities[1]->Position = { (71.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 12.15f * 5.0f, 0 };
	Cities[2]->Position = { (95.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 12.0f * 5.0f, 0 };
	Cities[3]->Position = { (147.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 14.15f * 5.0f, 0 };
	Cities[4]->Position = { (180.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 19.0f * 5.0f, 0 };
	Cities[5]->Position = { (208.0f - 256.0f / 2) * 5.0f,
		(float)WindowHalfHeight - 13.0f * 5.0f, 0 };

	int cityCount = 0;

	for (const auto& city : CityCount)
	{
		city->Y(150.0f);
		city->X(cityCount * 85.0f);
		cityCount++;
	}

	return false;
}

bool TheCityManager::BeginRun()
{

	return false;
}

void TheCityManager::SetCityModels(Model& innerModel, Model& mainModel)
{
	for (int i = 0; i < 6; i++)
	{
		Cities[i]->SetCityModels(innerModel, mainModel);
		CityCount[i]->SetCityModels(innerModel, mainModel);
	}
}

void TheCityManager::Update()
{
	Common::Update();

}

void TheCityManager::SetColors(Color mainColor, Color innerColor)
{
	for (const auto &city : Cities)
	{
		city->SetColor(mainColor, innerColor);
	}

	for (const auto &city : CityCount)
	{
		city->SetColor(mainColor, innerColor);
	}
}

void TheCityManager::NewGame()
{
	BonusCities = 1;
	NextBonusCityAmount = 8000;

	for (const auto &city : Cities)
	{
		city->Return();
	}

	SetColors(Blue, Aqua);
}

void TheCityManager::Clear()
{
	for (const auto &city : Cities)
	{
		city->Destroy();
	}

	for (const auto &city : CityCount)
	{
		city->Destroy();
	}
}

void TheCityManager::ShowNextCountedCity(size_t cityCount, Color innerColor)
{
	CityCount[cityCount]->Return();
	CityCount[cityCount]->SetCountColor(innerColor);
}

bool TheCityManager::CityCounted()
{
	int lastInLine = -1;

	for (const auto &city : Cities)
	{
		if (city->Enabled) lastInLine++;
	}

	if (lastInLine  < 0) return false;

	Cities[lastInLine]->Destroy();

	return true;
}

size_t TheCityManager::GetCityCount()
{
	size_t count = 0;

	for (const auto &city : Cities)
	{
		if (city->Enabled) count++;
	}

	return count;
}
