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
        ~ListaDCE();
        Nodo* getCabeza();
        Nodo* getCola();
        void setCabeza(Nodo*);
        void setCola(Nodo*);
        void insertar(Turno*);
        bool eliminarPorID(int);
        Nodo* buscarPorID(int);
        bool modificar(int,Fecha);
        void mostrarLista();
        void guardarListaEnArchivo();
        int generarSiguienteID();
        void cargarListaDesdeArchivo();
        void cifrarPlacas();
};
#endif