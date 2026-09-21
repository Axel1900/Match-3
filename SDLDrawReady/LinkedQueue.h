#pragma once
#include "TNode.h"
#include <iostream>

// =====================================================================
//  LinkedQueue<T>  -  Cola generica (FIFO) sobre nodos enlazados
//
//  Se opera por los DOS extremos: se encola por atras (_last) y se
//  desencola por adelante (_first). Por eso necesita los dos punteros:
//  sin _last, cada Enqueue tendria que recorrer toda la cola.
//
//  CUIDADO con el bug clasico: cuando sacas el ULTIMO elemento,
//  _first queda en nullptr pero _last se queda apuntando a un nodo
//  que ya liberaste. El siguiente Enqueue escribe sobre memoria muerta.
// =====================================================================

template <class T>
class LinkedQueue
{
private:
    TNode<T>* _first;
    TNode<T>* _last;
    int _size;

public:
    LinkedQueue();
    ~LinkedQueue();

    void Enqueue(T value);
    T Dequeue();
    T Front();
    bool IsEmpty();
    int GetSize();
    void Clear();
    void Print();
};

template <class T>
LinkedQueue<T>::LinkedQueue()
{
    _first = nullptr;
    _last = nullptr;
    _size = 0;
}

template <class T>
LinkedQueue<T>::~LinkedQueue()
{
    Clear();
}

template <class T>
void LinkedQueue<T>::Enqueue(T value)
{
    TNode<T>* nuevo = new TNode<T>(value, _size);

    if (_first == nullptr)
    {
        _first = nuevo;
        _last = nuevo;
    }
    else
    {
        _last->setNext(nuevo);
        _last = nuevo;
    }
    _size++;
}

template <class T>
T LinkedQueue<T>::Dequeue()
{
    TNode<T>* nodo = _first;
    T dato = nodo->getDato();

    _first = nodo->getNext();

    if (_first == nullptr)
        _last = nullptr;

    _size--;
    delete nodo;
    return dato;
}

template <class T>
T LinkedQueue<T>::Front()
{
    return _first->getDato();
}

template <class T>
bool LinkedQueue<T>::IsEmpty()
{
    return _first == nullptr;
}

template <class T>
int LinkedQueue<T>::GetSize()
{
    return _size;
}

template <class T>
void LinkedQueue<T>::Clear()
{
    while (_first != nullptr)
    {
        TNode<T>* nodo = _first;
        _first = nodo->getNext();
        delete nodo;
    }
    _last = nullptr;
    _size = 0;
}

template <class T>
void LinkedQueue<T>::Print()
{
    TNode<T>* actual = _first;
    while (actual != nullptr)
    {
        std::cout << actual->getDato() << " ";
        actual = actual->getNext();
    }
    std::cout << std::endl;
}