#ifndef LISTA_NODOS_H
#define LISTA_NODOS_H

class Nodo;
class ElementoNodo;

class ListaNodos {
    private:
        ElementoNodo* primero;
        ElementoNodo* ultimo;
        int cantidad;

    public:
        ListaNodos();
        ~ListaNodos();

        void insertar(Nodo* nodo);

        Nodo* buscarPorId(int id) const;

        ElementoNodo* getPrimero() const;
        ElementoNodo* getUltimo() const;

        int getCantidad() const;
        bool estaVacia() const;

        void vaciar();
};

#endif