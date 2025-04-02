#include "MissileBaseManager.h"

MissileBaseManager::MissileBaseManager()
{
	for (int i = 0; i < 3; i++)
	{
		EM.AddCommon(ABMBases[i] = DBG_NEW TheMissileBase());
	}
}

MissileBaseManager::~MissileBaseManager()
{
}

bool MissileBaseManager::Initialize()
{
	Common::Initialize();

	return false;
}

bool MissileBaseManager::BeginRun()
{
	Common::BeginRun();

	float positionX = (21.0f - 256.0f / 2) * 5.0f;
	float height = ((float)WindowHalfHeight -22.0f * 5);

	ABMBases[0]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (124.0f - 256.0f / 2) * 5.0f;

	ABMBases[1]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (240.0f - 256.0f / 2) * 5.0f;

	ABMBases[2]->SpawnMissileBase({ positionX, height, 0.0f });

	return false;
}

void MissileBaseManager::SetMissileModel(Model& model)
{
	for (int i = 0; i < 3; i++)
	{
		ABMBases[i]->SetMissileModel(model);
	}
}

void MissileBaseManager::Update()
{
	Common::Update();

}

void MissileBaseManager::Reset()
{
	for (auto base : ABMBases)
	{
		base->Reset();
	}
}

bool MissileBaseManager::MissileFired(size_t baseNumber)
{
	return (ABMBases[baseNumber]->MissileFired());
}