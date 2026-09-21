#pragma once
#include <iostream>

// =====================================================================
//  Grid<T>  -  Rejilla generica (arreglo 2D dinamico encapsulado)
//
//  Esta es la version bien hecha de la matriz que ya programaste antes
//  con punteros sueltos: ahora la memoria vive encapsulada dentro de
//  una clase que se encarga de reservarla y liberarla.
//
//  Y no es un ejercicio de escuela: esta rejilla es literalmente el
//  tablero de tu Match-3.
//
//  FLOOD FILL: dada una celda inicial, encuentra todas las celdas
//  CONECTADAS que tengan el mismo valor, y regresa cuantas son.
//  Es recursivo y tiene tres casos base:
//     1. te saliste de los limites
//     2. la celda tiene un valor distinto al que buscas
//     3. ya habias visitado esa celda
//  Y cuatro llamadas recursivas: arriba, abajo, izquierda, derecha.
//  Las diagonales NO cuentan como vecinas.
// =====================================================================

template <class T>
class Grid
{
private:
    T** _celdas;
    bool** _visitadas;
    int _filas;
    int _columnas;

    int FloodRecursivo(int fila, int columna, T objetivo);

public:
    Grid(int filas, int columnas);
    ~Grid();

    void Set(int fila, int columna, T valor);
    T Get(int fila, int columna);
    int GetFilas();
    int GetColumnas();
    int FloodFill(int fila, int columna);
    void Print();
};

template <class T>
Grid<T>::Grid(int filas, int columnas)
{
    // Los punteros arrancan en nullptr para que nada truene antes de
    // que reserves la memoria. NO borres estas dos lineas.
    _celdas = nullptr;
    _visitadas = nullptr;
    _filas = filas;
    _columnas = columnas;

    _celdas = new T * [_filas];
    _visitadas = new bool* [_filas];
    for (int f = 0; f < _filas; f++)
    {
        _celdas[f] = new T[_columnas];
        _visitadas[f] = new bool[_columnas];

        for (int c = 0; c < _columnas; c++)
            _visitadas[f][c] = false;
    }
}

template <class T>
Grid<T>::~Grid()
{
    for (int f = 0; f < _filas; f++)
    {
        delete[] _celdas[f];
        delete[] _visitadas[f];
    }
    delete[] _celdas;
    delete[] _visitadas;
}

template <class T>
void Grid<T>::Set(int fila, int columna, T valor)
{
    if (fila < 0 || fila >= _filas || columna < 0 || columna >= _columnas)
        return;

    _celdas[fila][columna] = valor;
}

template <class T>
T Grid<T>::Get(int fila, int columna)
{
    if (fila < 0 || fila >= _filas || columna < 0 || columna >= _columnas)
        return T();

    return _celdas[fila][columna];
}

template <class T>
int Grid<T>::GetFilas()
{
    return _filas;
}

template <class T>
int Grid<T>::GetColumnas()
{
    return _columnas;
}

template <class T>
int Grid<T>::FloodFill(int fila, int columna)
{
    if (fila < 0 || fila >= _filas || columna < 0 || columna >= _columnas)
        return 0;

    for (int f = 0; f < _filas; f++)
        for (int c = 0; c < _columnas; c++)
            _visitadas[f][c] = false;

    return FloodRecursivo(fila, columna, _celdas[fila][columna]);
}

template <class T>
int Grid<T>::FloodRecursivo(int fila, int columna, T objetivo)
{
    if (fila < 0 || fila >= _filas || columna < 0 || columna >= _columnas)
        return 0;

    if (_celdas[fila][columna] != objetivo)
        return 0;

    if (_visitadas[fila][columna])
        return 0;

    _visitadas[fila][columna] = true;

    return 1
        + FloodRecursivo(fila - 1, columna, objetivo)
        + FloodRecursivo(fila + 1, columna, objetivo)
        + FloodRecursivo(fila, columna - 1, objetivo)
        + FloodRecursivo(fila, columna + 1, objetivo);
}

template <class T>
void Grid<T>::Print()
{
    for (int f = 0; f < _filas; f++)
    {
        for (int c = 0; c < _columnas; c++)
        {
            std::cout << _celdas[f][c] << "\t";
        }
        std::cout << std::endl;
    }
}