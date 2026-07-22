#ifndef LISTA_ARISTAS_H
#define LISTA_ARISTAS_H

class Nodo;
class Arista;
class ElementoArista;

class ListaAristas {
private:
    ElementoArista* primero;
    ElementoArista* ultimo;
    int cantidad;

public:
    ListaAristas();
    ~ListaAristas();

    void insertar(Nodo* destino, double distancia, double tiempo);
    void insertar(Arista* arista);

    ElementoArista* getPrimero() const;
    ElementoArista* getUltimo() const;

    int getCantidad() const;
    bool estaVacia() const;

    void vaciar();
};

#endif