#include "TheABMBase.h"

TheABMBase::TheABMBase()
{
	for (int i = 0; i < 10; i++)
	{
		EM.AddModel3D(ABMIcons[i] = DBG_NEW Model3D());
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

	return false;
}

void TheABMBase::SetMissileModel(Model& model)
{
	for (int i = 0; i < 10; i++)
	{
		ABMIcons[i]->SetModel(model);
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

			ABMIcons[index]->Spawn({ positionX, positionY, 0.0f });
			ABMIcons[index]->ModelColor = Blue;
			ABMIcons[index]->Scale = 1.75f;
			ABMIcons[index]->Cull = false;
			ABMIcons[index]->Stationary = true;
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
	for (auto missile : ABMIcons)
	{
		missile->Enabled = true;
	}
}

void TheABMBase::Clear()
{
	for (auto missile : ABMIcons)
	{
		missile->Enabled = false;
	}
}

bool TheABMBase::MissileFired()
{
	size_t lastInStack = 0;

	for (auto missile : ABMIcons)
	{
		if (missile->Enabled) lastInStack++;

		if (lastInStack < 1) return false;
	}

	ABMIcons[lastInStack - 1]->Enabled = false;

	return true;
}
