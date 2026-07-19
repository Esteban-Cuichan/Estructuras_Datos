#ifndef GRAFO_H
#define GRAFO_H
#include "Nodo.h"
#include "Arista.h"
class Grafo{
    private:
        Nodo** nodos;
        Arista** aristas;
        int numNodos;
        int numAristas;
        int capacidadNodos;
        int capacidadAristas;
        static const int CAPACIDAD_INICIAL=10;
        static const int FACTOR_INCREMENTO=2;
        bool redimensionarNodos();
        bool redimensionarAristas();
    public:
        Grafo();
        ~Grafo();
        bool agregarNodo(Nodo* nodo);
        bool agregarArista(Arista* arista);
        Nodo* buscarNodoPorId(int id) const;
        Nodo* buscarNodoPorNombre(const char* nombre) const;
        Arista* buscarArista(Nodo* origen,Nodo* destino) const;
        int getNumNodos() const;
        Nodo* getNodo(int indice) const;
        int getNumAristas() const;
        Arista* getArista(int indice) const;
        bool existeNodo(int id) const;
        bool existeArista(Nodo* origen, Nodo* destino) const;
        char* toString() const;
        void limpiar();
    private:
        Grafo(const Grafo& otro);
        Grafo& operator=(const Grafo& otro);
};
#endif