#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grafo.h"
#include "ListaAdyacencia.h"
#include <climits>

class Dijkstra {
private:
    Grafo* grafo;
    double* distancias;
    int* predecesores;
    bool* visitados;
    int numNodos;
    int origen;
    int destino;
    bool caminoEncontrado;
    static const double INFINITO;

    void inicializar(int origen);
    int encontrarMinimo();
    void relajarArista(int actual, Arista* arista);

public:
    Dijkstra(Grafo* grafo);
    ~Dijkstra();
    bool ejecutar(int origen, int destino);
    double getDistancia(int nodo) const;
    int getPredecesor(int nodo) const;
    int* getCaminoCompleto(int destino) const;
    int getNumNodos() const;
    bool hayCamino() const;
    void imprimirDistancias() const;
    char* toString() const;

private:
    Dijkstra(const Dijkstra& otro);
    Dijkstra& operator=(const Dijkstra& otro);
};

#endif