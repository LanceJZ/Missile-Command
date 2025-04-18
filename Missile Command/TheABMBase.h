#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheABMBase : public Common
{
public:
	TheABMBase();
	virtual ~TheABMBase();

	bool OutOfAmmo = false;
	float Radius = 10.0f;
	Vector3 Position = { 0, 0, 0 };

	bool Initialize();
	bool BeginRun();

	void SetMissileModel(Model& model);
	void SpawnMissileBase(Vector3 position);

	void Update();

	void Reset(Color &color);
	void Clear();

	int GetMissileCount();
	bool MissileFired();
	bool MissileCounted(size_t count, size_t ammoCounted);

private:
	size_t ABMCounted = 0;

	Model3D* ABMAmmo[10];
	Model3D* AmmoCount[10];

};