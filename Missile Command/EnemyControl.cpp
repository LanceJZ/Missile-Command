#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	EM.AddCommon(ICBMControl = DBG_NEW TheICBMManager());
	EM.AddModel3D(Flier = DBG_NEW TheFlier());
	FlierLaunchTimerID = EM.AddTimer();
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
		else if (EM.TimerElapsed(FlierLaunchTimerID))
		{
			EM.ResetTimer(FlierLaunchTimerID);
			SpawnFlier();
		}
	}
}

void EnemyControl::NextWave()
{
	Wave++;
	Flier->NextWave(Wave);

	if (Wave < 8) EM.SetTimer(FlierLaunchTimerID, FlierCooldown[Wave - 1]);
}

void EnemyControl::NewGame()
{
	Reset();

	Wave = 0;
}


void EnemyControl::SpawnFlier()
{
	float speed = 0;

	if (GetRandomValue(0, 1) == 0)
	{
		speed = 60.0f / 3.0f;
		Flier->SetModel(BomberModel);
		Flier->Satelite = false;
	}
	else
	{
		speed = 60.0f / 2.0f;
		Flier->SetModel(SateliteModel);
		Flier->Satelite = true;
	}

	Flier->Spawn(speed);
}

void EnemyControl::Reset()
{
}
