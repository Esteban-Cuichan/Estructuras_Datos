#include "Nodo.h"

Nodo::Nodo(){
    this->siguiente=nullptr;
    this->anterior=nullptr;
}

Nodo::Nodo(Nodo* siguiente, Nodo* anterior){
    this->siguiente=siguiente;
    this->anterior=anterior;
}

Nodo* Nodo::getSiguiente(){
    return siguiente;
}

Nodo* Nodo::getAnterior(){
    return anterior;
}

Turno* Nodo::getTurno(){
    return turno;
}

void Nodo::setSiguiente(Nodo* siguiente){
    this->siguiente=siguiente;
}

void Nodo::setAnterior(Nodo* anterior){
    this->anterior=anterior;
}

void Nodo::setTurno(Turno* turno){
    this->turno=turno;
}