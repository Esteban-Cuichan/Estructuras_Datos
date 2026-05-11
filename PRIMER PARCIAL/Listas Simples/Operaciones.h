#ifndef OPERACIONES_H
#define OPERACIONES_H
#include "Lista.h"

class Operaciones{
    public:
        virtual void insertar(string,string)=0;
        virtual void imprimir()=0;
        virtual Nodo* buscar(string)=0;
        virtual void eliminar(string)=0;
        virtual ~Operaciones() {}
        virtual int contarCedulas(string)=0;
        virtual string buscarProvincia(string)=0;
        virtual void conteoGeneral()=0;
        virtual void limpiarpantalla()=0;
        virtual void guardarArchivoIngreso(string,string)=0;
        virtual bool busquedaDuplicado(string)=0;
        virtual void modificar(string,string)=0;
        virtual void eliminarUsuario(string)=0;
        virtual string buscarUsuario(string)=0;
};
#endif