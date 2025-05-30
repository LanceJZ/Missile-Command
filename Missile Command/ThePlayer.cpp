#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	EM.AddOnScreenText(AmmoText = DBG_NEW TheAmmoScreen());

	TargetColorTimerID = EM.AddTimer();
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetTheABMBaseManager(TheABMBaseManager* baseManager)
{
	ABMBaseManager = baseManager;
}

void ThePlayer::SetABMLaunchSound(Sound sound)
{
	ABMLaunchSound = sound;
}

void ThePlayer::SetLowOnAmmoSound(Sound sound)
{
	LowOnAmmoSound = sound;
}

void ThePlayer::SetAmmoOutSound(Sound sound)
{
	AmmoOutSound = sound;
}

bool ThePlayer::Initialize()
{
	Model3D::Initialize();

	EM.SetTimer(TargetColorTimerID, 0.05f);

	return false;
}

bool ThePlayer::BeginRun()
{
	Model3D::BeginRun();

	Radius = 0.0f;

	return false;
}

void ThePlayer::SetTargetandShotModel(Model& targetModel, Model& shotModel)
{
	TargetModel = targetModel;
	ShotModel = shotModel;
}

void ThePlayer::Input()
{
	Model3D::Input();

	Keyboard();
	Mouse();

	if (IsGamepadAvailable(0))
	{
		Gamepad();
	}
}

void ThePlayer::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	for (size_t i = 0; i < 3; i++)
	{
		if (ABMBaseManager->ABMBases[i]->OutOfAmmo) AmmoText->AmmoIsOut(i);
	}
}

void ThePlayer::FixedUpdate(float deltaTime)
{
	CrosshairUpdate();
	TargetUpdate();
}

void ThePlayer::Draw3D()
{
	Model3D::Draw3D();

}

void ThePlayer::Reset()
{
	for (const auto &abms: ABMs) abms->Destroy();

	for (const auto &targets: Targets) targets->Destroy();

	AmmoText->Reset();
}

void ThePlayer::NextWave(Color color)
{
	Enabled = true;
	ModelColor = color;
	AmmoText->SetColor(color);
}

void ThePlayer::Spawn(Vector3 position)
{
	Model3D::Spawn(position);
}

void ThePlayer::NewGame()
{
	NextWave(Blue);
}

void ThePlayer::FireABM()
{
	float distanceAlpha =
		Vector3Distance(Position, ABMBaseManager->ABMBases[0]->Position);
	float distanceDelta =
		Vector3Distance(Position, ABMBaseManager->ABMBases[1]->Position);
	float distanceOmega =
		Vector3Distance(Position, ABMBaseManager->ABMBases[2]->Position);

	size_t closest = 0;
	Entity target = {};


	if (distanceDelta < distanceAlpha && distanceDelta < distanceOmega)
	{
		closest = 1;
	}
	else if (distanceOmega < distanceAlpha && distanceOmega < distanceDelta)
	{
		closest = 2;
	}

	if (!ABMBaseManager->MissileFired(closest))
	{
		PlaySound(AmmoOutSound);

		if (closest == 0)
		{
			closest = 1;

			if (!ABMBaseManager->MissileFired(closest))
			{
				closest = 2;

				if (!ABMBaseManager->MissileFired(closest))
				{
					return;
				}
			}
		}
		else if (closest == 1)
		{
			if (distanceAlpha < distanceOmega)
			{
				closest = 0;
			}
			else
			{
				closest = 2;
			}

			if (!ABMBaseManager->MissileFired(closest))
			{
				PlaySound(AmmoOutSound);

				if (closest == 2)
				{
					closest = 0;

					if (!ABMBaseManager->MissileFired(closest))
					{
						return;
					}
				}
				else
				{
					closest = 2;

					if (!ABMBaseManager->MissileFired(closest))
					{
						return;
					}
				}
			}
		}
		else if (closest == 2)
		{
			closest = 1;

			if (!ABMBaseManager->MissileFired(closest))
			{
				PlaySound(AmmoOutSound);
				closest = 0;

				if (!ABMBaseManager->MissileFired(closest))
				{
					return;
				}
			}
		}
	}

	Vector3 abmBasePosition = ABMBaseManager->ABMBases[closest]->Position;
	Vector3 abmVelocity =
		M.GetVelocityFromVectorsZ(abmBasePosition, Position, ShotSpeed);

	PlaySound(ABMLaunchSound);

	if (ABMBaseManager->ABMBases[closest]->LowAmmo)
	{
		AmmoText->AmmoIsLow(closest);
		PlaySound(LowOnAmmoSound);
	}

	bool spawnShot = true;
	size_t shotNumber = ABMs.size();

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (!ABMs.at(i)->Enabled)
		{
			spawnShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnShot)
	{
		ABMs.push_back(DBG_NEW Shot());
		FM.Model3DFactory(ABMs.back(), ShotModel, CurrentColor, abmBasePosition,
			abmVelocity);
	}

	ABMs[shotNumber]->Spawn(abmBasePosition, Position,
		abmVelocity, CurrentColor);
	ABMs[shotNumber]->TargetIndex = SetTarget();
}

