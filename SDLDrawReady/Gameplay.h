#pragma once
#include "Escena.h"
#include "GameStateManager.h"
#include "Image.h"
#include "Text.h"
#include "Boton.h"
#include "MatchLogic.h"
#include "LinkedQueue.h" 
#include "RingBuffer.h"    
#include "ObjectPool.h"    

const int EVENTO_REVISAR_MATCHES = 0;  
const int EVENTO_DESTRUIR = 1; 
const int EVENTO_CAER = 2;  

const int MOVIMIENTOS_INICIALES = 20;
const int PUNTOS_POR_GEMA = 10;

const float TAM = 54; 
const float TABLERO_X = 48;
const float TABLERO_Y = 24;
const float TAM_GEMA = 49; 

const int JUGADAS_PARA_DESHACER = 5;   
const int MAX_PARTICULAS = 400;    
const int PARTICULAS_POR_GEMA = 6;

struct Jugada
{
	Gema gemas[FILAS][COLUMNAS];
	int puntaje;
	int movimientos;
};

struct Particula
{
	float x, y; 
	float vx, vy; 
	float giro; 
	int tipo;  
	int vida;  
};

class Gameplay : public Escena
{
private:
	Platform* platform;
	GameStateManager* manager;

	Tablero* tablero;              
	LinkedQueue<int> eventos;     
	RingBuffer<Jugada>* historial; 
	ObjectPool<Particula>* particulas;  

	Image fondo;
	Image gemas[TIPOS_DE_GEMA];
	Image cursor;

	Text* tituloPuntos;
	Text* textoPuntos;
	Text* tituloMovimientos;
	Text* textoMovimientos;
	Text* textoAyuda;
	Boton botonDeshacer;
	int puntosMostrados;
	int movimientosMostrados;
	int deshacerMostrados;

	int puntaje;
	int movimientos;
	int combo;
	bool yaSeAcabo;

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

	void GuardarJugada();
	void Deshacer();
	void CrearParticulas();
	void ActualizarParticulas();
	void DibujarParticulas();

public:
	Gameplay();
	~Gameplay();
	void Init(Platform* platform, GameStateManager* manager) override;
	void Draw() override;
	bool Input(ListaT<int>* keyDowns, ListaT<int>* keyUps, bool* leftclick, float* mouseX, float* mouseY) override;
	void Update() override;
	void Close() override;

	void DibujarEscena();  
	void Reiniciar();
	int GetPuntaje();
};
