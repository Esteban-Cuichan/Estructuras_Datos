#include "../include/Nodo.h"
#include "../include/ListaAristas.h"

Nodo::Nodo(int id, const string& nombre)
    : id(id),
      nombre(nombre),
      visitado(false),
      tiempoAcumulado(0.0),
      distanciaAcumulada(0.0),
      anterior(nullptr),
      aristas(new ListaAristas())
{
}

Nodo::~Nodo(){
    delete aristas;
    aristas = nullptr;
}

int Nodo::getId() const{
    return id;
}

string Nodo::getNombre() const{
    return nombre;
}

bool Nodo::estaVisitado() const{
    return visitado;
}

void Nodo::setVisitado(bool estado){
    visitado = estado;
}

double Nodo::getTiempoAcumulado() const{
    return tiempoAcumulado;
}

void Nodo::setTiempoAcumulado(double tiempo){
    tiempoAcumulado = tiempo;
}

double Nodo::getDistanciaAcumulada() const{
    return distanciaAcumulada;
}

void Nodo::setDistanciaAcumulada(double distancia){
    distanciaAcumulada = distancia;
}

Nodo* Nodo::getAnterior() const{
    return anterior;
}

void Nodo::setAnterior(Nodo* nodo){
    anterior = nodo;
}

ListaAristas* Nodo::getConexiones() const{
    return aristas;
}

void Nodo::reiniciar(){
    visitado = false;
    tiempoAcumulado = 0.0;
    distanciaAcumulada = 0.0;
    anterior = nullptr;
}