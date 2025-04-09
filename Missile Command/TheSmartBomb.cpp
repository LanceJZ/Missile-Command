#include "TheSmartBomb.h"

TheSmartBomb::TheSmartBomb()
{
	ColorChangeTimerID = EM.AddTimer();
}

TheSmartBomb::~TheSmartBomb()
{
}

void TheSmartBomb::SetEdgeModel(Model &edge)
{
	EdgeModel->SetModel(edge);
}

bool TheSmartBomb::Initialize()
{
	Model3D::Initialize();

	EM.SetTimer(ColorChangeTimerID, 0.05f);
	RadarRadius = Radius * 1.5f;

	return false;
}

bool TheSmartBomb::BeginRun()
{
	Model3D::BeginRun();

	EdgeModel->SetParent(*this);

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

void TheSmartBomb::Spawn()
{

}

void TheSmartBomb::Destroy()
{
	Entity::Destroy();

}

void TheSmartBomb::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}