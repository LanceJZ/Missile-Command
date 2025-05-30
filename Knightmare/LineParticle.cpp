#include "LineParticle.h"

LineParticle::LineParticle()
{
}

LineParticle::~LineParticle()
{
}

void LineParticle::SetEntityManager(EntityManager* entityManager)
{
	EM = entityManager;
}

bool LineParticle::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool LineParticle::BeginRun()
{
	LineModel::BeginRun();

	LifeTimerID = EM->AddTimer();

	return false;
}

void LineParticle::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (EM->TimerElapsed(LifeTimerID))
	{
		Enabled = false;
	}
}

void LineParticle::Draw3D()
{
	LineModel::Draw3D();
}

void LineParticle::Spawn(Vector3 position, Vector3 velocity, float radius,
	float maxSpeed, float maxTime)
{
	Vector3 spawnPosition = position;

	spawnPosition.x += M.GetRandomFloat(-radius, radius);
	spawnPosition.y += M.GetRandomFloat(-radius, radius);

	LineModel::Spawn(spawnPosition);

	Vector3 addedVelocity = M.GetRandomVelocity(M.GetRandomFloat(maxSpeed * 0.25f,
		maxSpeed));
	Velocity = Vector3Add(velocity, addedVelocity);

	EM->ResetTimer(LifeTimerID, M.GetRandomFloat(maxTime * 0.25f, maxTime));
}

void LineParticle::Destroy()
{
	Entity::Destroy();

}