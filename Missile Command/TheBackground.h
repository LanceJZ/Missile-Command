#pragma once
#include "Globals.h"
#include "Model3D.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	bool Initialize();
	bool BeginRun();

	void SetGroundModel(Model& model);

	void Update();

	void WaveColor(Color waveColor);

private:
	Model3D* Ground = nullptr;

};

