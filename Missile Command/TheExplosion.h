#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheExplosion : public Model3D
{
public:
	TheExplosion();
	virtual ~TheExplosion();

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	bool Start = false;

	size_t CurrentColorTimerID = 0;
	size_t ChangeSizeTimerID = 0;

	Colors GameColors;

	void ChangeColor();
	void ChangeSize();
};
