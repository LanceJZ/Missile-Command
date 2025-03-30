#include "MissileBaseManager.h"

MissileBaseManager::MissileBaseManager()
{
	for (int i = 0; i < 3; i++)
	{
		EM.AddCommon(MissileBases[i] = DBG_NEW TheMissileBase());
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

	MissileBases[0]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (124.0f - 256.0f / 2) * 5.0f;

	MissileBases[1]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (240.0f - 256.0f / 2) * 5.0f;

	MissileBases[2]->SpawnMissileBase({ positionX, height, 0.0f });

	return false;
}

void MissileBaseManager::SetMissileModel(Model& model)
{
	for (int i = 0; i < 3; i++)
	{
		MissileBases[i]->SetMissileModel(model);
	}
}

void MissileBaseManager::Update()
{
	Common::Update();

}

void MissileBaseManager::Reset()
{
	for (auto base : MissileBases)
	{
		base->Reset();
	}
}

bool MissileBaseManager::MissileFired(size_t baseNumber)
{
	return (MissileBases[baseNumber]->MissileFired());
}