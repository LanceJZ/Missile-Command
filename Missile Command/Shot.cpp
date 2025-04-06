#include "Shot.h"

Shot::Shot()
{
	CurrentColorTimerID = EM.AddTimer();
}

Shot::~Shot()
{
}

bool Shot::Initialize()
{
	Model3D::Initialize();

	Enabled = false;
	Cull = false;

	EM.SetTimer(CurrentColorTimerID, 0.05f);

	return false;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	Radius = 2;
	Scale = 2.0f;

	return false;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (EM.TimerElapsed(CurrentColorTimerID)) ChangeColor();
}

void Shot::Draw3D()
{
	Model3D::Draw3D();
}

void Shot::Spawn(Vector3& position)
{
	Model3D::Spawn(position);

	EM.ResetTimer(CurrentColorTimerID);
}

void Shot::Spawn(Vector3& position, Vector3& velocity)
{
	Spawn(position);
	Velocity = velocity;
}

void Shot::Spawn(Vector3& position, Vector3& velocity, Color color)
{
	Spawn(position, velocity);
	TrailColor = color;
}

void Shot::Destroy()
{
	Entity::Destroy();
}

void Shot::PlayerHit()
{
	Score.AddToScore(25);
	Destroy();
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