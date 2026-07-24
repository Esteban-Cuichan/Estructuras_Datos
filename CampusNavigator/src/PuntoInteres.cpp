#include "../include/PuntoInteres.h"

#include <iostream>

using namespace std;

PuntoInteres::PuntoInteres()
{
    idOSM = 0;

    nombre = "";
    tipo = "";

    latitud = 0.0;
    longitud = 0.0;
}

PuntoInteres::PuntoInteres(
    long long idOSM,
    const string& nombre,
    const string& tipo,
    double latitud,
    double longitud
)
{
    this->idOSM = idOSM;
    this->nombre = nombre;
    this->tipo = tipo;
    this->latitud = latitud;
    this->longitud = longitud;
}

PuntoInteres::~PuntoInteres()
{
}

long long PuntoInteres::getIdOSM() const
{
    return idOSM;
}

string PuntoInteres::getNombre() const
{
    return nombre;
}

string PuntoInteres::getTipo() const
{
    return tipo;
}

double PuntoInteres::getLatitud() const
{
    return latitud;
}

double PuntoInteres::getLongitud() const
{
    return longitud;
}

void PuntoInteres::setNombre(
    const string& nombre
)
{
    this->nombre = nombre;
}

void PuntoInteres::setTipo(
    const string& tipo
)
{
    this->tipo = tipo;
}

void PuntoInteres::setLatitud(
    double latitud
)
{
    this->latitud = latitud;
}

void PuntoInteres::setLongitud(
    double longitud
)
{
    this->longitud = longitud;
}

void PuntoInteres::mostrar() const
{
    cout << "Punto de interes" << endl;
    cout << "ID OSM: " << idOSM << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Tipo: " << tipo << endl;
    cout << "Latitud: " << latitud << endl;
    cout << "Longitud: " << longitud << endl;
}