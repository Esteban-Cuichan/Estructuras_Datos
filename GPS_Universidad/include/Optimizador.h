#ifndef OPTIMIZADOR_H
#define OPTIMIZADOR_H

#include "Grafo.h"
#include "ListaAdyacencia.h"
#include <queue>
#include <vector>
#include <utility>

class Optimizador {
private:
    Grafo* grafo;
    Nodo** cacheNodos;
    int tamanoCache;
    bool useCache;
    double* distancias;
    int* predecesores;
    bool* visitados;
    int numNodos;
    int accesosCache;
    int fallosCache;
    int aristasComprimidas;

    void inicializarDijkstra(int origen);
    void relajarArista(int u, Arista* arista);

public:
    Optimizador(Grafo* grafo);
    ~Optimizador();
    bool optimizarDijkstra(int origen, int destino);
    void construirCacheNodos();
    Nodo* obtenerNodoCache(int id);
    void optimizarMemoria();
    void comprimirGrafo();
    char* getEstadisticas() const;
    void setUseCache(bool habilitar);
    bool getUseCache() const;

private:
    Optimizador(const Optimizador& otro);
    Optimizador& operator=(const Optimizador& otro);
};

#endif