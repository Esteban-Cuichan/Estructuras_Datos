#ifndef LISTA_ADYACENCIA_H
#define LISTA_ADYACENCIA_H

#include "Arista.h"

namespace gps {

class ListaAdyacencia {
public:
    struct NodoAdyacente {
        Arista *arista;
        NodoAdyacente *siguiente;

        explicit NodoAdyacente(Arista *arista);
        ~NodoAdyacente();
    };

    ListaAdyacencia();
    ~ListaAdyacencia();

    void agregarArista(Arista *arista);
    bool eliminarArista(int destinoId);
    bool estaVacia() const;
    int obtenerTamano() const;
    const NodoAdyacente *obtenerCabeza() const;

private:
    NodoAdyacente *cabeza_;
    int tamano_;
};

} // namespace gps

#endif // LISTA_ADYACENCIA_H
