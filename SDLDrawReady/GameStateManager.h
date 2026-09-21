#pragma once
#include "GameState.h"
#include "Escena.h"
#include "Platform.h"
#include "Stack.h"

class GameState;

class GameStateManager
{
private:
	Stack<Escena*> states;   
	Platform* platform;

	Escena* pushPendiente;
	int popsPendientes;

	void AplicarCambiosPendientes();

public:
	GameStateManager();
	~GameStateManager();
	void GameLoop();
	void SetState(Escena* state); 
	void RealaseState();     

	void PedirPush(Escena* state); 
	void PedirPop(int cuantas);  
	void PedirSalir();  
};
