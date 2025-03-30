#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class Shot : public Model3D
{
public:
	Shot();
	virtual ~Shot();

	int HitPoints = 0;
	Vector3 TargetPosition = {};
	size_t TargetIndex = 0;

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Spawn(Vector3 position, Vector3 velocity);
	void Spawn(Vector3 position, Vector3 velocity, float lifetime);
	void SetLifeTimer(float lifetime);
	void Destroy();

private:
	size_t LifeTimerID = 0;
	size_t CurrentColorTimerID = 0;

	Colors GameColors;

	void ChangeColor();
};
