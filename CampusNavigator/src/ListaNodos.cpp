#include "../include/ListaNodos.h"
#include "../include/ElementoNodo.h"
#include "../include/Nodo.h"

ListaNodos::ListaNodos()
    : primero(nullptr),
      ultimo(nullptr),
      cantidad(0)
{
}

ListaNodos::~ListaNodos(){
    vaciar();
}

void ListaNodos::insertar(Nodo* nodo)
{
    if (nodo == nullptr){
        return;
    }

    ElementoNodo* nuevoElemento =
        new ElementoNodo(nodo);

    if (primero == nullptr){
        primero = nuevoElemento;
        ultimo = nuevoElemento;
    }else{
        ultimo->setSiguiente(nuevoElemento);
        ultimo = nuevoElemento;
    }

    cantidad++;
}

Nodo* ListaNodos::buscarPorId(int id) const{
    ElementoNodo* actual = primero;

    while (actual != nullptr){
        Nodo* nodoActual = actual->getNodo();

        if (nodoActual != nullptr && nodoActual->getId() == id){
            return nodoActual;
        }

        actual = actual->getSiguiente();
    }

    return nullptr;
}

ElementoNodo* ListaNodos::getPrimero() const{
    return primero;
}

ElementoNodo* ListaNodos::getUltimo() const{
    return ultimo;
}

int ListaNodos::getCantidad() const{
    return cantidad;
}

bool ListaNodos::estaVacia() const{
    return primero == nullptr;
}

void ListaNodos::vaciar(){
    ElementoNodo* actual = primero;

    while (actual != nullptr)
    {
        ElementoNodo* siguiente =
            actual->getSiguiente();

        delete actual;
        actual = siguiente;
    }

    primero = nullptr;
    ultimo = nullptr;
    cantidad = 0;
}