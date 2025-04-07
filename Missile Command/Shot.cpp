#include "Shot.h"
#include "rlgl.h"

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

	Destroy();

	EM.SetTimer(CurrentColorTimerID, 0.05f);

	Scale = 2.0f;

	return false;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	Radius = 2;

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

	rlBegin(RL_LINES);
	rlColor4ub(TrailColor.r, TrailColor.g, TrailColor.b, TrailColor.a);
	rlVertex3f(StartPosition.x, StartPosition.y, StartPosition.z);
	rlVertex3f(Position.x, Position.y, Position.z);
	rlEnd();
}

void Shot::Spawn(Vector3& position, Vector3& target, Vector3& velocity, Color color)
{
	Model3D::Spawn(position, velocity);

	TargetPosition = target;
	StartPosition = position;
	
	TrailColor = color;

	EM.ResetTimer(CurrentColorTimerID);
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