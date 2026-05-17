#ifndef LISTADCE_H
#define LISTADCE_H
#include "Nodo.h"
#include <iostream>
using namespace std;

class ListaDCE{
    private:
        Nodo* cabeza;
        Nodo* cola;
    public:
        ListaDCE();
        ListaDCE(Nodo*,Nodo*);
        ~ListaDCE(){}
        Nodo* getCabeza();
        Nodo* getCola();
        void setCabeza(Nodo*);
        void setCola(Nodo*);
        void insertar(Turno*);
        bool eliminarPorFecha(int,int,int);
        Nodo* buscarPorFecha(int,int,int);
        bool modificar(int,int,int,Fecha);
        void mostrarLista();
        void guardarListaEnArchivo();
};
#endif