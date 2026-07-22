#include "../include/ElementoNodo.h"
#include "../include/Nodo.h"

ElementoNodo::ElementoNodo(Nodo* nodo)
    : nodo(nodo),
      siguiente(nullptr)
{
}

ElementoNodo::~ElementoNodo(){
    delete nodo;
    nodo = nullptr;
}

Nodo* ElementoNodo::getNodo() const{
    return nodo;
}

ElementoNodo* ElementoNodo::getSiguiente() const{
    return siguiente;
}

void ElementoNodo::setNodo(Nodo* nodo){
    this->nodo = nodo;
}

void ElementoNodo::setSiguiente(ElementoNodo* siguiente){
    this->siguiente = siguiente;
}