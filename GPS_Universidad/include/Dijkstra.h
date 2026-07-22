#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grafo.h"

namespace gps {

class Dijkstra {
public:
    Dijkstra(const Grafo &grafo);
    ~Dijkstra();

    bool calcularRuta(int origenId, int destinoId);
    int obtenerNodoSiguiente(int id) const;
    double obtenerDistanciaTotal() const;
    int obtenerCantidadNodosRecorridos() const;

private:
    const Grafo &grafo_;
    double *distancias_;
    int *predecesores_;
    bool *visitado_;
    int cantidadNodos_;
    int origenIndice_;
    int destinoIndice_;
    double distanciaTotal_;
    int nodosRecorridos_;

    int obtenerIndiceMenorDistancia() const;
    void inicializarEstructuras();
    void liberarEstructuras();
};

} // namespace gps

#endif // DIJKSTRA_H
