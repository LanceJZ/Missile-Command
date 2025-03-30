#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheMissile : public Model3D
{
public:
	TheMissile();
	virtual ~TheMissile();

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:


};
