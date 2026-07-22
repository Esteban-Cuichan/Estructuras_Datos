#ifndef ELEMENTO_NODO_H
#define ELEMENTO_NODO_H

class Nodo;

class ElementoNodo {
    private:
        Nodo* nodo;
        ElementoNodo* siguiente;

    public:
        ElementoNodo(Nodo* nodo);
        ~ElementoNodo();

        Nodo* getNodo() const;
        ElementoNodo* getSiguiente() const;

        void setNodo(Nodo* nodo);
        void setSiguiente(ElementoNodo* siguiente);
};

#endif