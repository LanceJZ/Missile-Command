#pragma once
#include "Globals.h"
#include "OnScreenText.h"
#include "Colors.h"

class GameOverScreen : public OnScreenText
{
public:
	GameOverScreen();
	virtual ~GameOverScreen();

	Color BottomTextColor = Color {};
	Color TopTextColor = Color {};

	bool Initialize();
	bool BeginRun();

	void Update();
	void Draw2D();

private:


};

