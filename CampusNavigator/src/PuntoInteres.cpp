#include "../include/PuntoInteres.h"
#include <iomanip>
#include <iostream>


using namespace std;

PuntoInteres::PuntoInteres()
{
    idOSM = 0;

    nombre = "";
    categoria = "";
    subtipo = "";

    latitud = 0.0;
    longitud = 0.0;
}

PuntoInteres::PuntoInteres(
    long long idOSM,
    const string& nombre,
    const string& categoria,
    const string& subtipo,
    double latitud,
    double longitud
)
{
    this->idOSM = idOSM;
    this->nombre = nombre;
    this->categoria = categoria;
    this->subtipo = subtipo;
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

string PuntoInteres::getCategoria() const
{
    return categoria;
}

string PuntoInteres::getSubtipo() const
{
    return subtipo;
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

void PuntoInteres::setCategoria(
    const string& categoria
)
{
    this->categoria = categoria;
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
    cout << fixed << setprecision(7);

    cout << "Punto de interes" << endl;
    cout << "ID OSM: " << idOSM << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Categoria: " << categoria << endl;
    cout << "Subtipo: " << subtipo << endl;
    cout << "Latitud: " << latitud << endl;
    cout << "Longitud: " << longitud << endl;
}