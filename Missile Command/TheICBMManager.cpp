#include "TheICBMManager.h"

TheICBMManager::TheICBMManager()
{
	for (int i = 0; i < 8; i++)
	{
		EM.AddModel3D(ICBMs[i] = DBG_NEW Shot());
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

	FlierFireRate[0] = 128.0f/60.0f;
	FlierFireRate[1] = 96.0f/60.0f;
	FlierFireRate[2] = 64.0f/60.0f;
	FlierFireRate[3] = 48.0f/60.0f;
	FlierFireRate[4] = 32.0f/60.0f;
	FlierFireRate[5] = FlierFireRate[4];
	FlierFireRate[6] = 16.0f/60.0f;

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

	if (OutOfMissiles) return;

	if (ICBMsFiredThisWave > ICBMsFiredMax)
	{
		OutOfMissiles = true;
		return;
	}

	if (EM.TimerElapsed(LaunchCheckTimerID))
	{
		EM.ResetTimer(LaunchCheckTimerID);

		if (IsItTimeForAnotherSalvo() && !WaveEnded) FireSalvo();
	}

	if (Flier->Enabled)
	{
		if (EM.TimerElapsed(FlierFireTimerID))
		{
			if (FlierFires()) EM.ResetTimer(FlierFireTimerID);
		}
	}
}

void TheICBMManager::ResetFlierFireTimer()
{
	EM.ResetTimer(FlierFireTimerID);
}

void TheICBMManager::NewWave(Color waveColor)
{
	CurrentColor = waveColor;
	OutOfMissiles = false;
	WaveEnded = false;
	Wave++;
	MissileSpeed *= 1.25f;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	ICBMsFiredThisWave = 0;

	if (CealingPercent > MinimumCleaingPercent) CealingPercent -= 0.018f;

	LaunchCealing = GetLaunchCealing();

	if (Wave > 0 &&	Wave < 8)
	{
		EM.SetTimer(FlierFireTimerID, FlierFireRate[Wave - 1]);
	}

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
	OutOfMissiles = false;
	ICBMsFiredMax = NumberOfICBMsEachWave[Wave];
	ICBMsFiredThisWave = 0;
	MissileSpeed = 20.15f;
	CealingPercent = 0.68f;
	LaunchCealing = GetLaunchCealing();

	for (const auto& missiles : ICBMs)
	{
		missiles->Destroy();
	}

	Flier->Destroy();
}

bool TheICBMManager::IsItTimeForAnotherSalvo()
{
	int activeICBMs = 0;
	bool belowCealing = false;

	for (const auto &missile : ICBMs)
	{
		if (missile->Enabled && !missile->ByFlier)
		{
			if (missile->Position.y > LaunchCealing) belowCealing = true;

			activeICBMs++;
		}
	}

	if (belowCealing || activeICBMs < 2) return true;

	return false;
}

void TheICBMManager::FireSalvo()
{
	// TODO: Change X so it adds a random value to the next one, starting
	// TODO: from -WindowWidth +45, make sure they all don't add up to more than
	// TODO: WindowWidth - 45.

	for (int i = 0; i < 4; i++)
	{
		for (const auto &missile : ICBMs)
		{
			if (!missile->Enabled)
			{
				Vector3 position = {M.GetRandomFloat((float)-WindowHalfWidth,
					(float)WindowHalfWidth), (float)-WindowHalfHeight + 45.0f, 0.0f};
				FireICBM(missile, position);
				missile->ByFlier = false;
				if (ICBMsFiredThisWave > ICBMsFiredMax) return;
				break;
			}
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
