#ifndef CARGADOR_DATOS_H
#define CARGADOR_DATOS_H
#include "Grafo.h"
#include <cstdio>

class CargadorDatos {
    private:
        Grafo* grafo;
        static const int MAX_LINEA=1024;

        Nodo* parsearLineaNodo(char* linea, int numLinea);
        Arista* parsearLineaArista(char* linea, int numLinea);
        char* trim(char* str);
        bool split(const char* str, char delim, char*** tokens, int* count);
        TipoNodo obtenerTipoNodo(const char* tipo);
        TipoCamino obtenerTipoCamino(const char* tipo);
        EstadoCamino obtenerEstadoCamino(const char* estado);
        void liberarTokens(char** tokens, int count);

    public:
        CargadorDatos(Grafo* grafo);
        ~CargadorDatos();
        bool cargarEdificios(const char* archivo);
        bool cargarConexiones(const char* archivo);
        bool cargarConfiguracion(const char* archivo);

    private:
        CargadorDatos(const CargadorDatos& otro);
        CargadorDatos& operator=(const CargadorDatos& otro);
};

#endif