#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Text.h"
#include "Boton.h"

class Gameplay;

class Pausa : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;
	Gameplay* juego; 

	Text* titulo;
	float tituloX;
	Boton botonContinuar;
	Boton botonReiniciar;
	Boton botonMenu;
	Boton botonSalir;

public:
	Pausa(Gameplay* juego);
	~Pausa();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;
};
