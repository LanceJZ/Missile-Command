#include "TheICBMManager.h"

TheICBMManager::TheICBMManager()
{
	for (int i = 0; i < 8; i++)
	{
		EM.AddModel3D(ICBMs[i] = DBG_NEW Shot());
	}

	LaunchCheckTimerID = EM.AddTimer();
}

TheICBMManager::~TheICBMManager()
{
}

void TheICBMManager::SetMissileModels(Model& missileModel)
{
	for (int i = 0; i < 8; i++)
	{
		ICBMs[i]->SetModel(missileModel);
	}
}

bool TheICBMManager::Initialize()
{
	Common::Initialize();

	EM.SetTimer(LaunchCheckTimerID, 0.25f);

	NumberOfICBMsEachWave[0] = 12;
	NumberOfICBMsEachWave[1] = 15;
	NumberOfICBMsEachWave[2] = 18;
	NumberOfICBMsEachWave[3] = 12;
	NumberOfICBMsEachWave[4] = 16;
	NumberOfICBMsEachWave[5] = 14;
	NumberOfICBMsEachWave[6] = 17;
	NumberOfICBMsEachWave[7] = 10;
	NumberOfICBMsEachWave[8] = 13;
	NumberOfICBMsEachWave[9] = 16;
	NumberOfICBMsEachWave[10] = 19;
	NumberOfICBMsEachWave[11] = 12;
	NumberOfICBMsEachWave[12] = 14;
	NumberOfICBMsEachWave[13] = 16;
	NumberOfICBMsEachWave[14] = 18;
	NumberOfICBMsEachWave[15] = 14;
	NumberOfICBMsEachWave[16] = 16;
	NumberOfICBMsEachWave[17] = 18;
	NumberOfICBMsEachWave[18] = 20;

	return false;
}

bool TheICBMManager::BeginRun()
{
	Common::BeginRun();

	Reset();

	FireSalvo();

	return false;
}

void TheICBMManager::Update()
{
	Common::Update();

	if (EM.TimerElapsed(LaunchCheckTimerID))
	{
		EM.ResetTimer(LaunchCheckTimerID);

		if (IsItTimeForAnotherSalvo() && !WaveEnded) FireSalvo();
	}
}

void TheICBMManager::FireIICBM(Vector3& position, Vector3& target)
{
	Vector3 velocity = M.GetVelocityFromVectorsZ(position, target, MissileSpeed);

	for (int i = 0; i < 8; i++)
	{
		if (!ICBMs[i]->Enabled)
		{
			ICBMs[i]->Spawn(position, velocity, CurrentColor);
			ICBMs[i]->ModelColor = Aqua;
			break;
		}
	}
}

void TheICBMManager::NewWave()
{
	WaveEnded = false;
	Wave++;
	MissileSpeed *= 1.75f;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	ICBMsFiredThisWave = 0;

	if (CealingPercent > MinimumCleaingPercent) CealingPercent -= 0.75f;

	LaunchCealing = WindowHalfHeight -
		(WindowHalfHeight - (WindowFullHeight * CealingPercent) * 0.5f);

	FireSalvo();
}

void TheICBMManager::EndWave()
{
	WaveEnded = true;

	for (auto missile : ICBMs)
	{
		//missile->Reset();
	}
}

void TheICBMManager::Reset()
{
	Wave = 0;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	ICBMsFiredThisWave = 0;
	MissileSpeed = 20.15f;
	CealingPercent = 0.78f;
	LaunchCealing = -WindowHalfHeight +
		(WindowHalfHeight - (WindowHalfHeight * CealingPercent));
}

bool TheICBMManager::IsItTimeForAnotherSalvo()
{
	for (auto missile : ICBMs)
	{
		if (missile->Enabled)
		{
			if (missile->Position.y > LaunchCealing) return true;
		}
	}

	return false;
}

void TheICBMManager::FireSalvo()
{
	for (int i = 0; i < 4; i++)
	{
		for (auto missile : ICBMs)
		{
			if (ICBMsFiredThisWave >= ICBMsFiredMax) break;

			if (!missile->Enabled)
			{
				ICBMsFiredThisWave++;

				Vector3 startPosiotion = {M.GetRandomFloat((float)-WindowHalfWidth,
					(float)WindowHalfWidth), (float)-WindowHalfHeight + 33.0f, 0.0f};

				Vector3 velocity = M.GetVelocityFromVectorsZ(startPosiotion,
					Citys[GetRandomValue(0, 5)].Position, MissileSpeed);

				missile->Spawn(startPosiotion, velocity, CurrentColor);
				break;
			}
		}
	}
}