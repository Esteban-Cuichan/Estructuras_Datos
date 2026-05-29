#include "Vehiculo.h"

Vehiculo::Vehiculo(){
    this->placa="";
    this->color="";
    this->anio=0;
}

Vehiculo::Vehiculo(string col,string placa,int anio){
    this->placa=placa;
    this->color=color;
    this->anio=anio;
}

string Vehiculo::getColor(){
    return color;
}

string Vehiculo::getPlaca(){
    return placa;
}

int Vehiculo::getAnio(){
    return anio;
}