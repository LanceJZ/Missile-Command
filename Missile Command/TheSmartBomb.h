#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"
#include "TheExplosion.h"

class TheSmartBomb : public Model3D
{
public:
	TheSmartBomb();
	virtual ~TheSmartBomb();

	bool OrderNineIsEngaged = false;

	void SetEdgeModel(Model &edge);
	void SetTargetRefs(std::vector<Model3D*> targets);
	void SetExplosionRefs(std::vector<TheExplosion*> explosions);
	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void NextWave(Color color, Color edgeColor);
	void Spawn(Vector3 position, Vector3 target, float speed);
	void Destroy();

private:
	enum Mode
	{
		Go,
		Evade
	};

	size_t ColorChangeTimerID = 0;

	float RadarRadius = 0.0f;
	float Speed = 0.0f;

	Vector3 TargetPosition = {0.0f, 0.0f, 0.0f};
	Vector3 EvadeTargetPosition = {0.0f, 0.0f, 0.0f};

	Mode CurrentMode = Go;

	Colors GameColors;

	Model3D* EdgeModel = nullptr;
	std::vector<Model3D*> TargetRefs = {};
	std::vector<TheExplosion*> ExplosionRefs = {};

	void GoTime();
	void EvadeTime();
	bool CheckForEvade();
	bool CheckGoodForGo();
};
