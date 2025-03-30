#include "CityManager.h"

CityManager::CityManager()
{
	for (int i = 0; i < 6; i++)
	{
		EM.AddModel3D(Cities[i] = DBG_NEW TheCity());
	}
}

CityManager::~CityManager()
{
}

bool CityManager::Initialize()
{
	Common::Initialize();

	//256 is the width of the ground model. 5 is the scale of the ground model.

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

	return false;
}

bool CityManager::BeginRun()
{

	return false;
}

void CityManager::SetCityModels(Model& innerModel, Model& outerModel)
{
	for (int i = 0; i < 6; i++)
	{
		Cities[i]->SetCityModels(innerModel, outerModel);
	}
}

void CityManager::Update()
{
	Common::Update();

}
