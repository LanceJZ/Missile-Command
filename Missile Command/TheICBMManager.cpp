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

	CitiesToTarget();

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

	for (const auto &missile : ICBMs)
	{
		if (missile->Enabled)
		{
			if (missile->Position.y > (float)WindowHalfHeight - 14.0f * 5.0f)
			{
				missile->Destroy();
			}
		}
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
	MissileSpeed *= 1.25f;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	ICBMsFiredThisWave = 0;

	if (CealingPercent > MinimumCleaingPercent) CealingPercent -= 0.025f;

	LaunchCealing = WindowHalfHeight -
		(WindowHalfHeight - (WindowFullHeight * CealingPercent) * 0.5f);

	CitiesToTarget();
	FireSalvo();
}

void TheICBMManager::EndWave()
{
	WaveEnded = true;
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
	int activeICBMs = 0;

	for (const auto &missile : ICBMs)
	{
		if (missile->Enabled)
		{
			if (missile->Position.y > LaunchCealing) return true;

			activeICBMs++;
		}
	}

	if (activeICBMs < 2) return true;

	return false;
}

void TheICBMManager::FireSalvo()
{
	for (int i = 0; i < 4; i++)
	{
		for (auto missile : ICBMs)
		{
			if (ICBMsFiredThisWave > ICBMsFiredMax) break;

			if (!missile->Enabled)
			{
				ICBMsFiredThisWave++;

				Vector3 startPosiotion = {M.GetRandomFloat((float)-WindowHalfWidth,
					(float)WindowHalfWidth), (float)-WindowHalfHeight + 33.0f, 0.0f};


				int cityIndex = GetRandomValue(0, 5);
				Vector3 target = {0.0f, 0.0f, 0.0f};

				if (Cities[cityIndex].Targeted)
				{
					target = Cities[cityIndex].Position;
				}
				else
				{
					if (GetRandomValue(0, 1) == 0)
						target = ABMBases[GetRandomValue(0, 2)].Position;
					else
					{
						target.y = (float)WindowHalfHeight - 10.0f * 5.0f;

						if (cityIndex == 0) target.x =
							Cities[0].Position.x +
							(Cities[1].Position.x / 2.0f -
							Cities[0].Position.x / 2.0f);

						if (cityIndex == 1) target.x =
							Cities[1].Position.x +
							(Cities[2].Position.x / 2.0f -
							Cities[1].Position.x / 2.0f);

						if (cityIndex == 2) target.x =
							Cities[2].Position.x + 100.0f;

						if (cityIndex == 3) target.x =
							Cities[3].Position.x +
							(Cities[4].Position.x / 2.0f -
							Cities[3].Position.x / 2.0f);

						if (cityIndex == 4 || cityIndex == 5) target.x =
							Cities[5].Position.x +
							(Cities[5].Position.x / 2.0f -
							Cities[4].Position.x / 2.0f);

						if (cityIndex == 5) target.x =
							Cities[5].Position.x + 100.0f;

						target.z = 0.0f;
					}
				}

				Vector3 velocity = M.GetVelocityFromVectorsZ(startPosiotion,
					target, MissileSpeed);

				missile->Spawn(startPosiotion, velocity, CurrentColor);
				break;
			}
		}
	}
}

void TheICBMManager::CitiesToTarget()
{
	int activeCities = 0;
	int maxActiveCitiesTargeted = 3;
	int citiesTargeted = 0;

	for (auto city : Cities)
	{
		city.Targeted = false;

		if (city.Active) activeCities++;
	}

	if (activeCities > maxActiveCitiesTargeted)
		activeCities = maxActiveCitiesTargeted;
	else if (activeCities < maxActiveCitiesTargeted)
	{
		maxActiveCitiesTargeted = activeCities / 2;

		if (maxActiveCitiesTargeted < 1) maxActiveCitiesTargeted = 1;
	}

	while (citiesTargeted < maxActiveCitiesTargeted)
	{
		int cityIndex = GetRandomValue(0, 5);

		if (!Cities[cityIndex].Targeted && Cities[cityIndex].Active)
		{
			Cities[cityIndex].Targeted = true;
			citiesTargeted++;
		}
	}
}
