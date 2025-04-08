#include "TheCity.h"

TheCity::TheCity()
{
	EM.AddModel3D(InnerModel = DBG_NEW Model3D());
}

TheCity::~TheCity()
{
}

bool TheCity::Initialize()
{
	Model3D::Initialize();

	InnerModel->SetParent(*this);

	Scale = 1.2f;
	Stationary = true;

	return false;
}

bool TheCity::BeginRun()
{
	Model3D::BeginRun();

	Radius = InnerModel->Radius * 0.33f;
	InnerModel->NoCollision = true;

	return false;
}

void TheCity::SetCityModels(Model& innerModel, Model& outerModel)
{
	SetModel(outerModel);
	InnerModel->SetModel(innerModel);
}

void TheCity::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void TheCity::Draw3D()
{
	Model3D::Draw3D();
}

void TheCity::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	InnerModel->Spawn(position);
}

void TheCity::Return()
{
	Enabled = true;
	InnerModel->Enabled = true;
}

void TheCity::Destroy()
{
	Entity::Destroy();

	InnerModel->Destroy();
}

void TheCity::SetColor(Color main, Color inner)
{
	ModelColor = main;
	InnerModel->ModelColor = inner;
}
