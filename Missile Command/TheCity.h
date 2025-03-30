#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

enum CityState
{
	Active,
	Destroyed,
	Open
};

class TheCity : public Model3D
{
public:
	TheCity();
	virtual ~TheCity();

	CityState State = Active;

	bool Initialize();
	bool BeginRun();

	void SetCityModels(Model& innerModel, Model& outerModel);

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position);
	void Destroy();

private:
	Model3D* InnerModel = nullptr;

};
