#include "../include/ListaPuntoInteres.h"

#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

NodoPuntoInteres::NodoPuntoInteres(
    const PuntoInteres& dato
)
{
    this->dato = dato;
    siguiente = nullptr;
}

ListaPuntoInteres::ListaPuntoInteres()
{
    cabeza = nullptr;
    cantidad = 0;
}

ListaPuntoInteres::~ListaPuntoInteres()
{
    limpiar();
}

bool ListaPuntoInteres::estaVacia() const
{
    return cabeza == nullptr;
}

int ListaPuntoInteres::getCantidad() const
{
    return cantidad;
}

void ListaPuntoInteres::agregar(
    const PuntoInteres& punto
)
{
    NodoPuntoInteres* nuevo =
        new NodoPuntoInteres(punto);

    if (cabeza == nullptr)
    {
        cabeza = nuevo;
    }
    else
    {
        NodoPuntoInteres* actual = cabeza;

        while (actual->siguiente != nullptr)
        {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    cantidad++;
}

PuntoInteres* ListaPuntoInteres::buscarPorNombre(
    const string& nombre
)
{
    NodoPuntoInteres* actual = cabeza;

    while (actual != nullptr)
    {
        if(
            normalizarNombre(actual->dato.getNombre())
            ==
            normalizarNombre(nombre)
        )
        {
            return &(actual->dato);
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

PuntoInteres* ListaPuntoInteres::buscarPorIdOSM(
    long long idOSM
)
{
    NodoPuntoInteres* actual = cabeza;

    while (actual != nullptr)
    {
        if (actual->dato.getIdOSM() == idOSM)
        {
            return &(actual->dato);
        }

        actual = actual->siguiente;
    }

    return nullptr;
}

bool ListaPuntoInteres::existeNombre(
    const string& nombre
) const
{
    NodoPuntoInteres* actual = cabeza;

    while (actual != nullptr)
    {
        if(
            normalizarNombre(actual->dato.getNombre())
            ==
            normalizarNombre(nombre)
        )
        {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

void ListaPuntoInteres::mostrarTodos() const
{
    if (cabeza == nullptr)
    {
        cout << "No hay puntos de interes."
             << endl;

        return;
    }

    NodoPuntoInteres* actual = cabeza;

    cout << "\nPUNTOS DE INTERES" << endl;
    cout << "==================" << endl;

    while (actual != nullptr)
    {
        actual->dato.mostrar();

        cout << "------------------------"
             << endl;

        actual = actual->siguiente;
    }

    cout << "Total: "
         << cantidad
         << endl;
}

void ListaPuntoInteres::limpiar()
{
    NodoPuntoInteres* actual = cabeza;

    while (actual != nullptr)
    {
        NodoPuntoInteres* siguiente =
            actual->siguiente;

        delete actual;

        actual = siguiente;
    }

    cabeza = nullptr;
    cantidad = 0;
}
string ListaPuntoInteres::normalizarNombre(
    const string& nombre
) const
{
    string copia = nombre;

    for(char& c : copia)
    {
        c = tolower(c);
    }

    while(
        !copia.empty() &&
        isspace(copia.front())
    )
    {
        copia.erase(copia.begin());
    }

    while(
        !copia.empty() &&
        isspace(copia.back())
    )
    {
        copia.pop_back();
    }

    return copia;
}