#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class Shot : public Model3D
{
public:
	Shot();
	virtual ~Shot();

	bool ByFlier = false;
	size_t TargetIndex = 0;

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3& position, Vector3& target, Vector3& velocity, Color color);
	void Destroy();

	bool CheckHitTarget();

private:
	size_t CurrentColorTimerID = 0;

	Vector3 StartPosition = {};
	Vector3 TargetPosition = {};

	Colors GameColors;
	Color TrailColor = {};

	void ChangeColor();
};
