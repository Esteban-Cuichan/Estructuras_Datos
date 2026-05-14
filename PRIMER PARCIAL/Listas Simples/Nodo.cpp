#include "Nodo.h"
#include <iostream>
using namespace std;


Nodo::Nodo(string cedula, string nombre, Nodo* siguiente,string apellido){
    this->cedula=cedula;
    this->nombre=nombre;
    this->apellido=apellido;
    this->siguiente=siguiente;
}

Nodo::Nodo(){
    this->cedula="";
    this->nombre="";
    this->apellido="";
    this->siguiente=nullptr;
}

void Nodo::setCedula(string cedula){
    this->cedula=cedula;
}

void Nodo::setNombre(string nombre){
    this->nombre=nombre;
}

void Nodo::setApellido(string apellido){
    this->apellido=apellido;
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

string Nodo::getApellido(){
    return this->apellido;
}

Nodo* Nodo::getSiguiente(){
    return this->siguiente;
}