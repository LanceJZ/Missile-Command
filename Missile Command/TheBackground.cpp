#include "TheBackground.h"
#include "Colors.h"

TheBackground::TheBackground()
{
	EM.AddModel3D(Ground = DBG_NEW Model3D());
}

TheBackground::~TheBackground()
{
}

bool TheBackground::Initialize()
{
	Common::Initialize();

	Ground->Y(WindowHalfHeight);
	Ground->ModelColor = YELLOW;
	Ground->Z(100.0f);
	Ground->Scale = 5.0f;
	Ground->Stationary = true;

	return false;
}

bool TheBackground::BeginRun()
{
	Common::BeginRun();

	Ground->Radius = 0;
	return false;
}

void TheBackground::SetGroundModel(Model& model)
{
	Ground->SetModel(model);
}

void TheBackground::Update()
{
	Common::Update();

}