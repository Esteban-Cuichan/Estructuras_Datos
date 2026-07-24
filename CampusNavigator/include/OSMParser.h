#ifndef OSMPARSER_H
#define OSMPARSER_H

#include <string>

#include "Grafo.h"
#include "ListaPuntoInteres.h"

using namespace std;

class OSMParser
{
private:
    double calcularDistancia(
        double latitud1,
        double longitud1,
        double latitud2,
        double longitud2
    ) const;

    double calcularTiempo(
        double distancia
    ) const;

    bool esDestinoValido(
        const string& tipo,
        const string& valorTipo
    ) const;

public:
    OSMParser();

    bool cargarMapa(
        const string& rutaArchivo,
        Grafo* grafo,
        ListaPuntoInteres* listaPuntos
    );
};

#endif