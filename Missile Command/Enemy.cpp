#include "Enemy.h"

Enemy::Enemy()
{
	ShotTimerID = EM.AddTimer();
}

Enemy::~Enemy()
{
}

void Enemy::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void Enemy::SetShotModel(LineModelPoints model)
{
	ShotModel = model;
}

void Enemy::SetFireSound(Sound fireSound)
{
	FireSound = fireSound;
}

void Enemy::SetExplodeSound(Sound explodeSound)
{
	ExplodeSound = explodeSound;
}

bool Enemy::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool Enemy::BeginRun()
{
	LineModel::BeginRun();


	return false;
}

void Enemy::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (!Player->GameOver && !IsSoundPlaying(OnSound)) PlaySound(OnSound);
}

void Enemy::Draw3D()
{
	LineModel::Draw3D();
}

void Enemy::CheckShotsHitPlayer()
{
	for (const auto shot : Shots)
	{
		if (!shot->Enabled) continue;

		if (shot->CirclesIntersect(*Player))
		{
			break;
		}
	}
}

void Enemy::Spawn()
{
	if (!Player->GameOver) PlaySound(SpawnSound);

	Vector3 position = { 0.0f, 0.0f, 0.0f };


	Spawn(position);
}

void Enemy::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	EM.ResetTimer(ShotTimerID, M.GetRandomFloat(0.75f, 1.5f));
}

void Enemy::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);

	Destroy();
}

void Enemy::Reset()
{
	for (auto& shot : Shots)
	{
		shot->Destroy();
	}

	Velocity = { 0.0f, 0.0f, 0.0f };
	RotationVelocityZ = 0.0f;

	Destroy();
}

void Enemy::Destroy()
{
	Entity::Destroy();

}

void Enemy::Shoot()
{
	PlaySound(FireSound);

	EM.ResetTimer(ShotTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

	Shots[spawnNumber]->Spawn(Position);
}

void Enemy::Shoot(Vector3 velocity)
{
	EM.ResetTimer(ShotTimerID);

	bool spawnNew = true;
	size_t spawnNumber = Shots.size();

	for (size_t check = 0; check < spawnNumber; check++)
	{
		if (!Shots[check]->Enabled)
		{
			spawnNew = false;
			spawnNumber = check;
			break;
		}
	}

	if (spawnNew)
	{
		Shots.push_back(DBG_NEW Shot());
		EM.AddLineModel(Shots[spawnNumber], ShotModel);
		Shots[spawnNumber]->BeginRun();
	}

}

void Enemy::ChasePlayer()
{
	RotationVelocityZ = 0.0f;

	Vector3 target = Player->Position;

	if (!Player->Enabled)
	{
		if (X() > 0.0f) target.x = (float)WindowFullWidth;
		else target.x = (float)-WindowFullWidth;

		if (Y() > 0.0f) target.y = (float)WindowFullHeight;
		else target.y = (float)-WindowFullHeight;
	}

	M.SetRotateVelocity(target, TurnSpeed, Speed);
}

void Enemy::Explode()
{
	Particles.SpawnLineDots(Position,
		Vector3Multiply(Velocity, {0.25f}),
		20, 100, 20, 2.0f, WHITE);

	Destroy();

}

bool Enemy::CheckCollisions()
{
	for (const auto& shot : Player->ABMs)
	{
		if (shot->CirclesIntersect(*this))
		{
			shot->Destroy();
			Hit();

			if (Player->GameOver) return true;

			Player->ScoreUpdate(Points);

			return true;
		}
	}

	if (!Player->GetBeenHit())
	{
		if (CirclesIntersect(*Player))
		{

			Hit();
			Player->ScoreUpdate(Points);

			Player->Hit(Position, Velocity);

			return true;
		}
	}

	return false;
}