#ifndef RECONSTRUCTOR_RUTAS_H
#define RECONSTRUCTOR_RUTAS_H

#include "Grafo.h"

class ReconstructorRutas {
private:
    Grafo* grafo;
    int* predecesores;
    int numNodos;
    int origen;
    int destino;
    int* camino;
    int longitudCamino;
    double* distanciasTramo;
    double distanciaTotal;

    bool reconstruirCamino(int origen, int destino);
    void calcularDistanciasTramo();
    void limpiarCamino();

public:
    ReconstructorRutas(Grafo* grafo, int* predecesores);
    ~ReconstructorRutas();
    bool reconstruir(int origen, int destino);
    int* getCamino() const;
    int getLongitudCamino() const;
    double getDistanciaTotal() const;
    double getDistanciaTramo(int pos) const;
    char* getInstrucciones() const;
    char* toString() const;
    void imprimirRuta() const;

private:
    ReconstructorRutas(const ReconstructorRutas& otro);
    ReconstructorRutas& operator=(const ReconstructorRutas& otro);
};

#endif