#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	TargetColorTimerID = EM.AddTimer();
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetMissileManager(MissileBaseManager* baseManager)
{
	BaseManager = baseManager;
}

bool ThePlayer::Initialize()
{
	Model3D::Initialize();

	ModelColor = Blue;
	Cull = false;
	EM.SetTimer(TargetColorTimerID, 0.1f);

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

	CrosshairUpdate();
}

void ThePlayer::FixedUpdate(float deltaTime)
{
	TargetUpdate();
}

void ThePlayer::Draw3D()
{
	Model3D::Draw3D();

}

void ThePlayer::Hit()
{
	Acceleration = { 0 };
	Velocity = { 0 };
	Lives--;
	Enabled = false;
}

void ThePlayer::Hit(Vector3 location, Vector3 velocity)
{
	Entity::Hit();

}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}

	if (Score > NextNewLifeScore)
	{
		NextNewLifeScore += 10000;
		Lives++;
		NewLife = true;
	}
}

void ThePlayer::Reset()
{
	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Enabled = true;
}

void ThePlayer::Spawn(Vector3 position)
{
	Model3D::Spawn(position);
}

void ThePlayer::NewGame()
{
	Lives = 4;
	NextNewLifeScore = 10000;
	Score = 0;
	GameOver = false;
	Reset();
}

void ThePlayer::SetHighScore(int highScore)
{
	HighScore = highScore;
}

int ThePlayer::GetScore()
{
	return Score;
}

void ThePlayer::FireMissile()
{
	float distanceAlpha =
		Vector3Distance(Position, BaseManager->MissileBases[0]->Position);
	float distanceDelta =
		Vector3Distance(Position, BaseManager->MissileBases[1]->Position);
	float distanceOmega =
		Vector3Distance(Position, BaseManager->MissileBases[2]->Position);

	Entity target = {};

	size_t closest = 0;

	if (distanceDelta < distanceAlpha && distanceDelta < distanceOmega)
	{
		closest = 1;
	}
	else if (distanceOmega < distanceAlpha && distanceOmega < distanceDelta)
	{
		closest = 2;
	}

	if (!BaseManager->MissileFired(closest))
	{
		if (closest == 0)
		{
			closest = 1;

			if (!BaseManager->MissileFired(closest))
			{
				closest = 2;

				if (!BaseManager->MissileFired(closest))
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

			if (!BaseManager->MissileFired(closest))
			{
				if (closest == 2)
				{
					closest = 0;

					if (!BaseManager->MissileFired(closest))
					{
						return;
					}
				}
				else
				{
					closest = 2;

					if (!BaseManager->MissileFired(closest))
					{
						return;
					}
				}
			}
		}
		else if (closest == 2)
		{
			closest = 1;

			if (!BaseManager->MissileFired(closest))
			{
				closest = 0;

				if (!BaseManager->MissileFired(closest))
				{
					return;
				}
			}
		}
	}

	Vector3 missileBasePosition = BaseManager->MissileBases[closest]->Position;
	Vector3 missileVelocity =
		Vector3Multiply(GetVelocityFromVectorZ(missileBasePosition,
		ShotSpeed), {-1.0f, -1.0f, 0});

	bool spawnShot = true;
	size_t shotNumber = Missiles.size();

	for (size_t i = 0; i < shotNumber; i++)
	{
		if (!Missiles.at(i)->Enabled)
		{
			spawnShot = false;
			shotNumber = i;
			break;
		}
	}

	if (spawnShot)
	{
		Missiles.push_back(DBG_NEW Shot());
		EntityFactory(Missiles.back(), ShotModel, Aqua, missileBasePosition,
			missileVelocity);
	}
	else Missiles[shotNumber]->Spawn(missileBasePosition, missileVelocity);

	Missiles[shotNumber]->TargetIndex = SetTarget();
	Missiles[shotNumber]->TargetPosition = Position;
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
	else if (Y() > WindowHalfHeight)
	{
		Y((float)WindowHalfHeight);
		SetMousePosition((int)GetMousePosition().x, (int)WindowFullHeight);
	}
}

void ThePlayer::TargetUpdate()
{
	for (size_t i = 0; i < Targets.size(); i++)
	{
		if (Targets.at(i)->Enabled)
		{
			if (EM.TimerElapsed(TargetColorTimerID))
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
		EntityFactory(Targets.at (targetNumber), TargetModel, TargetColor, Position,
			{ 0, 0, 0 });
	}
	else Targets.at(targetNumber)->Spawn(Position);

	return targetNumber;
}

void ThePlayer::EntityFactory(Model3D* entity, Model& model, Color color,
	Vector3 position, Vector3 velocity)
{
	EM.AddModel3D(entity, model);
	entity->ModelColor = color;
	entity->Cull = false;
	entity->Initialize();
	entity->BeginRun();
	entity->Spawn(position);

	if (velocity.x != 0.0f && velocity.y != 0.0f) entity->Velocity = velocity;
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
		FireMissile();
	}
}
