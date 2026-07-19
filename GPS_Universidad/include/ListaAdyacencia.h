#ifndef LISTA_ADYACENCIA_H
#define LISTA_ADYACENCIA_H
#include "Arista.h"
#include "Nodo.h"
class NodoArista;
class NodoArista {
    private:
        Arista* arista;
        NodoArista* siguiente;
    public:
        NodoArista(Arista* arista);
        ~NodoArista();
        Arista* getArista() const;
        NodoArista* getSiguiente() const;
        void setSiguiente(NodoArista* siguiente);
    private:
        NodoArista(const NodoArista& otro);
        NodoArista& operator=(const NodoArista& otro);
};

class ListaAdyacencia {
    private:
        NodoArista* cabeza;
        int tamano;
    public:
        ListaAdyacencia();
        ~ListaAdyacencia();
        bool agregarArista(Arista* arista);
        bool eliminarArista(Arista* arista);
        Arista* buscarArista(Nodo* destino) const;
        NodoArista* getPrimero() const;
        int getTamano() const;
        bool estaVacia() const;
        char* toString() const;
        void limpiar();
    private:
        ListaAdyacencia(const ListaAdyacencia& otro);
        ListaAdyacencia& operator=(const ListaAdyacencia& otro);
};

#endif