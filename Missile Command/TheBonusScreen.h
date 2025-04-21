#pragma once
#include "Globals.h"
#include "OnScreenText.h"
#include "Colors.h"

class TheBonusScreen : public OnScreenText
{
public:
	TheBonusScreen();
	virtual ~TheBonusScreen();

	bool Initialize();
	bool BeginRun();

	void Update();
	void Draw2D();

	void SetColors(Color mainColor, Color secondaryColor);
	void SetBonusCities(unsigned cityBonus);
	void SetBonusAmmo(unsigned ammoBonus);
	void ShowBonusPoints();
	void HideBonusPoints();
	void ShowScoreMultiplier(unsigned multiplier);
	void HideScoreMultiplier();
	void ShowAtariLogo();
	void HideAtariLogo();
	void ShowBonusCityText();
	void HideBonusCityText();
	void ClearPoints();
	Color GetTextColor();

private:
	bool BonusPoints = false;
	bool ScoreMultiplier = false;
	bool AtariLogo = false;
	bool BonusCityText = false;

	unsigned CityBonus = 0;
	unsigned AmmoBonus = 0;
	unsigned Multiplier = 0;

	Color MainColor = {};
	Color SecondaryColor = {};

};

