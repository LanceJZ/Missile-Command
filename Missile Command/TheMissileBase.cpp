#include "TheMissileBase.h"

TheMissileBase::TheMissileBase()
{
	for (int i = 0; i < 10; i++)
	{
		EM.AddModel3D(MissileIcons[i] = DBG_NEW Model3D());
	}
}

TheMissileBase::~TheMissileBase()
{
}

bool TheMissileBase::Initialize()
{
	Common::Initialize();

	return false;
}

bool TheMissileBase::BeginRun()
{
	Common::BeginRun();

	return false;
}

void TheMissileBase::SetMissileModel(Model& model)
{
	for (int i = 0; i < 10; i++)
	{
		MissileIcons[i]->SetModel(model);
	}
}

void TheMissileBase::SpawnMissileBase(Vector3 position)
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

			MissileIcons[index]->Spawn({ positionX, positionY, 0.0f });
			MissileIcons[index]->ModelColor = Blue;
			MissileIcons[index]->Scale = 1.75f;
			MissileIcons[index]->Cull = false;
			MissileIcons[index]->Stationary = true;
			index++;
		}
	}
}

void TheMissileBase::Update()
{
	Common::Update();

}

void TheMissileBase::Reset()
{
	for (auto missile : MissileIcons)
	{
		missile->Enabled = true;
	}
}

void TheMissileBase::Clear()
{
	for (auto missile : MissileIcons)
	{
		missile->Enabled = false;
	}
}

bool TheMissileBase::MissileFired()
{
	size_t lastInStack = 0;

	for (auto missile : MissileIcons)
	{
		if (missile->Enabled) lastInStack++;

		if (lastInStack < 1) return false;
	}

	MissileIcons[lastInStack - 1]->Enabled = false;

	return true;
}
