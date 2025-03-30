#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheMissileBase : public Common
{
public:
	TheMissileBase();
	virtual ~TheMissileBase();

	Vector3 Position = { 0, 0, 0 };
	Model3D* MissileIcons[10];

	bool Initialize();
	bool BeginRun();

	void SetMissileModel(Model& model);
	void SpawnMissileBase(Vector3 position);

	void Update();

	void Reset();
	void Clear();

	bool MissileFired();

private:


};
