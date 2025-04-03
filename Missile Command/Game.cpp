#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().

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

bool Game::Initialize() //Initialize
{
	Common::Initialize();

	SetWindowTitle("Missile Command Alpha 0.01");

	float multiW = 1.0f, multiH = 1.0f;
	FieldSize = { GetScreenWidth() * multiW, (float)GetScreenHeight() * multiH };

	Logic->SetPlayer(Player);
	Logic->SetMissileBases(ABMBases);
	Logic->SetEnemies(Enemies);
	Logic->SetTheCityManager(Cities);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.
	DisableCursor();

	return true;
}

//..
// Model Names:
// City = InnerCity and OuterCity, Ground = Ground, Cube = Cube, Cross = Cross,
// Smart Bomb = SmartBomb, SmartBombEdge,
// Satellite = MC_SatelliteMain-1, MC_SataliteInside,
// Player = PlayerCursor, Player target for player missile = PlayerTarget,
// Missile Ammo = MissileAmmo, Explosion = Explosion
// Bomber = MC_Bomber
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
	GameInput();
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
	Logic->Input();

	if (IsKeyPressed(KEY_P))
	{
		if (Logic->State == Pause) Logic->State = InPlay;
		else if (Logic->State == InPlay) Logic->State = Pause;
	}

	if (Logic->State == Ended)
	{
		if (IsKeyPressed(KEY_N)) Logic->NewGame();
	}
}