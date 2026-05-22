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
        void insertar(string,string,string);
        void imprimir();
        Nodo* buscar(string);
        void eliminar(string);
        int contarCedulas(string);
        string buscarProvincia(string);
        void conteoGeneral();
        void limpiarpantalla();
        void guardarArchivoIngreso(string,string,string);
        bool busquedaDuplicado(string);
        void modificar(string,string);
        void eliminarUsuario(string);
        string buscarUsuario(string);
        void eliminarDigito();
        void mostrarArchivo();
        string correoExis(string);
        void ordenamientoburbuja();
};
#endif