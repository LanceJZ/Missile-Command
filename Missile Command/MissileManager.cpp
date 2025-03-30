#include "MissileManager.h"

MissileManager::MissileManager()
{

}

MissileManager::~MissileManager()
{
}

void MissileManager::SetMissileModels(Model& missileModel, Model& trailmodel)
{
	MissileModel = missileModel;
	MissileTrailModel = trailmodel;
}

bool MissileManager::Initialize()
{
	Common::Initialize();

	return false;
}

bool MissileManager::BeginRun()
{

	return false;
}

void MissileManager::Update()
{
	Common::Update();

}
