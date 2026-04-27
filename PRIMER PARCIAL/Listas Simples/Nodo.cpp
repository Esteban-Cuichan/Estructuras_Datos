#include "Nodo.h"
#include <iostream>
using namespace std;


Nodo::Nodo(string cedula, string nombre, Nodo* siguiente){
    this->cedula=cedula;
    this->nombre=nombre;
    this->siguiente=siguiente;
}

Nodo::Nodo(){
    this->cedula="";
    this->nombre="";
    this->siguiente=nullptr;
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

string Nodo::getCedula(){
    return this->cedula;
}

string Nodo::getNombre(){
    return this->nombre;
}

Nodo* Nodo::getSiguiente(){
    return this->siguiente;
}