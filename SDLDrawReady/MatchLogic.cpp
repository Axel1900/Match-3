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

void Tablero::SetGema(int fila, int col, Gema gema)
{
	celdas->Set(fila, col, gema);
}

void Tablero::Marcar(int fila, int col)
{
	Gema g = celdas->Get(fila, col);
	g.marcada = true;
	celdas->Set(fila, col, g);
}

void Tablero::CambiarTipo(int fila, int col, int nuevoTipo)
{
	Gema g = celdas->Get(fila, col);
	g.tipo = nuevoTipo;
	celdas->Set(fila, col, g);
}

void Tablero::Llenar()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g;
			g.tipo = rand() % TIPOS_DE_GEMA;
			g.esBomba = false;
			g.marcada = false;
			g.caida = 0;
			celdas->Set(fila, col, g);
		}
	}

	int marcadas = MarcarMatches(false);
	while (marcadas > 0)
	{
		for (int fila = 0; fila < FILAS; fila++)
		{
			for (int col = 0; col < COLUMNAS; col++)
			{
				Gema g = celdas->Get(fila, col);
				if (g.marcada)
				{
					g.tipo = rand() % TIPOS_DE_GEMA;
					g.marcada = false;
					celdas->Set(fila, col, g);
				}
			}
		}
		marcadas = MarcarMatches(false);
	}

	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = celdas->Get(fila, col);
			g.caida = FILAS;
			celdas->Set(fila, col, g);
		}
	}
}

bool Tablero::SonVecinas(int fila1, int col1, int fila2, int col2)
{
	int distancia = abs(fila1 - fila2) + abs(col1 - col2);
	return distancia == 1;
}

void Tablero::Intercambiar(int fila1, int col1, int fila2, int col2)
{
	Gema primera = celdas->Get(fila1, col1);
	Gema segunda = celdas->Get(fila2, col2);
	celdas->Set(fila1, col1, segunda);
	celdas->Set(fila2, col2, primera);
}

int Tablero::MarcarMatches(bool crearBombas)
{
	int bombaFila = -1;
	int bombaCol = -1;

	for (int fila = 0; fila < FILAS; fila++)
	{
		int contador = 1;
		for (int col = 1; col <= COLUMNAS; col++)
		{
			bool esIgual = false;
			if (col < COLUMNAS)
			{
				if (celdas->Get(fila, col).tipo == celdas->Get(fila, col - 1).tipo)
				{
					esIgual = true;
				}
			}

			if (esIgual)
			{
				contador++;
			}
			else
			{
				if (contador >= 3)
				{
					int inicio = col - contador;
					for (int k = inicio; k < col; k++)
					{
						Marcar(fila, k);
					}
					if (contador >= 4)
					{
						bombaFila = fila;
						bombaCol = inicio + contador / 2;
					}
				}
				contador = 1;
			}
		}
	}

	for (int col = 0; col < COLUMNAS; col++)
	{
		int contador = 1;
		for (int fila = 1; fila <= FILAS; fila++)
		{
			bool esIgual = false;
			if (fila < FILAS)
			{
				if (celdas->Get(fila, col).tipo == celdas->Get(fila - 1, col).tipo)
				{
					esIgual = true;
				}
			}

			if (esIgual)
			{
				contador++;
			}
			else
			{
				if (contador >= 3)
				{
					int inicio = fila - contador;
					for (int k = inicio; k < fila; k++)
					{
						Marcar(k, col);
					}
					if (contador >= 4)
					{
						bombaFila = inicio + contador / 2;
						bombaCol = col;
					}
				}
				contador = 1;
			}
		}
	}

	if (crearBombas && bombaFila != -1)
	{
		Gema g = celdas->Get(bombaFila, bombaCol);
		g.marcada = false;
		g.esBomba = true;
		celdas->Set(bombaFila, bombaCol, g);
	}

	return ContarMarcadas();
}

void Tablero::ExplotarBomba(int fila, int col)
{
	celdas->FloodFill(fila, col);

	for (int f = 0; f < FILAS; f++)
	{
		for (int c = 0; c < COLUMNAS; c++)
		{
			if (celdas->FueVisitada(f, c))
			{
				Marcar(f, c);
			}
		}
	}
}

int Tablero::ContarMarcadas()
{
	int total = 0;
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			if (celdas->Get(fila, col).marcada)
			{
				total++;
			}
		}
	}
	return total;
}

void Tablero::LimpiarMarcas()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = celdas->Get(fila, col);
			g.marcada = false;
			celdas->Set(fila, col, g);
		}
	}
}

void Tablero::QuitarMarcadasYBajar()
{
	for (int col = 0; col < COLUMNAS; col++)
	{
		int filaDestino = FILAS - 1;

		for (int fila = FILAS - 1; fila >= 0; fila--)
		{
			Gema g = celdas->Get(fila, col);
			if (!g.marcada)
			{
				if (fila != filaDestino)
				{
					g.caida = (float)(filaDestino - fila);
					celdas->Set(filaDestino, col, g);
				}
				filaDestino--;
			}
		}

		int cuantasNuevas = filaDestino + 1;
		for (int fila = filaDestino; fila >= 0; fila--)
		{
			Gema nueva;
			nueva.tipo = rand() % TIPOS_DE_GEMA;
			nueva.esBomba = false;
			nueva.marcada = false;
			nueva.caida = (float)cuantasNuevas;
			celdas->Set(fila, col, nueva);
		}
	}
}

bool Tablero::HayMovimientoPosible()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			if (celdas->Get(fila, col).esBomba) return true; 

			if (col + 1 < COLUMNAS)
			{
				Intercambiar(fila, col, fila, col + 1);
				int n = MarcarMatches(false);
				LimpiarMarcas();
				Intercambiar(fila, col, fila, col + 1);
				if (n > 0) return true;
			}

			if (fila + 1 < FILAS)
			{
				Intercambiar(fila, col, fila + 1, col);
				int n = MarcarMatches(false);
				LimpiarMarcas();
				Intercambiar(fila, col, fila + 1, col);
				if (n > 0) return true;
			}
		}
	}
	return false;
}

bool Tablero::HayAnimacion()
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			if (celdas->Get(fila, col).caida > 0) return true;
		}
	}
	return false;
}

void Tablero::AvanzarAnimacion(float velocidad)
{
	for (int fila = 0; fila < FILAS; fila++)
	{
		for (int col = 0; col < COLUMNAS; col++)
		{
			Gema g = celdas->Get(fila, col);
			if (g.caida > 0)
			{
				g.caida = g.caida - velocidad;
				if (g.caida < 0) g.caida = 0;
				celdas->Set(fila, col, g);
			}
		}
	}
}
