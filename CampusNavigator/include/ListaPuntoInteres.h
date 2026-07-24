#ifndef LISTAPUNTOINTERES_H
#define LISTAPUNTOINTERES_H

#include "PuntoInteres.h"

#include <string>

using namespace std;

class NodoPuntoInteres
{
public:
    PuntoInteres dato;
    NodoPuntoInteres* siguiente;

    NodoPuntoInteres(
        const PuntoInteres& dato
    );
};

class ListaPuntoInteres
{
private:
    NodoPuntoInteres* cabeza;
    int cantidad;

public:
    ListaPuntoInteres();

    ~ListaPuntoInteres();

    bool estaVacia() const;

    int getCantidad() const;

    void agregar(
        const PuntoInteres& punto
    );

    PuntoInteres* buscarPorNombre(
        const string& nombre
    );

    PuntoInteres* buscarPorIdOSM(
        long long idOSM
    );

    bool existeNombre(
        const string& nombre
    ) const;

    void mostrarTodos() const;

    void limpiar();
};

#endif