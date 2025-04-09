#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheSmartBomb : public Model3D
{
public:
	TheSmartBomb();
	virtual ~TheSmartBomb();

	void SetEdgeModel(Model &edge);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void NextWave(Color color, Color edgeColor);
	void Spawn();
	void Destroy();

private:
	enum Mode
	{
		Go,
		Evade
	};

	size_t ColorChangeTimerID = 0;
	float RadarRadius = 0.0f;
	Mode CurrentMode = Go;

	Colors GameColors;
	Model3D* EdgeModel = nullptr;

	void Spawn(Vector3 position);

};
