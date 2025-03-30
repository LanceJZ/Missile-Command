#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	EM.AddCommon(Missiles = DBG_NEW MissileManager());
}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void EnemyControl::SetMissileModels(Model& missileModel, Model& trailmodel)
{
	Missiles->SetMissileModels(missileModel, trailmodel);
}

bool EnemyControl::Initialize()
{
	Common::Initialize();


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

}

void EnemyControl::NewGame()
{
	Reset();

}


void EnemyControl::Reset()
{

}
