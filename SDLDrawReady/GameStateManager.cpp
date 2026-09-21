#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager()
{
	platform = new Platform("Match-3");

	SDL_Window* ventana = SDL_GetRenderWindow(Platform::renderer);
	SDL_SetWindowSize(ventana, 740, 480);
	SDL_SetWindowPosition(ventana, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	platform->width = 740;
	platform->height = 480;
	pushPendiente = nullptr;
	popsPendientes = 0;
}

GameStateManager::~GameStateManager()
{
	while (!states.IsEmpty())
	{
		RealaseState();
	}
	delete pushPendiente; 
	delete platform;
}

void GameStateManager::GameLoop()
{
	while (true)
	{
		if (states.IsEmpty())
		{
			break;
		}

		Escena* state = states.Top();
		platform->CheckEvent(state, &GameState::Input);
		state->Update();
		state->Draw();

		AplicarCambiosPendientes();
	}
}

void GameStateManager::AplicarCambiosPendientes()
{
	while (popsPendientes > 0)
	{
		RealaseState();
		popsPendientes--;
	}

	if (pushPendiente != nullptr)
	{
		SetState(pushPendiente);
		pushPendiente = nullptr;
	}
}

void GameStateManager::SetState(Escena* state)
{
	state->Init(platform, this);
	states.Push(state); 
}

void GameStateManager::RealaseState()
{
	if (states.IsEmpty()) return;

	Escena* escena = states.Pop();  
	escena->Close();
	delete escena;
}

void GameStateManager::PedirPush(Escena* state)
{
	if (pushPendiente != nullptr)
	{
		delete state;
		return;
	}
	pushPendiente = state;
}

void GameStateManager::PedirPop(int cuantas)
{
	popsPendientes = popsPendientes + cuantas;
}

void GameStateManager::PedirSalir()
{
	popsPendientes = 1000; 
}
