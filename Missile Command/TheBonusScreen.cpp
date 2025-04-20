#include "TheBonusScreen.h"

TheBonusScreen::TheBonusScreen()
{

}

TheBonusScreen::~TheBonusScreen()
{
}

void TheBonusScreen::SetColors(Color mainColor, Color secondaryColor)
{
	MainColor = mainColor;
	SecondaryColor = secondaryColor;
}

void TheBonusScreen::SetBonusCities(unsigned cityBonus)
{
	CityBonus = cityBonus;
}

void TheBonusScreen::SetBonusAmmo(unsigned ammoBonus)
{
	AmmoBonus = ammoBonus;
}

void TheBonusScreen::ShowBonusPoints()
{
	BonusPoints = true;
}

void TheBonusScreen::HideBonusPoints()
{
	BonusPoints = false;
}

void TheBonusScreen::ShowScoreMultiplier()
{
	ScoreMultiplier = true;
}

void TheBonusScreen::HideScoreMultiplier(unsigned multiplier)
{
	Multiplier = multiplier;
	ScoreMultiplier = false;
}

void TheBonusScreen::ShowAtariLogo()
{
	AtariLogo = true;
}

void TheBonusScreen::HideAtariLogo()
{
	AtariLogo = false;
}

void TheBonusScreen::ClearPoints()
{
	CityBonus = 0;
	AmmoBonus = 0;
	Multiplier = 1;
	Enabled = false;
}

Color TheBonusScreen::GetTextColor()
{
	return SecondaryColor;
}

bool TheBonusScreen::Initialize()
{
	OnScreenText::Initialize();

	return false;
}

bool TheBonusScreen::BeginRun()
{
	MainColor = Blue;
	SecondaryColor = Red;

	return false;
}

void TheBonusScreen::Update()
{
	OnScreenText::Update();

}

void TheBonusScreen::Draw2D()
{
	OnScreenText::Draw2D();

	if (BonusPoints)
	{
		DrawText("BONUS POINTS", WindowHalfWidth - 200,
			WindowHalfHeight - 200, 45, MainColor);

		std::string ammoPoints = std::to_string(AmmoBonus);
		std::string cityPoints = std::to_string(CityBonus);

		if (AmmoBonus > 0)
		{
			DrawText(ammoPoints.c_str(), WindowHalfWidth - 150,
				WindowHalfHeight, 45, SecondaryColor);
		}

		if (CityBonus > 0)
		{
			DrawText(cityPoints.c_str(), WindowHalfWidth - 200,
				WindowHalfHeight + 115, 45, SecondaryColor);
		}
	}
	else if (ScoreMultiplier)
	{
		std::string multiplier = std::to_string(Multiplier);

		DrawText("PLAYER", WindowHalfWidth / 2,
			WindowHalfHeight - 200, 45, MainColor);
		DrawText("X POINTS", WindowHalfWidth / 2,
			WindowHalfHeight, 45, MainColor);

		DrawText("1", WindowHalfWidth + 100,
			WindowHalfHeight - 200, 45, SecondaryColor);
		DrawText(multiplier.c_str(), WindowHalfWidth / 2 - 300,
			WindowHalfHeight, 45, SecondaryColor);

		if (AtariLogo)
		{
			DrawText("ATARI   1980", WindowHalfWidth / 2,
				WindowFullHeight, 45, MainColor);
		}
	}
}
