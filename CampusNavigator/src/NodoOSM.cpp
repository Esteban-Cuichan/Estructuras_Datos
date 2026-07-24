#include "../include/NodoOSM.h"

NodoOSM::NodoOSM(
    long long id,
    double latitud,
    double longitud
)
{
    this->id = id;
    this->latitud = latitud;
    this->longitud = longitud;
}

long long NodoOSM::getId() const
{
    return id;
}

double NodoOSM::getLatitud() const
{
    return latitud;
}

double NodoOSM::getLongitud() const
{
    return longitud;
}