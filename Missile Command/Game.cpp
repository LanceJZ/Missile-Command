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

	SetWindowTitle("Missile Command Alpha 0.05");

	float multiW = 1.0f, multiH = 1.0f;
	FieldSize = { GetScreenWidth() * multiW, (float)GetScreenHeight() * multiH };

	Logic->SetBackground(BackGround);
	Logic->SetPlayer(Player);
	Logic->SetMissileBases(ABMBases);
	Logic->SetEnemies(Enemies);
	Logic->SetTheCityManager(Cities);

	Enemies->SetPlayer(Player);

	Score.SetPosition((int)(WindowHalfWidth / 1.75f), 2);
	Score.SetColor(Red);
	HighScore.SetPosition((int)(WindowHalfWidth * 1.15f), 2);
	HighScore.SetColor(Red);

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

	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.

	return true;
}

void Game::Input()
{
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