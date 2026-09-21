#pragma once
#include "Grid.h"

const int FILAS = 8;
const int COLUMNAS = 8;
const int TIPOS_DE_GEMA = 5;

struct Gema
{
	int tipo;
	bool esBomba;
	bool marcada; 
	float caida; 
};

inline bool operator==(Gema a, Gema b)
{
	return a.tipo == b.tipo;
}

inline bool operator!=(Gema a, Gema b)
{
	return a.tipo != b.tipo;
}

class Tablero
{
private:
	Grid<Gema>* celdas;

	void Marcar(int fila, int col);
	void CambiarTipo(int fila, int col, int nuevoTipo);

public:
	Tablero();
	~Tablero();

	Gema GetGema(int fila, int col);

	void SetGema(int fila, int col, Gema gema);

	void Llenar();                      
	bool SonVecinas(int fila1, int col1, int fila2, int col2);
	void Intercambiar(int fila1, int col1, int fila2, int col2);

	int  MarcarMatches(bool crearBombas);  
	void ExplotarBomba(int fila, int col);
	int  ContarMarcadas();
	void LimpiarMarcas();
	void QuitarMarcadasYBajar();  

	bool HayMovimientoPosible();

	bool HayAnimacion();
	void AvanzarAnimacion(float velocidad);
};
