#include "TheSmartBomb.h"

TheSmartBomb::TheSmartBomb()
{
	ColorChangeTimerID = EM.AddTimer();
	EM.AddModel3D(EdgeModel = DBG_NEW Model3D());
}

TheSmartBomb::~TheSmartBomb()
{
}

void TheSmartBomb::SetEdgeModel(Model &edge)
{
	EdgeModel->SetModel(edge);
}

void TheSmartBomb::SetTargetRefs(std::vector<Model3D*> &targets)
{
	TargetRefs = targets;
}

void TheSmartBomb::SetExplosionRefs(std::vector<TheExplosion*> &explosions)
{
	ExplosionRefs = explosions;
}

bool TheSmartBomb::Initialize()
{
	Model3D::Initialize();

	EM.SetTimer(ColorChangeTimerID, 0.05f);

	return false;
}

bool TheSmartBomb::BeginRun()
{
	Model3D::BeginRun();

	EdgeModel->SetParent(*this);
	RadarRadius = Radius * 1.5f;

	Destroy();

	return false;
}

void TheSmartBomb::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (EM.TimerElapsed(ColorChangeTimerID))
	{
		EM.ResetTimer(ColorChangeTimerID);
		ModelColor = GameColors.ChangeColor();
	}

	switch (CurrentMode)
	{
	case Go:
		GoTime();
		break;
	case Evade:
		EvadeTime();
		break;
	}
}

void TheSmartBomb::Draw3D()
{
	Model3D::Draw3D();
}

void TheSmartBomb::NextWave(Color color, Color edgeColor)
{
	ModelColor = color;
	EdgeModel->ModelColor = edgeColor;
}

void TheSmartBomb::Spawn(Vector3 position, Vector3 target, float speed)
{
	Entity::Spawn(position);

	TargetPosition = target;
	Speed = speed;
	CurrentMode = Go;
}

void TheSmartBomb::Destroy()
{
	Entity::Destroy();

	EdgeModel->Destroy();
}

void TheSmartBomb::GoTime()
{
	Velocity = GetVelocityFromVectorZ(TargetPosition, Speed);

	if (CheckForEvade()) CurrentMode = Evade;
}

void TheSmartBomb::EvadeTime()
{
	Velocity = M.GetVelocityFromVectorsZ(EvadeTargetPosition, Position, Speed);

	if (CheckGoodForGo()) CurrentMode = Go;
}

bool TheSmartBomb::CheckForEvade()
{
	for (const auto& target : TargetRefs)
	{
		if (target->Enabled)
		{
			if (CheckCollisionSpheres(target->Position, target->Radius,
				Position, RadarRadius))
			{
				EvadeTargetPosition = target->Position;
				return true;
			}
		}
	}

	for (const auto& explosion : ExplosionRefs)
	{
		if (explosion->Enabled)
		{
			if (CheckCollisionSpheres(explosion->Position, explosion->Radius,
				Position, RadarRadius))
			{
				EvadeTargetPosition = explosion->Position;
				return true;
			}
		}
	}

	return false;
}

bool TheSmartBomb::CheckGoodForGo()
{
	return !CheckForEvade();
}
