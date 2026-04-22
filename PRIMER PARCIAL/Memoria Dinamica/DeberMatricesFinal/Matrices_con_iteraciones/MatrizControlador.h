#ifndef MATRIZ_CONTROLADOR_H
#define MATRIZ_CONTROLADOR_H
#include "Matriz.h"

class MatrizControlador{
    public:
        void sumar();
        void multiplicar();
        void ingreso(Matriz<float>& mat);
        //void imprimir(const Matriz<float>& mat);
        void imprimir(const Matriz<float>& mat);
        void encerar(Matriz<float>& mat);
};

#endif