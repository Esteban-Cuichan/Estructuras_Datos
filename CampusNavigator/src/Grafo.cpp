#include "../include/Grafo.h"
#include "../include/ListaNodos.h"
#include "../include/ElementoNodo.h"
#include "../include/Nodo.h"
#include "../include/ListaAristas.h"
#include "../include/ElementoArista.h"
#include "../include/Arista.h"

#include <iostream>

using namespace std;

Grafo::Grafo()
    : nodos(new ListaNodos())
{
}

Grafo::~Grafo()
{
    delete nodos;
    nodos = nullptr;
}

bool Grafo::agregarNodo(
    int id,
    const string& nombre,
    double latitud,
    double longitud
)
{
    if (id < 0)
    {
        return false;
    }

    if (nombre.empty())
    {
        return false;
    }

    if (latitud < -90.0 || latitud > 90.0)
    {
        return false;
    }

    if (longitud < -180.0 || longitud > 180.0)
    {
        return false;
    }

    if (buscarNodo(id) != nullptr)
    {
        return false;
    }

    Nodo* nuevoNodo = new Nodo(
        id,
        nombre,
        latitud,
        longitud
    );

    nodos->insertar(nuevoNodo);

    return true;
}

bool Grafo::agregarConexion(
    int idOrigen,
    int idDestino,
    double distancia,
    double tiempo
)
{
    if (distancia < 0.0 || tiempo < 0.0)
    {
        return false;
    }

    Nodo* origen = buscarNodo(idOrigen);
    Nodo* destino = buscarNodo(idDestino);

    if (origen == nullptr || destino == nullptr)
    {
        return false;
    }

    if (origen == destino)
    {
        return false;
    }

    ListaAristas* conexiones = origen->getConexiones();

    if (conexiones == nullptr)
    {
        return false;
    }

    conexiones->insertar(
        destino,
        distancia,
        tiempo
    );

    return true;
}

bool Grafo::agregarConexionBidireccional(
    int idNodoA,
    int idNodoB,
    double distancia,
    double tiempo
)
{
    if (idNodoA == idNodoB)
    {
        return false;
    }

    bool conexionA = agregarConexion(
        idNodoA,
        idNodoB,
        distancia,
        tiempo
    );

    bool conexionB = agregarConexion(
        idNodoB,
        idNodoA,
        distancia,
        tiempo
    );

    return conexionA && conexionB;
}

Nodo* Grafo::buscarNodo(int id) const
{
    if (nodos == nullptr)
    {
        return nullptr;
    }

    return nodos->buscarPorId(id);
}

ListaNodos* Grafo::getNodos() const
{
    return nodos;
}

int Grafo::getCantidadNodos() const
{
    if (nodos == nullptr)
    {
        return 0;
    }

    return nodos->getCantidad();
}

bool Grafo::estaVacio() const
{
    if (nodos == nullptr)
    {
        return true;
    }

    return nodos->estaVacia();
}

void Grafo::reiniciarNodos()
{
    if (nodos == nullptr)
    {
        return;
    }

    ElementoNodo* actual = nodos->getPrimero();

    while (actual != nullptr)
    {
        Nodo* nodoActual = actual->getNodo();

        if (nodoActual != nullptr)
        {
            nodoActual->reiniciar();
        }

        actual = actual->getSiguiente();
    }
}

void Grafo::mostrarNodos() const
{
    if (nodos == nullptr || nodos->estaVacia())
    {
        cout << "El grafo no contiene nodos." << endl;
        return;
    }

    cout << "\nNODOS DEL GRAFO" << endl;
    cout << "================" << endl;

    ElementoNodo* actual = nodos->getPrimero();

    while (actual != nullptr)
    {
        Nodo* nodoActual = actual->getNodo();

        if (nodoActual != nullptr)
        {
            cout << nodoActual->getId()
                 << " - "
                 << nodoActual->getNombre()
                 << endl;

            cout << "   Latitud: "
                 << nodoActual->getLatitud()
                 << endl;

            cout << "   Longitud: "
                 << nodoActual->getLongitud()
                 << endl;
        }

        actual = actual->getSiguiente();
    }
}

void Grafo::mostrarConexiones() const
{
    if (nodos == nullptr || nodos->estaVacia())
    {
        cout << "El grafo no contiene nodos." << endl;
        return;
    }

    cout << "\nCONEXIONES DEL GRAFO" << endl;
    cout << "====================" << endl;

    ElementoNodo* elementoNodo =
        nodos->getPrimero();

    while (elementoNodo != nullptr)
    {
        Nodo* nodoOrigen =
            elementoNodo->getNodo();

        if (nodoOrigen != nullptr)
        {
            cout << "\n"
                 << nodoOrigen->getId()
                 << " - "
                 << nodoOrigen->getNombre()
                 << endl;

            ListaAristas* conexiones =
                nodoOrigen->getConexiones();

            if (
                conexiones == nullptr ||
                conexiones->estaVacia()
            )
            {
                cout << "  Sin conexiones." << endl;
            }
            else
            {
                ElementoArista* elementoArista =
                    conexiones->getPrimero();

                while (elementoArista != nullptr)
                {
                    Arista* arista =
                        elementoArista->getArista();

                    if (
                        arista != nullptr &&
                        arista->getDestino() != nullptr
                    )
                    {
                        Nodo* destino =
                            arista->getDestino();

                        cout << "  -> "
                             << destino->getId()
                             << " - "
                             << destino->getNombre()
                             << " | Distancia: "
                             << arista->getDistancia()
                             << " metros"
                             << " | Tiempo: "
                             << arista->getTiempo()
                             << " segundos"
                             << endl;
                    }

                    elementoArista =
                        elementoArista->getSiguiente();
                }
            }
        }

        elementoNodo =
            elementoNodo->getSiguiente();
    }
}