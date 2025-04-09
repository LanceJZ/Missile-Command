#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	EM.AddCommon(ICBMControl = DBG_NEW TheICBMManager());
	EM.AddModel3D(Flier = DBG_NEW TheFlier());
	FlierLaunchTimerID = EM.AddTimer();

	for (int i = 0; i < 3; i++)
	{
		EM.AddModel3D(SmartBombs[i] = DBG_NEW TheSmartBomb());
	}
}


EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void EnemyControl::SetICBMModel(Model& missileModel)
{
	ICBMControl->SetMissileModels(missileModel);
}

void EnemyControl::SetBomberModel(Model& bomberModel)
{
	BomberModel = bomberModel;
}

void EnemyControl::SetSateliteModel(Model& sateliteModel, Model& sateliteInnerModel)
{
	SateliteModel = sateliteModel;
	Flier->SetSateliteInnnerModel(sateliteInnerModel);
}

void EnemyControl::SetSmartBombModel(Model& smartBombMainModel,
	Model& smartBombEdgeModel)
{
	for (int i = 0; i < 7; i++)
	{
		SmartBombs[i]->SetModel(smartBombMainModel);
		SmartBombs[i]->SetEdgeModel(smartBombEdgeModel);
	}
}

bool EnemyControl::Initialize()
{
	Common::Initialize();

	Flier->ModelColor = Red;

	ICBMControl->SetBomberReference(Flier);

	FlierCooldown[0] = 240.0f/60.0f;
	FlierCooldown[1] = 160.0f/60.0f;
	FlierCooldown[2] = 128.0f/60.0f;
	FlierCooldown[3] = FlierCooldown[2];
	FlierCooldown[4] = 96.0f/60.0f;
	FlierCooldown[5] = 64.0f/60.0f;
	FlierCooldown[6] = 32.0f/60.0f;

	EM.SetTimer(FlierLaunchTimerID, FlierCooldown[0]);

	return false;
}

bool EnemyControl::BeginRun()
{
	Common::BeginRun();

	Reset();

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

	if (Wave > 0)
	{
		if (Flier->Enabled) EM.ResetTimer(FlierLaunchTimerID);
		else if (EM.TimerElapsed(FlierLaunchTimerID) && !ICBMControl->OutOfMissiles)
		{
			EM.ResetTimer(FlierLaunchTimerID);
			ICBMControl->ResetFlierFireTimer();
			SpawnFlier();
		}
	}
}

void EnemyControl::ResetLaunchTimer()
{
	EM.ResetTimer(FlierLaunchTimerID);
}

void EnemyControl::NextWave(unsigned wave, Color icbmColor,
	Color secondColor, Color edgeColor)
{
	Wave = wave;
	Flier->NextWave(Wave, icbmColor, secondColor);
	ICBMControl->NewWave(icbmColor);

	for (const auto &smartBomb : SmartBombs)
		smartBomb->NextWave(icbmColor, edgeColor);

	if (Wave < 8) EM.SetTimer(FlierLaunchTimerID, FlierCooldown[Wave - 1]);
}

void EnemyControl::NewGame()
{
	Wave = 0;

	ICBMControl->NewGame();
}

void EnemyControl::Reset()
{
	ICBMControl->Reset();
}

void EnemyControl::SpawnFlier()
{
	float speed = 0;

	if (GetRandomValue(0, 1) == 0)
	{
		speed = (60.0f / 3.0f) * 1.5f;
		Flier->SetModel(BomberModel);
		Flier->Satelite = false;
	}
	else
	{
		speed = (60.0f / 2.0f) * 1.5f;
		Flier->SetModel(SateliteModel);
		Flier->Satelite = true;
	}

	Flier->Spawn(speed);
}
