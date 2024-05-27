//External includes
#ifdef DEBUG
#include "vld.h" // only need in debug
#endif // DEBUG

#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"
#include "GameScenes.h"

using namespace dae;

int main(/*int argc, char* args[]*/)
{
	// waarom meegeven als je toch weg doet? //
	// 
	//Unreferenced parameters
	//(void)argc;
	//(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	constexpr uint32_t width{ 640 };
	constexpr uint32_t height{ 480 };

	SDL_Window* pWindow
	{
		SDL_CreateWindow(
			"RayTracer - **Maurice Vandenheede(2DAE18N)**",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height, 0)
	};

	if (!pWindow) return 1;

	//Initialize "framework"
	Timer* pTimer = new Timer{};
	Renderer* pRenderer = new Renderer{ pWindow, width, height };

	//Scene_W1* pScene{ new Scene_W1{} };
	//Scene_W2* pScene{ new Scene_W2{} };
	//Scene_W3_TestScene* pScene{ new Scene_W3_TestScene{} };
	//Scene_W3* pScene{ new Scene_W3{} };
	//Scene_W4_TestScene* pScene{ new Scene_W4_TestScene{} };
	Scene_W4_ReferenceScene* pScene{ new Scene_W4_ReferenceScene{} };
	//Scene_W4_BunnyScene* pScene{ new Scene_W4_BunnyScene{} };
	//Scene_W4_Extra* pScene{ new Scene_W4_Extra{} };
	pScene->Initialize();

	//Start loop
	pTimer->Start();

	float printTimer{};
	bool showFPS{ true };
	bool isLooping{ true };
	bool takeScreenshot{ false };

	SDL_Event e{};

	while (isLooping)
	{
		//--------- Get input events ---------//
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_X:
					takeScreenshot = true;
					break;

				case SDL_SCANCODE_F2:
					pRenderer->ToggleShadows();
					break;

				case SDL_SCANCODE_F3:
					pRenderer->CycleLightingMode();
					break;

				case SDL_SCANCODE_F6:
					pTimer->StartBenchmark();
					break;

				case SDL_SCANCODE_F:
					showFPS = !showFPS;
					break;

				default:
					break;
				}
				break;
			}
		}

		//--------- Update ---------//
		pScene->Update(pTimer);

		//--------- Render ---------//
		pRenderer->Render(pScene);

		//--------- Timer ---------//
		pTimer->Update();

		// fps
		if (showFPS)
		{
			printTimer += pTimer->GetElapsed();
			if (printTimer >= 1.f)
			{
				printTimer = 0.f;
				std::cout << "dFPS: " << pTimer->GetdFPS() << "\n";
			}
		}

		// screenShot
		if (takeScreenshot)
		{
			takeScreenshot = false;
			if (pRenderer->SaveBufferToImage())
			{
				std::cout << "Something went wrong. Screenshot not saved!" << "\n";	
			}
			else
			{
				std::cout << "Screenshot saved!" << "\n";
			}
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pScene;
	delete pRenderer;
	delete pTimer;

	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}