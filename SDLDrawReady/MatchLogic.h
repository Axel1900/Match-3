#pragma once
// ============================================================
// MatchLogic.h  -  TODA la logica del tablero
// ------------------------------------------------------------
// CERO SDL aqui. Este archivo solo sabe de numeros:
// que comida hay en cada casilla, cuales hacen match, etc.
#include "Grid.h" 

const int FILAS = 8;
const int COLUMNAS = 8;
const int TIPOS_DE_GEMA = 5; 

struct Gema
{
	int tipo;  
	bool marcada;  
	float caida; 
};

class Tablero
{
private:
	Grid<Gema>* celdas;

	void Marcar(int fila, int col);

public:
	Tablero();
	~Tablero();

	Gema GetGema(int fila, int col);

	void Llenar();
	bool SonVecinas(int fila1, int col1, int fila2, int col2);
	void Intercambiar(int fila1, int col1, int fila2, int col2);

	int  MarcarMatches(); 
	int  ContarMarcadas();
	void QuitarMarcadasYBajar();  

	bool HayAnimacion();
	void AvanzarAnimacion(float velocidad);
};
