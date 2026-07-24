#include "../include/ListaNodoOSM.h"

#include <iostream>

using namespace std;

ElementoNodoOSM::ElementoNodoOSM(
    const NodoOSM& dato
)
    : dato(dato)
{
    siguiente = nullptr;
}

ListaNodoOSM::ListaNodoOSM()
{
    cabeza = nullptr;
    cantidad = 0;
}

ListaNodoOSM::~ListaNodoOSM()
{
    limpiar();
}

bool ListaNodoOSM::estaVacia() const
{
    return cabeza == nullptr;
}

int ListaNodoOSM::getCantidad() const
{
    return cantidad;
}

void ListaNodoOSM::agregar(
    const NodoOSM& nodo
)
{
    ElementoNodoOSM* nuevo =
        new ElementoNodoOSM(nodo);

    if(cabeza == nullptr)
    {
        cabeza = nuevo;
    }
    else
    {
        ElementoNodoOSM* actual = cabeza;

        while(actual->siguiente != nullptr)
        {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    cantidad++;
}

NodoOSM* ListaNodoOSM::buscar(
    long long id
)
{
    ElementoNodoOSM* actual = cabeza;

    while(actual != nullptr)
    {
        if(actual->dato.getId() == id)
        {
            return &(actual->dato);
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

void ListaNodoOSM::mostrarTodos() const
{
    ElementoNodoOSM* actual = cabeza;

    while(actual != nullptr)
    {
        cout
            << actual->dato.getId()
            << "  "
            << actual->dato.getLatitud()
            << "  "
            << actual->dato.getLongitud()
            << endl;

        actual = actual->siguiente;
    }

    cout << "Total de nodos OSM: "
         << cantidad
         << endl;
}

void ListaNodoOSM::limpiar()
{
    ElementoNodoOSM* actual = cabeza;

    while(actual != nullptr)
    {
        ElementoNodoOSM* siguiente =
            actual->siguiente;

        delete actual;

        actual = siguiente;
    }

    cabeza = nullptr;

    cantidad = 0;
}