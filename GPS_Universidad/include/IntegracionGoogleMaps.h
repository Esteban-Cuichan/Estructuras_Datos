#ifndef INTEGRACION_GOOGLE_MAPS_H
#define INTEGRACION_GOOGLE_MAPS_H

#include "Grafo.h"
#include <cstdio>

class IntegracionGoogleMaps {
private:
    Grafo* grafo;
    int* ruta;
    int longitudRuta;
    static const char* BASE_URL;

    char* generarWaypoints() const;
    char* codificarURL(const char* cadena) const;
    const char* getNombreNodo(int id) const;

public:
    IntegracionGoogleMaps(Grafo* grafo);
    ~IntegracionGoogleMaps();
    void setRuta(int* ruta, int longitud);
    char* generarEnlaceNodo(Nodo* nodo) const;
    char* generarEnlaceRuta() const;
    char* generarEnlaceGrafo() const;
    char* generarURLCoordenadas(double lat, double lon) const;
    char* generarKMLRuta() const;
    bool exportarCoordenadas(const char* archivo) const;
    void mostrarEnlaceRuta() const;

private:
    IntegracionGoogleMaps(const IntegracionGoogleMaps& otro);
    IntegracionGoogleMaps& operator=(const IntegracionGoogleMaps& otro);
};

#endif