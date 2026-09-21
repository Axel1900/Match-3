#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Text.h"
#include "Boton.h"

class Gameplay;

class GameOver : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;
	Gameplay* juego;

	Text* titulo;
	Text* textoPuntaje;
	float tituloX;
	float textoPuntajeX;
	Boton botonReiniciar;
	Boton botonMenu;
	Boton botonSalir;

public:
	GameOver(Gameplay* juego);
	~GameOver();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;
};
