#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = EM.AddTimer();
	CurrentColorTimerID = EM.AddTimer();
}

Shot::~Shot()
{
}

bool Shot::Initialize()
{
	Model3D::Initialize();

	EM.SetTimer(CurrentColorTimerID, 0.05f);

	return false;
}

bool Shot::BeginRun()
{
	Enabled = false;

	return false;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	//if (EM.TimerElapsed(LifeTimerID)) Destroy();

	if (EM.TimerElapsed(CurrentColorTimerID)) ChangeColor();
}

void Shot::Draw3D()
{
	Model3D::Draw3D();
}

void Shot::Spawn(Vector3 position)
{
	Model3D::Spawn(position);
	EM.ResetTimer(CurrentColorTimerID);
}

void Shot::Spawn(Vector3 position, Vector3 velocity)
{
	Spawn(position);
	Velocity = velocity;
}

void Shot::Spawn(Vector3 position, Vector3 velocity, float lifetime)
{
	SetLifeTimer(lifetime);
	Spawn(position, velocity);
}

void Shot::SetLifeTimer(float lifetime)
{
	EM.ResetTimer(LifeTimerID, lifetime);
}

void Shot::Destroy()
{
	Entity::Destroy();
}

bool Shot::CheckHitTarget()
{
	bool hit = CirclesIntersect(TargetPosition, 5.0f);

	if (hit) Destroy();

	return hit;
}

void Shot::ChangeColor()
{
	EM.ResetTimer(CurrentColorTimerID);
	ModelColor = GameColors.ChangeColor();
}