void ThePlayer::CrosshairUpdate()
{
	X(GetMousePosition().x - WindowHalfWidth);
	Y(GetMousePosition().y - WindowHalfHeight);

	if (X() < -WindowHalfWidth)
	{
		X(-WindowHalfWidth);
		SetMousePosition(0, (int)GetMousePosition().y);
	}
	else if (X() > WindowHalfWidth)
	{
		X(WindowHalfWidth);
		SetMousePosition((int)WindowFullWidth, (int)GetMousePosition().y);
	}

	if (Y() < -WindowHalfHeight)
	{
		Y(-WindowHalfHeight);
		SetMousePosition((int)GetMousePosition().x, 0);
	}
	else if (Y() > WindowHalfHeight - 203.0f)
	{
		Y((float)WindowHalfHeight - 203.0f);
		SetMousePosition((int)GetMousePosition().x, (int)WindowFullHeight - 203);
	}
}

void ThePlayer::TargetUpdate()
{
	if (EM.TimerElapsed(TargetColorTimerID))
	{
		EM.ResetTimer(TargetColorTimerID);

		for (size_t i = 0; i < Targets.size(); i++)
		{
			if (Targets.at(i)->Enabled)
			{
				Targets.at(i)->ModelColor = GameColors.ChangeColor();
			}
		}
	}
}

size_t ThePlayer::SetTarget()
{
	bool spawnTarget = true;
	size_t targetNumber = Targets.size();

	for (size_t i = 0; i < targetNumber; i++)
	{
		if (!Targets.at(i)->Enabled)
		{
			spawnTarget = false;
			targetNumber = i;
			break;
		}
	}

	if (spawnTarget)
	{
		Targets.push_back(new Model3D());
		FM.Model3DFactory(Targets.at (targetNumber), TargetModel, CurrentColor,
			Position, { 0.0f, 0.0f, 0.0f });
		Targets.back()->Radius = 4.0f;
	}
	else Targets.at(targetNumber)->Spawn(Position);

	return targetNumber;
}

void ThePlayer::Gamepad()
{
	//Button B is 6 for Shield //Button A is 7 for Fire //Button Y is 8 for Hyperspace
	//Button X is 5	//Left bumper is 9 //Right bumper is 11 for Shield //Left Trigger is 10
	//Right Trigger is 12 for Thrust //Dpad Up is 1 for	//Dpad Down is 3 for
	//Dpad Left is 4 for rotate left //Dpad Right is 2 for rotate right
	//Axis 1 is -1 for , 1 for  on left stick.
	//Axis 0 is -1 for Left, 1 for right on left stick.

	if (IsGamepadButtonDown(0, 12))
	{
	}
	else
	{
	}

	if (IsGamepadButtonDown(0, 4) || GetGamepadAxisMovement(0, 0) < -0.25f)
	{
	}
	else if (IsGamepadButtonDown(0, 2) || GetGamepadAxisMovement(0, 0) > 0.25f)
	{
	}

	if (IsGamepadButtonPressed(0, 7))
	{
	}

	if (IsGamepadButtonDown(0, 11) || IsGamepadButtonDown(0, 6))
	{
	}
	else
	{
	}
}

void ThePlayer::Keyboard()
{
	if (IsKeyDown(KEY_RIGHT))
	{
	}
	else if (IsKeyDown(KEY_LEFT))
	{
	}
	else
	{
	}

	if (IsKeyDown(KEY_UP))
	{
	}
	else
	{
	}

	if (IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL) ||
		IsKeyPressed(KEY_SPACE))
	{
	}

	if (IsKeyDown(KEY_DOWN))
	{
	}
	else
	{
	}

	if (IsKeyPressed(KEY_A)) //Base Alpha
	{

	}

	if (IsKeyPressed(KEY_S)) //Base Delta
	{

	}

	if (IsKeyPressed(KEY_D)) //Base Omega
	{

	}
}

void ThePlayer::Mouse()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		FireABM();
	}
}
