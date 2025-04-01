#include "TheExplosion.h"

TheExplosion::TheExplosion()
{
	CurrentColorTimerID = EM.AddTimer();
	ChangeSizeTimerID = EM.AddTimer();
}

TheExplosion::~TheExplosion()
{
}

bool TheExplosion::Initialize()
{
	Model3D::Initialize();

	EM.SetTimer(CurrentColorTimerID, 0.05f);
	EM.SetTimer(ChangeSizeTimerID, 0.17f);

	return false;
}

bool TheExplosion::BeginRun()
{
	Model3D::BeginRun();

	Stationary = true;

	return false;
}

void TheExplosion::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (EM.TimerElapsed(CurrentColorTimerID)) ChangeColor();

	if (EM.TimerElapsed(ChangeSizeTimerID)) ChangeSize();
}

void TheExplosion::Draw3D()
{
	Model3D::Draw3D();
}

void TheExplosion::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	ModelColor = WHITE;
	Scale = 0.1f;
	Start = true;
}

void TheExplosion::Destroy()
{
	Entity::Destroy();

}

void TheExplosion::ChangeColor()
{
	EM.ResetTimer(CurrentColorTimerID);

	ModelColor = GameColors.ChangeColor();
}

void TheExplosion::ChangeSize()
{
	EM.ResetTimer(ChangeSizeTimerID);

	if (Start)
	{
		Scale += 0.075f;

		if (Scale > 1.0f) Start = false;
	}
	else
	{
		Scale -= 0.1f;

		if (Scale < 0.1f) Enabled = false;
	}
}
