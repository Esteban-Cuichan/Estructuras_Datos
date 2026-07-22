#include "../include/Arista.h"

Arista::Arista(Nodo* destino, double distancia, double tiempo)
    : destino(destino),
      distancia(distancia),
      tiempo(tiempo)
{
}

Nodo* Arista::getDestino() const{
    return destino;
}

double Arista::getDistancia() const{
    return distancia;
}

double Arista::getTiempo() const{
    return tiempo;
}

void Arista::setDistancia(double distancia){
    this->distancia = distancia;
}

void Arista::setTiempo(double tiempo){
    this->tiempo = tiempo;
}