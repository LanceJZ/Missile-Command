#pragma once
#include "Globals.h"
#include "OnScreenText.h"
#include "Colors.h"

class TheAmmoScreen : public OnScreenText
{
public:
	TheAmmoScreen();
	virtual ~TheAmmoScreen();

	bool Initialize();
	bool BeginRun();

	void Update();
	void Draw2D();

	void SetColor(Color main);
	void AmmoIsLow(size_t base);
	void AmmoIsOut(size_t base);
	void Reset();

private:
	bool AmmoLow[3] = { false };
	bool AmmoOut[3] = { false };

	Color MainColor = Blue;
};

