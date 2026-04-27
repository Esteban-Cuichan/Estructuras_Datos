#include "Nodo.h"
#include <iostream>
using namespace std;


Nodo::Nodo(string cedula, string nombre, Nodo* siguiente, Nodo* anterior){
    this->cedula=cedula;
    this->nombre=nombre;
    this->siguiente=siguiente;
    this->anterior=anterior;
}

Nodo::Nodo(){
    this->cedula="";
    this->nombre="";
    this->siguiente=nullptr;
    this->anterior=nullptr;
}

void Nodo::setCedula(string cedula){
    this->cedula=cedula;
}

void Nodo::setNombre(string nombre){
    this->nombre=nombre;
}

void Nodo::setSiguiente(Nodo* siguiente){
    this->siguiente=siguiente;
}

void Nodo::setAnterior(Nodo* anterior){
    this->anterior=anterior;
}

string Nodo::getCedula(){
    return this->cedula;
}

string Nodo::getNombre(){
    return this->nombre;
}

Nodo* Nodo::getSiguiente(){
    return this->siguiente;
}

Nodo* Nodo::getAnterior(){
    return this->anterior;
}