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
	Ground->Z(100.0f);
	Ground->Scale = 5.0f;
	Ground->Stationary = true;

	return false;
}

bool TheBackground::BeginRun()
{
	Common::BeginRun();

	Ground->NoCollision = true;
	Ground->ModelColor = YELLOW;

	return false;
}

void TheBackground::SetGroundModel(Model& model)
{
	Ground->SetModel(model);
}

void TheBackground::Update()
{
	Common::Update();

	ClearBackground(BackGroundColor);
}

void TheBackground::Clear()
{
	ClearBackground(BLACK);
	Ground->Enabled = false;
}

void TheBackground::WaveColor(Color groundColor, Color backgroundColor)
{
	Ground->ModelColor = groundColor;
	BackGroundColor = backgroundColor;
	Ground->Enabled = true;

}