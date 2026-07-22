#ifndef GRAFO_H
#define GRAFO_H

#include "Nodo.h"
#include "Arista.h"
#include "ListaAdyacencia.h"

namespace gps {

class Grafo {
public:
    Grafo();
    ~Grafo();

    bool agregarNodo(Nodo *nodo);
    bool agregarArista(Arista *arista);
    bool eliminarNodo(int id);

    int obtenerCantidadNodos() const;
    const Nodo *obtenerNodoPorId(int id) const;
    Nodo *obtenerNodoPorId(int id);
    const Nodo *obtenerNodoPorIndice(int indice) const;
    const ListaAdyacencia *obtenerAdyacenciaPorIndice(int indice) const;
    int obtenerIndiceNodo(int id) const;

private:
    Nodo **nodos_;
    ListaAdyacencia **adyacencias_;
    int cantidad_;
    int capacidad_;

    void asegurarCapacidad(int nuevaCapacidad);
};

} // namespace gps

#endif // GRAFO_H
