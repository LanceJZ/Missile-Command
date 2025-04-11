#pragma warning ( push )
#pragma warning ( disable : 26495 )

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "raylib.h"
#include "Game.h"
#include "glfw/glfw3.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

ContentManager CM = {};
EntityManager EM = {};
FactoryManager FM = {};
KnightMath M = {};
ParticleManager Particles = {};
Camera TheCamera = {};
Vector2 FieldSize = {};
TheScore Score = {};
TheScore HighScore = {};

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	static Game game;

	int windowHeight = 960; //height
	int windowWidth = 1280; //width

	InitWindow(windowWidth, windowHeight, "Missile Command");
	InitAudioDevice();

	Image icon = LoadImage("icon.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);

	if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
	glfwSwapInterval(0);
	SetTargetFPS(120);

	Particles.Initialize();
	Particles.SetManagers(EM);
	FM.SetEntityManager(&EM);

	// Define the camera to look into our 3D world
	// Y positive is down, X positive is right, Z positive is away from camera.
	// Camera position
	TheCamera.position = { 0.0f, 0.0f, -500.0f };
	// Camera looking at point
	TheCamera.target = { 0.0f, 0.0f, 0.0f };
	// Camera up vector (rotation towards target)
	TheCamera.up = { 0.0f, -1.0f, 0.0f };
	//Camera/World space is screen space.
	TheCamera.fovy = (float)GetScreenHeight();
	// Camera mode type
	TheCamera.projection = CAMERA_ORTHOGRAPHIC;
	// The Managers needs a reference to The Camera
	EM.SetCamera(TheCamera);

	game.Initialize();
	EM.Initialize();
	FM.Initialize();
	game.Load();
	game.BeginRun();
	EM.BeginRun();
	FM.BeginRun();

	while (!WindowShouldClose())
	{
		game.Input();
		float deltaTime = GetFrameTime();

#if _DEBUG
		if (deltaTime > 0.05f) deltaTime = 0.05f;
#endif
		EM.AlwaysUpdate(deltaTime);

		if (game.Logic->State != GameState::Pause)
		{
			EM.Input();

			EM.Update(deltaTime);
			game.Update(deltaTime);
			EM.FixedUpdate(deltaTime);
			game.FixedUpdate(deltaTime);

			if (IsKeyPressed(KEY_P) || (IsGamepadAvailable(0)
				&& IsGamepadButtonPressed(0, 13)))
			{
				game.Logic->State = Pause;
			}
		}
		else
		{
			if (IsKeyPressed(KEY_P) || (IsGamepadAvailable(0)
				&& IsGamepadButtonPressed(0, 13)))
			{
				game.Logic->State = InPlay;
			}
		}

		BeginDrawing();
		//ClearBackground({ 8, 2, 16, 100 });
		BeginMode3D(TheCamera);
		EM.Draw3D();
		game.Draw3D();
		EndMode3D();
		EM.Draw2D();
		game.Draw2D();
		Score.Draw2D();
		HighScore.Draw2D();

#ifdef _DEBUG
		DrawFPS(5, 5);
#endif
		EndDrawing();
	}

	UnloadImage(icon);

	CloseWindow();
	CloseAudioDevice();
}