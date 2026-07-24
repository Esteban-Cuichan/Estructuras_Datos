#ifndef LISTANODOOSM_H
#define LISTANODOOSM_H

#include "NodoOSM.h"

class ElementoNodoOSM
{
public:

    NodoOSM dato;

    ElementoNodoOSM* siguiente;

    ElementoNodoOSM(
        const NodoOSM& dato
    );
};

class ListaNodoOSM
{
private:

    ElementoNodoOSM* cabeza;

    int cantidad;

public:

    ListaNodoOSM();

    ~ListaNodoOSM();

    bool estaVacia() const;

    int getCantidad() const;

    void agregar(
        const NodoOSM& nodo
    );

    NodoOSM* buscar(
        long long id
    );

    void limpiar();

    void mostrarTodos() const;
};

#endif