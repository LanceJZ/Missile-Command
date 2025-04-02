#include "TheICBMManager.h"

TheICBMManager::TheICBMManager()
{

}

TheICBMManager::~TheICBMManager()
{
}

void TheICBMManager::SetMissileModels(Model& missileModel)
{
	ICBMModel = missileModel;
}

bool TheICBMManager::Initialize()
{
	Common::Initialize();

	return false;
}

bool TheICBMManager::BeginRun()
{

	return false;
}

void TheICBMManager::Update()
{
	Common::Update();

}
