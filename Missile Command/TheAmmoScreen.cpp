#include "TheAmmoScreen.h"

TheAmmoScreen::TheAmmoScreen()
{

}

TheAmmoScreen::~TheAmmoScreen()
{
}

bool TheAmmoScreen::Initialize()
{
	OnScreenText::Initialize();

	return false;
}

bool TheAmmoScreen::BeginRun()
{

	return false;
}

void TheAmmoScreen::Update()
{
	OnScreenText::Update();

}

void TheAmmoScreen::Draw2D()
{
	OnScreenText::Draw2D();

	if (AmmoLow[0])
	{
		DrawText("LOW", 50,
			WindowFullHeight - 45, 45, MainColor);
	}
	else if (AmmoOut[0])
	{
		DrawText("OUT", 50,
			WindowFullHeight - 45, 45, MainColor);
	}

	if (AmmoLow[1])
	{
		DrawText("LOW", WindowHalfWidth - (int)(45 * 1.5f),
			WindowFullHeight - 45, 45, MainColor);
	}
	else if (AmmoOut[1])
	{
		DrawText("OUT", WindowHalfWidth - (int)(45 * 1.5f),
			WindowFullHeight - 45, 45, MainColor);
	}

	if (AmmoLow[2])
	{
		DrawText("LOW", WindowFullWidth - 115,
			WindowFullHeight - 45, 45, MainColor);
	}
	else if (AmmoOut[2])
	{
		DrawText("OUT", WindowFullWidth - 115,
			WindowFullHeight - 45, 45, MainColor);
	}
}

void TheAmmoScreen::SetColor(Color main)
{
	MainColor = main;
}

void TheAmmoScreen::AmmoIsLow(size_t base)
{
	AmmoLow[base] = true;
}

void TheAmmoScreen::AmmoIsOut(size_t base)
{
	AmmoLow[base] = false;
	AmmoOut[base] = true;
}

void TheAmmoScreen::Reset()
{
	for (size_t i = 0; i < 3; i++)
	{
		AmmoLow[i] = false;
		AmmoOut[i] = false;
	}
}