#include "../include/Dijkstra.h"
#include "../include/Grafo.h"
#include "../include/Nodo.h"
#include "../include/ListaNodos.h"
#include "../include/ElementoNodo.h"
#include "../include/ListaAristas.h"
#include "../include/ElementoArista.h"
#include "../include/Arista.h"

#include <limits>
#include <iostream>

using namespace std;

Dijkstra::Dijkstra(Grafo* grafo)
{
    this->grafo = grafo;
}
Nodo* Dijkstra::obtenerMenorNoVisitado()
{
    if(grafo==nullptr)
        return nullptr;

    ListaNodos* lista = grafo->getNodos();

    ElementoNodo* actual = lista->getPrimero();

    Nodo* menor = nullptr;

    while(actual!=nullptr)
    {
        Nodo* nodo = actual->getNodo();

        if(!nodo->estaVisitado())
        {
            if(menor==nullptr)
            {
                menor=nodo;
            }
            else if(nodo->getTiempoAcumulado() < menor->getTiempoAcumulado())
            {
                menor=nodo;
            }
        }

        actual=actual->getSiguiente();
    }

    return menor;
}
bool Dijkstra::calcular(int origen,int destino)
{

    grafo->reiniciarNodos();

    Nodo* nodoOrigen=grafo->buscarNodo(origen);

    Nodo* nodoDestino=grafo->buscarNodo(destino);

    if(nodoOrigen==nullptr || nodoDestino==nullptr)
        return false;

    nodoOrigen->setTiempoAcumulado(0);

    nodoOrigen->setDistanciaAcumulada(0);

    while(true)
    {

        Nodo* actual=obtenerMenorNoVisitado();

        if(actual==nullptr)
            break;

        if(actual->getTiempoAcumulado()==std::numeric_limits<double>::infinity())
            break;

        actual->setVisitado(true);

        ListaAristas* conexiones=actual->getConexiones();

        ElementoArista* conexion=conexiones->getPrimero();

        while(conexion!=nullptr)
        {

            Arista* arista=conexion->getArista();

            Nodo* vecino=arista->getDestino();

            if(!vecino->estaVisitado())
            {

                double nuevoTiempo=
                    actual->getTiempoAcumulado()+
                    arista->getTiempo();

                double nuevaDistancia=
                    actual->getDistanciaAcumulada()+
                    arista->getDistancia();

                if(nuevoTiempo < vecino->getTiempoAcumulado())
                {

                    vecino->setTiempoAcumulado(nuevoTiempo);

                    vecino->setDistanciaAcumulada(nuevaDistancia);

                    vecino->setAnterior(actual);

                }

            }

            conexion=conexion->getSiguiente();

        }

    }

    return nodoDestino->getAnterior()!=nullptr;

}
void Dijkstra::imprimirRutaRecursiva(Nodo* nodo)
{
    if (nodo == nullptr)
        return;

    imprimirRutaRecursiva(nodo->getAnterior());

    cout << "[*] " << nodo->getNombre() << endl;
}

void Dijkstra::imprimirRuta(int destino)
{
    Nodo* nodoDestino = grafo->buscarNodo(destino);

    if (nodoDestino == nullptr)
    {
        cout << "Nodo destino inexistente." << endl;
        return;
    }

    if (nodoDestino->getAnterior() == nullptr &&
        nodoDestino->getTiempoAcumulado() != 0)
    {
        cout << "No existe una ruta." << endl;
        return;
    }

    cout << "\n==============================" << endl;
    cout << "RUTA MAS CORTA ENCONTRADA" << endl;
    cout << "==============================" << endl;

    imprimirRutaRecursiva(nodoDestino);

    cout << "\nDistancia total : "
        << getDistanciaTotal(destino)
        << " metros" << endl;

    cout << "Tiempo total    : "
        << getTiempoTotal(destino)
        << " segundos" << endl;

    cout << "Tiempo aprox.   : "
        << getTiempoTotal(destino)/60.0
        << " minutos" << endl;
}

double Dijkstra::getTiempoTotal(int destino)
{
    Nodo* nodoDestino = grafo->buscarNodo(destino);

    if (nodoDestino == nullptr)
        return -1;

    return nodoDestino->getTiempoAcumulado();
}
double Dijkstra::getDistanciaTotal(int destino)
{
    Nodo* nodoDestino = grafo->buscarNodo(destino);

    if (nodoDestino == nullptr)
        return -1;

    return nodoDestino->getDistanciaAcumulada();
}