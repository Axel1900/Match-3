#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Image.h"
#include "Text.h"
#include "Boton.h"

class Menu : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;

	Image fondo;
	Text* titulo;
	Text* subtitulo;
	float tituloX;     
	float subtituloX;
	Boton botonJugar;
	Boton botonSalir;

public:
	Menu();
	~Menu();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;
};
