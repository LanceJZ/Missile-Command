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

private:
	Model3D* Ground = nullptr;

};

