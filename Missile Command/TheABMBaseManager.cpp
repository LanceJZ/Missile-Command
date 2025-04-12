#include "TheABMBaseManager.h"

TheABMBaseManager::TheABMBaseManager()
{
	for (int i = 0; i < 3; i++)
	{
		EM.AddCommon(ABMBases[i] = DBG_NEW TheABMBase());
	}
}

TheABMBaseManager::~TheABMBaseManager()
{
}

bool TheABMBaseManager::Initialize()
{
	Common::Initialize();

	float positionX = (21.0f - 256.0f / 2) * 5.0f;
	float height = ((float)WindowHalfHeight -22.0f * 5);

	ABMBases[0]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (124.0f - 256.0f / 2) * 5.0f;

	ABMBases[1]->SpawnMissileBase({ positionX, height, 0.0f });

	positionX = (240.0f - 256.0f / 2) * 5.0f;

	ABMBases[2]->SpawnMissileBase({ positionX, height, 0.0f });

	return false;
}

bool TheABMBaseManager::BeginRun()
{
	Common::BeginRun();

	return false;
}

void TheABMBaseManager::SetMissileModel(Model& model)
{
	for (int i = 0; i < 3; i++)
	{
		ABMBases[i]->SetMissileModel(model);
	}
}

void TheABMBaseManager::Update()
{
	Common::Update();

	if (!OutOfAmmo)
	{
		OutOfAmmo = true;

		for (const auto &base : ABMBases)
		{
			if (!base->OutOfAmmo) OutOfAmmo = false;
		}
	}
}

void TheABMBaseManager::Reset(Color color)
{
	for (const auto &base : ABMBases)
	{
		base->Reset(color);
	}

	OutOfAmmo = false;
}

bool TheABMBaseManager::MissileFired(size_t baseNumber)
{
	return (ABMBases[baseNumber]->MissileFired());
}