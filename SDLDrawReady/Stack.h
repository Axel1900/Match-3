#pragma once
#include "TNode.h"
#include <iostream>

// =====================================================================
//  Stack<T>  -  Pila generica (LIFO)
//
//  El tope es la CABEZA de la lista: cada nodo apunta hacia ABAJO.
//  Gracias a eso, Push y Pop son O(1): nunca hay que recorrer nada.
//
//  TODO: implementa todo lo que dice TODO.
// =====================================================================

template <class T>
class Stack
{
private:
    TNode<T>* _top;
    int _size;

    // Privada a proposito: si alguien de afuera pudiera pasar un nodo,
    // no quedaria claro quien es dueño de esa memoria ni quien la libera.
    // Desde afuera solo se puede usar Push(T value).
    void Push(TNode<T>* n);

public:
    Stack();
    Stack(const Stack<T>& otro);              // constructor de copia
    Stack<T>& operator=(const Stack<T>& otro); // operador de asignacion
    ~Stack();

    void Push(T value);
    T Pop();
    T Top();
    bool IsEmpty();
    int GetSize();
    void Clear();
    void Print();
};

template <class T>
Stack<T>::Stack()
{
    _top = nullptr;
    _size = 0;
}

template <class T>
Stack<T>::Stack(const Stack<T>& otro)
{

    _top = nullptr;
    _size = 0;

    Stack<T> temp;
    TNode<T>* actual = otro._top;
    while (actual != nullptr)
    {
        temp.Push(actual->getDato());
        actual = actual->getNext();
    }
    while (!temp.IsEmpty())
    {
        this->Push(temp.Pop());
    }
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& otro)
{
    if (this == &otro)
        return *this;

    Clear();

    Stack<T> temp;
    TNode<T>* actual = otro._top;
    while (actual != nullptr)
    {
        temp.Push(actual->getDato());
        actual = actual->getNext();
    }
    while (!temp.IsEmpty())
    {
        this->Push(temp.Pop());
    }

    return *this;
}

template <class T>
Stack<T>::~Stack()
{
    Clear();
}

template <class T>
void Stack<T>::Push(TNode<T>* n)
{
    n->setNext(_top);
    _top = n;
    _size++;
}

template <class T>
void Stack<T>::Push(T value)
{
    TNode<T>* nuevo = new TNode<T>(value, _size);
    Push(nuevo);
}

template <class T>
T Stack<T>::Pop()
{
    TNode<T>* nodo = _top;
    T dato = nodo->getDato();
    _top = nodo->getNext();
    _size--;
    delete nodo;
    return dato;
}

template <class T>
T Stack<T>::Top()
{
    return _top->getDato();
}

template <class T>
bool Stack<T>::IsEmpty()
{
    return _top == nullptr;
}

template <class T>
int Stack<T>::GetSize()
{
    return _size;
}

template <class T>
void Stack<T>::Clear()
{
    while (_top != nullptr)
    {
        TNode<T>* nodo = _top;
        _top = nodo->getNext();
        delete nodo;
    }
    _size = 0;
}

template <class T>
void Stack<T>::Print()
{
    TNode<T>* actual = _top;
    while (actual != nullptr)
    {
        std::cout << actual->getDato() << " ";
        actual = actual->getNext();
    }
    std::cout << std::endl;
}