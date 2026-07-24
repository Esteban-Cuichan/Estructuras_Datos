#include "../include/Nodo.h"
#include "../include/ListaAristas.h"

#include <limits>

Nodo::Nodo(
    int id,
    const string& nombre,
    double latitud,
    double longitud
)
    : id(id),
      nombre(nombre),
      latitud(latitud),
      longitud(longitud),
      visitado(false),
      tiempoAcumulado(
          std::numeric_limits<double>::infinity()
      ),
      distanciaAcumulada(
          std::numeric_limits<double>::infinity()
      ),
      anterior(nullptr),
      aristas(new ListaAristas())
{
}

Nodo::~Nodo()
{
    delete aristas;
    aristas = nullptr;
}

int Nodo::getId() const
{
    return id;
}

string Nodo::getNombre() const
{
    return nombre;
}

double Nodo::getLatitud() const
{
    return latitud;
}

double Nodo::getLongitud() const
{
    return longitud;
}

bool Nodo::estaVisitado() const
{
    return visitado;
}

void Nodo::setVisitado(bool estado)
{
    visitado = estado;
}

double Nodo::getTiempoAcumulado() const
{
    return tiempoAcumulado;
}

void Nodo::setTiempoAcumulado(double tiempo)
{
    tiempoAcumulado = tiempo;
}

double Nodo::getDistanciaAcumulada() const
{
    return distanciaAcumulada;
}

void Nodo::setDistanciaAcumulada(double distancia)
{
    distanciaAcumulada = distancia;
}

Nodo* Nodo::getAnterior() const
{
    return anterior;
}

void Nodo::setAnterior(Nodo* nodo)
{
    anterior = nodo;
}

ListaAristas* Nodo::getConexiones() const
{
    return aristas;
}

void Nodo::reiniciar()
{
    visitado = false;

    tiempoAcumulado =
        std::numeric_limits<double>::infinity();

    distanciaAcumulada =
        std::numeric_limits<double>::infinity();

    anterior = nullptr;
}