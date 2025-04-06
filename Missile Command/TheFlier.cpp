#include "TheFlier.h"

TheFlier::TheFlier()
{
	EM.AddModel3D(SateliteInside = DBG_NEW Model3D());
}

TheFlier::~TheFlier()
{
}

void TheFlier::SetSateliteInnnerModel(Model model)
{
	SateliteInside->SetModel(model);
}

bool TheFlier::Initialize()
{
	Model3D::Initialize();

	Enabled = false;
	SateliteInside->Enabled = false;
	SateliteInside->SetParent(*this);

	FlierHeightRange[0].Minimum = -(int(148 * 4.15f) - WindowHalfHeight);
	FlierHeightRange[0].Maximum = -(int(195 * 4.15f) - WindowHalfHeight);
	FlierHeightRange[1].Minimum = -(int(132 * 4.15f) - WindowHalfHeight);
	FlierHeightRange[1].Maximum = -(int(163 * 4.15f) - WindowHalfHeight);
	FlierHeightRange[2].Minimum = -(int(100 * 4.15f) - WindowHalfHeight);
	FlierHeightRange[2].Maximum = -(int(131 * 4.15f) - WindowHalfHeight);

	return false;
}

bool TheFlier::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void TheFlier::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void TheFlier::Draw3D()
{
	Model3D::Draw3D();
}

void TheFlier::Spawn(float speed)
{
	Entity::Spawn();

	SateliteInside->Enabled = Satelite;

	if (GetRandomValue(0, 1) == 0)
	{
		X((float)-WindowHalfWidth);
		Velocity.x = speed;
		RotationY = 0.0f;
	}
	else
	{
		X((float)WindowHalfWidth);
		Velocity.x = -speed;
		RotationY = PI;
	}

	if (CurrentHeightRange.Minimum < CurrentHeightRange.Maximum)
		Position.y = (M.GetRandomFloat((float)CurrentHeightRange.Minimum,
			(float)CurrentHeightRange.Maximum));
	else
		Position.y = (M.GetRandomFloat((float)CurrentHeightRange.Maximum,
			(float)CurrentHeightRange.Minimum));

}

void TheFlier::NextWave(unsigned wave)
{
	Wave = wave;

	if (wave < 6)
	{
		int range = ((int)wave + 1) / 2;
		CurrentHeightRange = FlierHeightRange[range - 1];
	}

	Destroy();
}

void TheFlier::PlayerHit()
{
	Score.AddToScore(100);
	Destroy();
}

void TheFlier::Destroy()
{
	Entity::Destroy();

	SateliteInside->Destroy();
}
