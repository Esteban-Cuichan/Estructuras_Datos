#ifndef NODO_H
#define NODO_H
#include <string.h>
#include <iostream>
using namespace std;
class Nodo{
    private:
        string cedula;
        string nombre;
        Nodo* siguiente;
        Nodo* anterior;
    public:
        void setCedula(string);
        void setNombre(string);
        void setSiguiente(Nodo*);
        void setAnterior(Nodo*);
        string getCedula();
        string getNombre();
        Nodo* getSiguiente();
        Nodo* getAnterior();
        Nodo(string,string,Nodo*,Nodo*);
        Nodo();
        ~Nodo() {};
};
#endif