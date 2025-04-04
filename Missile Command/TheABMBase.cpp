#include "TheABMBase.h"

TheABMBase::TheABMBase()
{
	for (int i = 0; i < 10; i++)
	{
		EM.AddModel3D(ABMAmmo[i] = DBG_NEW Model3D());
	}
}

TheABMBase::~TheABMBase()
{
}

bool TheABMBase::Initialize()
{
	Common::Initialize();

	return false;
}

bool TheABMBase::BeginRun()
{
	Common::BeginRun();

	for (int i = 0; i < 10; i++)
	{
		ABMAmmo[i]->HideCollision = true;
		ABMAmmo[i]->Scale = 1.75f;
		ABMAmmo[i]->Cull = false;
		ABMAmmo[i]->Stationary = true;
		ABMAmmo[i]->ModelColor = Blue;
	}

	return false;
}

void TheABMBase::SetMissileModel(Model& model)
{
	for (int i = 0; i < 10; i++)
	{
		ABMAmmo[i]->SetModel(model);
	}
}

void TheABMBase::SpawnMissileBase(Vector3 position)
{
	Position = position;
	size_t index = 0;

	for (int line = 0; line < 4; line++)
	{
		for (int row = 0; row < line + 1; row++)
		{
			float spaceX = 22.0f;

			float positionX = position.x + (-spaceX * line) + (row * spaceX) +
				(line * (spaceX / 2));

			float positionY = position.y + 2.0f + (8.5f * line);

			ABMAmmo[index]->Spawn({ positionX, positionY, 0.0f });
			index++;
		}
	}
}

void TheABMBase::Update()
{
	Common::Update();

}

void TheABMBase::Reset()
{
	for (auto missile : ABMAmmo)
	{
		missile->Enabled = true;
	}
}

void TheABMBase::Clear()
{
	for (auto missile : ABMAmmo)
	{
		missile->Enabled = false;
	}
}

int TheABMBase::GetMissileCount()
{
	int count = 0;

	for (const auto &missile : ABMAmmo)
	{
		if (missile->Enabled) count++;
	}

	return count;
}

bool TheABMBase::MissileFired()
{
	int lastInStack = -1;

	for (auto missile : ABMAmmo)
	{
		if (missile->Enabled) lastInStack++;
	}

	if (lastInStack < 0) return false;

	ABMAmmo[lastInStack]->Enabled = false;

	return true;
}
