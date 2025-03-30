#include "Colors.h"

Colors::Colors()
{
}

Colors::~Colors()
{
}

Color Colors::ChangeColor()
{
	Color modelColor = { 0, 0, 0, 255 };
	CurrentColorNumber++;

	if (CurrentColorNumber > 8) CurrentColorNumber = 0;

	switch (CurrentColorNumber)
	{
	case 0:
		modelColor = BLACK;
		break;
	case 1:
		modelColor = WHITE;
		break;
	case 2:
		modelColor = Yellow;
		break;
	case 3:
		modelColor = Magenta;
		break;
	case 4:
		modelColor = Red;
		break;
	case 5:
		modelColor = Aqua;
		break;
	case 6:
		modelColor = LightGreen;
		break;
	case 7:
		modelColor = Blue;
		break;

	default:
		break;
	}

	return modelColor;
}