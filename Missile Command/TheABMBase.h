#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheABMBase : public Common
{
public:
	TheABMBase();
	virtual ~TheABMBase();

	Vector3 Position = { 0, 0, 0 };
	float Radius = 10.0f;
	Model3D* ABMIcons[10];

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