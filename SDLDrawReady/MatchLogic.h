#pragma once
#include "Grid.h"

const int FILAS = 8;
const int COLUMNAS = 8;
const int TIPOS_DE_GEMA = 5;

struct Gema
{
	int tipo;
};

class Tablero
{
private:
	Grid<Gema>* celdas;

public:
	Tablero();
	~Tablero();

	Gema GetGema(int fila, int col);

	void Llenar();
};
