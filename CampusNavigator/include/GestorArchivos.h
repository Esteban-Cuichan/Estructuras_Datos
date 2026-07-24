#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <string>

using namespace std;

class Grafo;

class GestorArchivos
{
public:
    bool cargarNodos(
        const string& rutaArchivo,
        Grafo* grafo
    );

    bool cargarAristas(
        const string& rutaArchivo,
        Grafo* grafo
    );
};

#endif