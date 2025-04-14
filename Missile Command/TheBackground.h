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

	void Clear();
	void WaveColor(Color groundColor, Color backgroundColor);

private:
	Model3D* Ground = nullptr;
	Color BackGroundColor = {};

};

