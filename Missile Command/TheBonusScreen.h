#pragma once
#include "Globals.h"
#include "OnScreenText.h"

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
	void ShowScoreMultiplier();
	void HideScoreMultiplier(unsigned multiplier);
	void ShowAtariLogo();
	void HideAtariLogo();
	void ClearPoints();
private:
	bool BonusPoints = false;
	bool ScoreMultiplier = false;
	bool AtariLogo = false;

	unsigned CityBonus = 0;
	unsigned AmmoBonus = 0;
	unsigned Multiplier = 0;

	Color MainColor = {};
	Color SecondaryColor = {};

};

