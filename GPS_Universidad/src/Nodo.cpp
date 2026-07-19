#include "../include/Nodo.h"
#include "../include/ListaAdyacencia.h"
#include <cstdio>
#include <cstdlib>

Nodo::Nodo(int id,const char* nombre,double longitud,double latitud)
    : id(id),longitud(longitud),latitud(latitud),tipo(TipoNodo::BLOQUE){
        int longitudNombre=std::strlen(nombre)+1;
        this->nombre=new char[longitudNombre];
        std::strcpy(this->nombre, nombre);
        this->listaAdyacencia=new ListaAdyacencia();
}

Nodo::~Nodo(){
    delete[] this->nombre;
    delete this->listaAdyacencia;
}

int Nodo::getId() const{
    return this->id;
}

const char* Nodo::getNombre() const{
    return this->nombre;
}

double Nodo::getLatitud() const{
    return this->latitud;
}

double Nodo::getLongitud() const{
    return this->longitud;
}

TipoNodo Nodo::getTipo() const{
    return this->tipo;
}

void Nodo::setTipo(TipoNodo tipo){
    this->tipo=tipo;
}

ListaAdyacencia* Nodo::getListaAdyacencia() const{
    return this->listaAdyacencia;
}

void Nodo::setListaAdyacencia(ListaAdyacencia* lista){
    this->listaAdyacencia=lista;
}

char* Nodo::toString() const{
    const int TAMANO_BUFFER=200;
    char buffer[TAMANO_BUFFER];
    std::snprintf(buffer, TAMANO_BUFFER,
                "ID: %d | Nombre: %s | Tipo: %d | (%.6f, %.6f)",
                this->id,
                this->nombre,
                static_cast<int>(this->tipo),
                this->latitud,
                this->latitud);
    int longitudReal=std::strlen(buffer)+1;
    char* resultado=new char[longitudReal];
    std:strcpy(resultado,buffer);
    return resultado;
}

Nodo::Nodo(const Nodo& otro){}

Nodo& Nodo::operator=(const Nodo& otro){
    return *this;
}