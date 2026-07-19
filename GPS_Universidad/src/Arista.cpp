#include "../include/Arista.h"
#include "../include/Nodo.h"
#include <cstdio>
#include <cstring>

Arista::Arista(int id,Nodo* origen,Nodo* destino,double distancia,TipoCamino tipo)
            :id(id),origen(origen),destino(destino),distancia(distancia),tipo(tipo),estado(EstadoCamino::ABIERTO){
                if(origen==nullptr || destino==nullptr){

                }
}

Arista::~Arista(){

}

int Arista::getId() const{
    return this->id;
}

Nodo* Arista::getOrigen() const {
    return this->origen;
}

void Arista::setOrigen(Nodo* origen){
    if(origen!=nullptr){
        this->origen=origen;
    }
}

Nodo* Arista::getDestino() const{
    return this->destino;
}

void Arista::setDestino(Nodo* destino){
    if(destino!=nullptr){
        this->destino=destino;
    }
}

double Arista::getDistancia() const{
    return this->distancia;
}

void Arista::setDistancia(double distancia){
    this->distancia=(distancia<0) ? -distancia : distancia;
}

TipoCamino Arista::getTipo() const{
    return this->tipo;
}

void Arista::setTipo(TipoCamino tipo){
    this->tipo=tipo;
}

EstadoCamino Arista::getEstado() const {
    return this->estado;
}

void Arista::setEstado(EstadoCamino estado){
    this->estado=estado;
}

bool Arista::esIgual(Nodo* origen,Nodo* destino) const {
    if(origen==nullptr || destino==nullptr){
        return false;
    }

    return (this->origen == origen && this->destino==destino);
}

char* Arista::toString() const{
    const int TAMANO_BUFFER = 300;
    char buffer[TAMANO_BUFFER];
    const char* nombreOrigen=(this->origen!=nullptr) ? this->origen->getNombre() : "NULL";
    const char* nombreDestino=(this->destino!=nullptr) ? this->destino->getNombre(): "NULL";
    std:snprintf(buffer,TAMANO_BUFFER,
            "Arista ID: %d | Origen: %s | Destino: %s | Dist: %.3f m | Tipo: %d  | Estado: %d",
            this->id,
            nombreOrigen,
            nombreDestino,
            this->distancia,
            static_cast<int>(this->tipo),
            static_cast<int>(this->estado));
    int longitudReal=std::strlen(buffer)+1;
    char* resultado=new char[longitudReal];
    std::strcpy(resultado,buffer);
    return resultado;
}

Arista::Arista(const Arista& otro){

}

Arista& Arista::operator=(const Arista& otro){
    return *this;
}