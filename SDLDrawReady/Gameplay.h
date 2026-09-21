#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Image.h"
#include "Text.h"
#include "MatchLogic.h"
#include "LinkedQueue.h"

const int EVENTO_REVISAR_MATCHES = 0;
const int EVENTO_DESTRUIR = 1;    
const int EVENTO_CAER = 2;   

const int PUNTOS_POR_GEMA = 10;


const float TAM = 54;        
const float TABLERO_X = 48;
const float TABLERO_Y = 24;

class Gameplay : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;

	Tablero* tablero;   
	LinkedQueue<int> eventos; 

	Image fondo;
	Image gemas[TIPOS_DE_GEMA];
	Image cursor;

	Text* tituloPuntos;
	Text* textoPuntos;
	Text* textoAyuda;
	int puntosMostrados;

	int puntaje;
	int combo;   

	bool haySeleccion;
	int selFila;
	int selCol;

	bool vieneDeSwap;
	int swapFila1, swapCol1, swapFila2, swapCol2;

	int framesEspera;   

	bool TableroQuieto();
	void ClickEnCelda(int fila, int col);
	void ProcesarSiguienteEvento();
	void ActualizarTextos();

public:
	Gameplay();
	~Gameplay();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;
};
