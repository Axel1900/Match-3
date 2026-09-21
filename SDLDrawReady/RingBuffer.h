#pragma once

template <class T>
class RingBuffer
{
private:
	T* _datos;
	int _capacidad;
	int _inicio;
	int _cantidad;

public:
	RingBuffer(int capacidad)
	{
		_capacidad = capacidad;
		_datos = new T[_capacidad];   

		_inicio = 0;
		_cantidad = 0;
	}

	~RingBuffer()
	{
		delete[] _datos;
		_datos = nullptr;
	}

	RingBuffer(const RingBuffer& otro) = delete;
	RingBuffer& operator=(const RingBuffer& otro) = delete;

	void PushBack(T valor)
	{
		if (_cantidad < _capacidad)
		{
			int posicion = (_inicio + _cantidad) % _capacidad;
			_datos[posicion] = valor;
			_cantidad++;
		}
		else
		{
			_datos[_inicio] = valor;
			_inicio = (_inicio + 1) % _capacidad;
		}
	}

	T PopBack()
	{
		_cantidad--;
		int posicion = (_inicio + _cantidad) % _capacidad;
		return _datos[posicion];
	}

	bool IsEmpty()
	{
		return _cantidad == 0;
	}

	bool IsFull()
	{
		return _cantidad == _capacidad;
	}

	int GetSize()
	{
		return _cantidad;
	}

	void Clear()
	{
		_inicio = 0;
		_cantidad = 0;
	}
};
