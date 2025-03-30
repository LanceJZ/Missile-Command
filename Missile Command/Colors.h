#pragma once
#include "Globals.h"

#define Yellow Color{253, 254, 0, 255}
#define Magenta Color{255, 0, 252, 255}
#define Red Color{255, 0, 0, 255}
#define Aqua Color{0, 254, 255, 255}
#define LightGreen Color{0, 255, 0, 255}
#define Blue Color{4, 3, 250, 255}

class Colors
{
public:
	Colors();
	~Colors();

	int CurrentColorNumber = 0;

	Color ChangeColor();
};