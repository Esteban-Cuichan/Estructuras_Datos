#ifndef DIJKSTRA_H
#define DIJKSTRA_H

class Grafo;
class Nodo;

class Dijkstra
{
private:

    Grafo* grafo;

    Nodo* obtenerMenorNoVisitado();

    void imprimirRutaRecursiva(Nodo* nodo);

public:

    Dijkstra(Grafo* grafo);

    bool calcular(int origen, int destino);

    void imprimirRuta(int destino);

    double getTiempoTotal(int destino);

    double getDistanciaTotal(int destino);
};

#endif