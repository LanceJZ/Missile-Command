#include "GameOverScreen.h"

GameOverScreen::GameOverScreen()
{

}

GameOverScreen::~GameOverScreen()
{
}

bool GameOverScreen::Initialize()
{
	OnScreenText::Initialize();

	return false;
}

bool GameOverScreen::BeginRun()
{
	BottomTextColor = BLACK;
	TopTextColor = Blue;

	return false;
}

void GameOverScreen::Update()
{
	OnScreenText::Update();

}

void GameOverScreen::Draw2D()
{
	if (!Enabled) return;

	OnScreenText::Draw2D();

	DrawText("GAME OVER  INSERT COINS", WindowHalfWidth / 2,
		WindowFullHeight - 45, 45, BottomTextColor);

	DrawText("PRESS N To Start New Game", WindowHalfWidth / 2,
		WindowHalfHeight + 100, 45, TopTextColor);
}
