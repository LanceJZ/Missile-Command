#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().
	EM.AllInView = true;

	LogicID = EM.AddCommon(Logic = DBG_NEW GameLogic());
	BackGroundID = EM.AddCommon(BackGround = DBG_NEW TheBackground());
	PlayerID = EM.AddModel3D(Player = DBG_NEW ThePlayer());
	MissileBasesID = EM.AddCommon(ABMBases = DBG_NEW TheABMBaseManager());
	EnemiesID = EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	CitiesID = EM.AddCommon(Cities = DBG_NEW TheCityManager());
}

Game::~Game()
{
}

bool Game::Initialize()
{
	Common::Initialize();

	SetWindowTitle("Missile Command Beta 1.00");

	float multiW = 1.0f, multiH = 1.0f;
	FieldSize = { GetScreenWidth() * multiW, (float)GetScreenHeight() * multiH };

	Logic->SetBackground(BackGround);
	Logic->SetPlayer(Player);
	Logic->SetMissileBases(ABMBases);
	Logic->SetEnemies(Enemies);
	Logic->SetTheCityManager(Cities);

	Enemies->SetPlayer(Player);

	Score.SetPosition((int)(WindowHalfWidth / 1.75f), 2);
	HighScore.SetPosition((int)(WindowHalfWidth * 1.15f), 2);

	//Any Entities added after this point need this method fired manually.
	DisableCursor();

	return true;
}

//..
// Model Names:
// City = InnerCity and OuterCity, Ground = Ground, Cube = Cube, Cross = Cross,
// Smart Bomb = SmartBomb, SmartBombEdge,
// Satellite = MC_SatelliteMain-1, MC_SataliteInside, Bomber = MC_Bomber,
// Player = PlayerCursor, Player target for player missile = PlayerTarget,
// Missile Ammo = MissileAmmo, Explosion = Explosion
//..
//..
// Sound Names:
// Explosion = Explosion, Player Fire ABM = ABMLaunch, Out of Ammo = AmmoOut,
// Low On Ammo = AmmoLow, Bonus City Awarded = BonusCityTune,
// Incoming ICBMs = Radar, Counting bonus city = CityCount,
// Counting bonus ammo = AmmoCount,
// Smart Bomb = SmartBomb (0.2948), Wave Start = Wave Start, Flier = Flier

bool Game::Load()
{
	Model cube = CM.LoadAndGetModel("Cube");

	Logic->SetExplosionModel(CM.LoadAndGetModel("Explosion"));
	BackGround->SetGroundModel(CM.LoadAndGetModel("Ground"));


	Player->SetModel(CM.LoadAndGetModel("PlayerCursor"));
	Player->SetTargetandShotModel(CM.LoadAndGetModel("PlayerTarget"), cube);

	ABMBases->SetMissileModel(CM.LoadAndGetModel("MissileAmmo"));

	Enemies->SetICBMModel(cube);
	Enemies->SetBomberModel(CM.LoadAndGetModel("MC_Bomber"));
	Enemies->SetSateliteModel(CM.LoadAndGetModel("MC_SatelliteMain-1"),
		CM.LoadAndGetModel("MC_SateliteInside"));
	Enemies->SetSmartBombModel(CM.LoadAndGetModel("SmartBomb"),
		CM.LoadAndGetModel("SmartBombEdge"));

	Cities->SetCityModels(CM.LoadAndGetModel("InnerCity"),
		CM.LoadAndGetModel("OuterCity"));

	// -------------- Sounds ---------------
	Logic->SetWaveStartSound(CM.LoadAndGetSound("Wave Start"));
	Logic->SetBonusCitySound(CM.LoadAndGetSound("BonusCityTune"));
	Logic->SetExplosionSound(CM.LoadAndGetSound("Explosion"));
	Logic->SetFlierSound(CM.LoadAndGetSound("Flier"));
	Logic->SetSmartBombSound(CM.LoadAndGetSound("SmartBomb"));
	Logic->SetAmmoCityCountSound(CM.LoadAndGetSound("AmmoCount"),
		CM.LoadAndGetSound("CityCount"));

	Player->SetABMLaunchSound(CM.LoadAndGetSound("ABMLaunch"));
	Player->SetLowOnAmmoSound(CM.LoadAndGetSound("AmmoLow"));
	Player->SetAmmoOutSound(CM.LoadAndGetSound("AmmoOut"));

	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.

	return true;
}

void Game::Input()
{
	if (Logic->State == TheMainMenu) Logic->Input();
}


void Game::Update(float deltaTime)
{
	if (Logic->State == Pause)	return;
}

void Game::FixedUpdate(float deltaTime)
{
	if (Logic->State == Pause)	return;
}

void Game::Draw()
{
	BeginMode3D(TheCamera);

	//3D Drawing here.
	Draw3D();

	EndMode3D();

	//2D drawing, fonts go here.

	Draw2D();
}

void Game::Draw3D()
{

#ifdef _DEBUG
	int fsx = int(FieldSize.x * 0.5f);
	int fsy = int(FieldSize.y * 0.5f);

	DrawLine(-fsx, -fsy, fsx, -fsy, { DARKBLUE });
	DrawLine(fsx, -fsy, fsx, fsy, { DARKBLUE });
	DrawLine(fsx, fsy - 1, -fsx, fsy, { DARKBLUE });
	DrawLine(-fsx + 0.5f, fsy - 1, -fsx + 0.5f, -fsy, { DARKBLUE });
#endif
}

void Game::Draw2D()
{
}

void Game::GameInput()
{
}