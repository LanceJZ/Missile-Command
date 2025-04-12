#include "TheICBMManager.h"

TheICBMManager::TheICBMManager()
{
	for (int i = 0; i < 8; i++)
	{
		EM.AddModel3D(ICBMs[i] = DBG_NEW Shot());
	}

	for (int i = 0; i < 3; i++)
	{
		EM.AddModel3D(SmartBombs[i] = DBG_NEW TheSmartBomb());
	}

	LaunchCheckTimerID = EM.AddTimer();
	FlierFireTimerID = EM.AddTimer();
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

void TheICBMManager::SetBomberReference(TheFlier* bomber)
{
	Flier = bomber;
}

void TheICBMManager::SetSmartBombModel(Model& smartBombMainModel, Model& smartBombEdgeModel)
{
	for (int i = 0; i < 3; i++)
	{
		SmartBombs[i]->SetModel(smartBombMainModel);
		SmartBombs[i]->SetEdgeModel(smartBombEdgeModel);
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

	NumberOfSmartBombsEachWave[0] = 1; // Starts at wave 6
	NumberOfSmartBombsEachWave[1] = 1;
	NumberOfSmartBombsEachWave[2] = 2;
	NumberOfSmartBombsEachWave[3] = 3;
	NumberOfSmartBombsEachWave[4] = 4;
	NumberOfSmartBombsEachWave[5] = 4;
	NumberOfSmartBombsEachWave[6] = 5;
	NumberOfSmartBombsEachWave[7] = 5;
	NumberOfSmartBombsEachWave[8] = 6;
	NumberOfSmartBombsEachWave[9] = 6;
	NumberOfSmartBombsEachWave[10] = 7; // Continues with this amount forever more.

	FlierFireRate[0] = 128.0f / 60.0f;
	FlierFireRate[1] = 96.0f / 60.0f;
	FlierFireRate[2] = 64.0f / 60.0f;
	FlierFireRate[3] = 48.0f / 60.0f;
	FlierFireRate[4] = 32.0f / 60.0f;
	FlierFireRate[5] = FlierFireRate[4];
	FlierFireRate[6] = 16.0f / 60.0f;

	ICBMSpeedForWave[0] = 60.0f / 4.8125f;
	ICBMSpeedForWave[1] = 60.0f / 2.875f;
	ICBMSpeedForWave[2] = 60.0f / 1.75f;
	ICBMSpeedForWave[3] = 60.0f / 1.3f;
	ICBMSpeedForWave[4] = 60.0f / 1.1625f;
	ICBMSpeedForWave[5] = 60.0f / 1.1375f;
	ICBMSpeedForWave[6] = 60.0f / 1.125f;
	ICBMSpeedForWave[7] = 60.0f / 1.025f;
	ICBMSpeedForWave[8] = 60.0f / 1.01625f;
	ICBMSpeedForWave[9] = 60.0f / 1.014f;
	ICBMSpeedForWave[10] = 60.0f / 1.012f;
	ICBMSpeedForWave[11] = 60.0f / 1.006f;
	ICBMSpeedForWave[12] = 60.0f / 1.0018f;
	ICBMSpeedForWave[13] = 60.0f / 1.0014f;
	ICBMSpeedForWave[14] = 60.0f / 1.0012f;
	ICBMSpeedForWave[15] = 60.0f / 1.0006f;
	ICBMSpeedForWave[16] = 60.0f / 1.00018f;
	ICBMSpeedForWave[17] = 60.0f / 1.00014f;
	ICBMSpeedForWave[18] = 60.0f / 1.000125f;

	return false;
}

bool TheICBMManager::BeginRun()
{
	Common::BeginRun();

	Reset();

	CitiesToTarget();

	//FireSalvo();

	return false;
}

void TheICBMManager::Update()
{
	Common::Update();

	//for (const auto &missile : ICBMs)
	//{
	//	if (missile->Enabled)
	//	{
	//		if (missile->Position.y > (float)WindowHalfHeight - 14.0f * 5.0f)
	//		{
	//			missile->Destroy();
	//		}
	//	}
	//}

	if (OutOfMissiles) return;

	if (WaveEnded) return;

	if (ICBMsFiredThisWave > ICBMsFiredMax)
	{
		OutOfMissiles = true;
		return;
	}

	if (EM.TimerElapsed(LaunchCheckTimerID))
	{
		EM.ResetTimer(LaunchCheckTimerID);

		if (Wave > 4 && ICBMsFiredThisWave > 4)
		{
			int attacks = 0;

			for (const auto &smartBomb : SmartBombs)
			{
				if (smartBomb->Enabled) attacks+=2;
			}

			for (const auto &missile : ICBMs)
			{
				if (missile->Enabled) attacks++;
			}

			if (attacks < 8) LaunchSmartBomb();
			else if (IsItTimeForAnotherSalvo()) FireSalvo();
		}

		IsItMERVTime();

		if (Flier->Enabled)
		{
			if (EM.TimerElapsed(FlierFireTimerID) && ICBMsFiredThisWave > 4)
			{
				if (FlierFires()) EM.ResetTimer(FlierFireTimerID);
			}
		}

		if (IsItTimeForAnotherSalvo())
		{
			FireSalvo();
		}
	}

}

void TheICBMManager::ResetFlierFireTimer()
{
	EM.ResetTimer(FlierFireTimerID);
}

void TheICBMManager::NewWave(Color icbmColor, Color edgeColor)
{
	CurrentColor = icbmColor;
	OutOfMissiles = false;
	WaveEnded = false;
	Wave++;
	ICBMMaxSalvosThisWave++;

	if (Wave < 19)
	{
		MissileSpeed = ICBMSpeedForWave[Wave] * 2.15f;
		ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	}
	else MissileSpeed = 60.0f * 2.15f;

	ICBMsFiredThisWave = 0;
	SmartBombsFiredThisWave = 0;

	if (CealingPercent > MinimumCleaingPercent) CealingPercent -= 0.018f;

	LaunchCealing = GetLaunchCealing();

	if (Wave > 0 &&	Wave < 8)
	{
		EM.SetTimer(FlierFireTimerID, FlierFireRate[Wave - 1]);
	}

	for (const auto &smartBomb : SmartBombs)
		smartBomb->NextWave(icbmColor, edgeColor);

	CitiesToTarget();
	FireSalvo();
}

void TheICBMManager::EndWave()
{
	WaveEnded = true;
}

void TheICBMManager::Reset()
{
	for (const auto& missiles : ICBMs)
	{
		missiles->Destroy();
	}

	for (const auto& smartBombs : SmartBombs)
	{
		smartBombs->Destroy();
	}

	Flier->Destroy();
}

void TheICBMManager::NewGame()
{
	Wave = 0;
	ICBMMaxSalvosThisWave = 2;
	ICBMSalvosFired = 0;
	OutOfMissiles = false;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	MissileSpeed = ICBMSpeedForWave[Wave] * 2.15f;
	ICBMsFiredThisWave = 0;
	SmartBombsFiredThisWave = 0;
	CealingPercent = 0.68f;
	LaunchCealing = GetLaunchCealing();
	CurrentColor = Red;

	Reset();
}

bool TheICBMManager::IsItTimeForAnotherSalvo()
{
	if (ICBMSalvosFired > ICBMMaxSalvosThisWave)
	{
		bool tooLowForMIRV = true;

		for (const auto &missile : ICBMs)
		{
			if (missile->Enabled)
			{
				if (missile->Position.y > 0.0f) tooLowForMIRV = true;
			}
		}

		if (!tooLowForMIRV) return false;
	}

	int activeICBMs = 0;
	bool belowCealing = false;

	for (const auto &smartBomb : SmartBombs)
	{
		if (smartBomb->Enabled) activeICBMs+=2;
	}

	for (const auto &missile : ICBMs)
	{
		if (missile->Enabled && !missile->ByFlier)
		{
			activeICBMs++;

			if (activeICBMs > 7) return false;

			if (missile->Position.y > LaunchCealing) belowCealing = true;
		}
	}

	if (belowCealing || activeICBMs < 2) return true;

	return false;
}

void TheICBMManager::IsItMERVTime()
{
	int activeICBMs = 0;

	for (const auto &smartBomb : SmartBombs)
	{
		if (smartBomb->Enabled) activeICBMs+=2;
	}

	for (const auto& missile : ICBMs)
	{
		if (missile->Enabled && !missile->ByFlier)
		{
			activeICBMs++;

			if (activeICBMs > 7) return;

			if (missile->Position.y > -(159.0f - 115.0f) * 4.15f)
			{
				if (missile->Position.y < -(128.0f - 115.0f) * 4.15f)
				{
					Vector3 position = missile->Position;

					for (int i = 0; i < 3; i++)
					{
						for (const auto& missileMIRV : ICBMs)
						{
							if (!missileMIRV->Enabled)
							{
								FireICBM(missileMIRV, position);
							}
						}
					}

					break;
				}
			}
		}
	}
}

void TheICBMManager::FireSalvo()
{
	// TODO: Change X so it adds a random value to the next one, starting
	// TODO: from -WindowWidth +45, make sure they all don't add up to more than
	// TODO: WindowWidth - 45.

	ICBMSalvosFired ++;

	float missileXPosition = (float)-WindowHalfWidth +
		M.GetRandomFloat(105.0f, 400.0f);

	for (int i = 0; i < 4; i++)
	{
		for (const auto &missile : ICBMs)
		{
			if (!missile->Enabled)
			{
				Vector3 position = {missileXPosition,
					(float)-WindowHalfHeight + 45.0f, 0.0f};

				missileXPosition += M.GetRandomFloat(75.0f, 320.0f);

				if (missileXPosition > (float)WindowHalfWidth - 145.0f)
					missileXPosition = (float)WindowHalfWidth - 145.0f;

				FireICBM(missile, position);

				missile->ByFlier = false;

				if (ICBMsFiredThisWave > ICBMsFiredMax) return;

				break;
			}
		}
	}
}

void TheICBMManager::LaunchSmartBomb()
{
	for (const auto &smartBomb : SmartBombs)
	{
		if (!smartBomb->Enabled)
		{
			SmartBombsFiredThisWave++;
			Vector3 position = {M.GetRandomFloat((float)-WindowHalfWidth,
				(float)WindowHalfWidth), (float)-WindowHalfHeight + 45.0f, 0.0f};
			int cityIndex = GetRandomValue(0, 5);
			Vector3 target = {0.0f, 0.0f, 0.0f};

			if (Cities[cityIndex].Targeted)
			{
				target = Cities[cityIndex].Position;
			}
			else
			{
				target = ABMBases[GetRandomValue(0, 2)].Position;
			}

			smartBomb->Spawn(position, target, MissileSpeed);

			if (Wave > 8) smartBomb->OrderNineIsEngaged = true;

			break;
		}
	}
}

bool TheICBMManager::FlierFires()
{
	for (const auto &missile : ICBMs)
	{
		if (!missile->Enabled)
		{
			FireICBM(missile, Flier->Position);
			missile->ByFlier = true;
			return true;
		}
	}

	return false;
}

void TheICBMManager::FireICBM(Shot* missile, Vector3& position)
{
	ICBMsFiredThisWave++;

	int cityIndex = GetRandomValue(0, 5);
	Vector3 target = {0.0f, 0.0f, 0.0f};

	if (Cities[cityIndex].Targeted)	target = Cities[cityIndex].Position;
	else target = ABMBases[GetRandomValue(0, 2)].Position;

	Vector3 velocity = M.GetVelocityFromVectorsZ(position,
		target, MissileSpeed);

	missile->Spawn(position, target, velocity, CurrentColor);
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

float TheICBMManager::GetLaunchCealing()
{
	return -(WindowHalfHeight * (CealingPercent));
}
