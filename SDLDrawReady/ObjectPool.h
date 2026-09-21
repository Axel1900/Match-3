#pragma once
#include "Stack.h" 


template <class T>
class ObjectPool
{
private:
	T* _objetos;    
	bool* _enUso;    
	int _capacidad;
	Stack<int> _libres;  

public:
	ObjectPool(int capacidad)
	{
		_capacidad = capacidad;
		_objetos = new T[_capacidad];
		_enUso = new bool[_capacidad];

		for (int i = _capacidad - 1; i >= 0; i--)
		{
			_enUso[i] = false;
			_libres.Push(i);  
		}
	}

	~ObjectPool()
	{
		delete[] _objetos;
		delete[] _enUso;
	}

	ObjectPool(const ObjectPool& otro) = delete;
	ObjectPool& operator=(const ObjectPool& otro) = delete;

	int Obtener()
	{
		if (_libres.IsEmpty())
		{
			return -1;
		}
		int indice = _libres.Pop();
		_enUso[indice] = true;
		return indice;
	}

	void Liberar(int indice)
	{
		if (indice < 0 || indice >= _capacidad) return;
		if (!_enUso[indice]) return; 

		_enUso[indice] = false;
		_libres.Push(indice);
	}

	void LiberarTodos()
	{
		for (int i = 0; i < _capacidad; i++)
		{
			Liberar(i);
		}
	}

	T& Get(int indice)
	{
		return _objetos[indice];
	}

	bool EstaEnUso(int indice)
	{
		return _enUso[indice];
	}

	int GetCapacidad()
	{
		return _capacidad;
	}

	int CuantosEnUso()
	{
		return _capacidad - _libres.GetSize();
	}
};
