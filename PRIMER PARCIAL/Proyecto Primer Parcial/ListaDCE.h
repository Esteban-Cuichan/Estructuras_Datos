#ifndef LISTADCE_H
#define LISTADCE_H
#include "Nodo.h"
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
        /*insertar(Turno);
        eliminar(int);
        buscar(int);
        modificar(int)*/
};
#endif