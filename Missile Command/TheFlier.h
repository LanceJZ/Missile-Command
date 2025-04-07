#pragma once
#include "Globals.h"
#include "Model3D.h"
#include "Colors.h"

class TheFlier : public Model3D
{
public:
	TheFlier();
	virtual ~TheFlier();

	bool Satelite = false;
	Color InnerColor = Blue;

	void SetSateliteInnnerModel(Model model);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(float speed);
	void NextWave(unsigned wave);
	void Destroy();

private:
	struct FlierHeightRangeData
	{
		int	Minimum = 0;
		int Maximum = 0;
	};

	unsigned Wave = 0;

	FlierHeightRangeData FlierHeightRange[3] = {};

	FlierHeightRangeData CurrentHeightRange = {};

	Model3D* SateliteInside = nullptr;
};
