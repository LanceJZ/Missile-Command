#include "TheMissile.h"

TheMissile::TheMissile()
{
}

TheMissile::~TheMissile()
{
}

bool TheMissile::Initialize()
{
	Model3D::Initialize();

	return false;
}

bool TheMissile::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void TheMissile::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void TheMissile::Draw3D()
{
	Model3D::Draw3D();
}

void TheMissile::Spawn(Vector3 position)
{
	Model3D::Spawn(position);

}

void TheMissile::Destroy()
{
	Entity::Destroy();

}
