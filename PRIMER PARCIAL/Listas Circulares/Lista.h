#ifndef LISTA_H
#define LISTA_H
#include "Nodo.h"
#include "Operaciones.h"

class Lista:public Operaciones{
    private:
        Nodo* cabeza;
        Nodo* cola;
    public:
        Lista();
        ~Lista() {};
        void setCabeza(Nodo*);
        void setCola(Nodo*);
        Nodo* getCabeza();
        Nodo* getCola();
        void insertar(string,string);
        void imprimir();
        Nodo* buscar(string);
        void eliminar(string);
};
#endif