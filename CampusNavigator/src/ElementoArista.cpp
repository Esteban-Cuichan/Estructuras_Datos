#include "../include/ElementoArista.h"
#include "../include/Arista.h"

ElementoArista::ElementoArista(Arista* arista)
    : arista(arista),
      siguiente(nullptr)
{
}

ElementoArista::~ElementoArista(){
    delete arista;
    arista = nullptr;
}

Arista* ElementoArista::getArista() const{
    return arista;
}

ElementoArista* ElementoArista::getSiguiente() const{
    return siguiente;
}

void ElementoArista::setArista(Arista* arista){
    this->arista = arista;
}

void ElementoArista::setSiguiente(ElementoArista* siguiente){
    this->siguiente = siguiente;
}