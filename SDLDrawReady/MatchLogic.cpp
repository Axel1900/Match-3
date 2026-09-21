#include "MatchLogic.h"
#include <cstdlib>

Tablero::Tablero()
{
	celdas = new Grid<Gema>(FILAS, COLUMNAS);
	Llenar();
}

Tablero::~Tablero()
{
	delete celdas;
	celdas = nullptr;
}

Gema Tablero::GetGema(int fila, int col)
{
	return celdas->Get(fila, col);
}

void Tablero::Llenar()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g;
			g.tipo = rand() % TIPOS_DE_GEMA;
			celdas->Set(fila, col, g);
		}
	}
}
