#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Image.h"
#include "Text.h"
#include "MatchLogic.h"

const float TAM = 54;       
const float TABLERO_X = 48;
const float TABLERO_Y = 24;

class Gameplay : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;

	Tablero* tablero;  

	Image fondo;
	Image gemas[TIPOS_DE_GEMA];

	Text* textoAyuda;

public:
	Gameplay();
	~Gameplay();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;
};
