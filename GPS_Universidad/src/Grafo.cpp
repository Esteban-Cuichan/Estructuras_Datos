#include "Grafo.h"
#include <algorithm>

namespace gps {

Grafo::Grafo()
    : nodos_(nullptr), adyacencias_(nullptr), cantidad_(0), capacidad_(0) {
    asegurarCapacidad(8);
}

Grafo::~Grafo() {
    for (int i = 0; i < cantidad_; ++i) {
        delete nodos_[i];
        delete adyacencias_[i];
    }
    delete[] nodos_;
    delete[] adyacencias_;
}

void Grafo::asegurarCapacidad(int nuevaCapacidad) {
    if (nuevaCapacidad <= capacidad_) {
        return;
    }

    int capacidadFinal = std::max(nuevaCapacidad, capacidad_ * 2);
    if (capacidadFinal < 8) {
        capacidadFinal = 8;
    }

    Nodo **nuevosNodos = new Nodo *[capacidadFinal];
    ListaAdyacencia **nuevasAdyacencias = new ListaAdyacencia *[capacidadFinal];

    for (int i = 0; i < cantidad_; ++i) {
        nuevosNodos[i] = nodos_[i];
        nuevasAdyacencias[i] = adyacencias_[i];
    }

    delete[] nodos_;
    delete[] adyacencias_;

    nodos_ = nuevosNodos;
    adyacencias_ = nuevasAdyacencias;
    capacidad_ = capacidadFinal;
}

bool Grafo::agregarNodo(Nodo *nodo) {
    if (obtenerIndiceNodo(nodo->obtenerId()) != -1) {
        return false;
    }

    if (cantidad_ + 1 > capacidad_) {
        asegurarCapacidad(cantidad_ + 1);
    }

    nodos_[cantidad_] = nodo;
    adyacencias_[cantidad_] = new ListaAdyacencia();
    ++cantidad_;
    return true;
}

bool Grafo::agregarArista(Arista *arista) {
    int origenIndice = obtenerIndiceNodo(arista->obtenerOrigenId());
    int destinoIndice = obtenerIndiceNodo(arista->obtenerDestinoId());

    if (origenIndice == -1 || destinoIndice == -1) {
        return false;
    }

    adyacencias_[origenIndice]->agregarArista(arista);
    return true;
}

bool Grafo::eliminarNodo(int id) {
    int indice = obtenerIndiceNodo(id);
    if (indice == -1) {
        return false;
    }

    delete nodos_[indice];
    delete adyacencias_[indice];

    for (int i = indice; i < cantidad_ - 1; ++i) {
        nodos_[i] = nodos_[i + 1];
        adyacencias_[i] = adyacencias_[i + 1];
    }

    --cantidad_;
    return true;
}

int Grafo::obtenerCantidadNodos() const {
    return cantidad_;
}

const Nodo *Grafo::obtenerNodoPorId(int id) const {
    int indice = obtenerIndiceNodo(id);
    return indice != -1 ? nodos_[indice] : nullptr;
}

Nodo *Grafo::obtenerNodoPorId(int id) {
    int indice = obtenerIndiceNodo(id);
    return indice != -1 ? nodos_[indice] : nullptr;
}

const Nodo *Grafo::obtenerNodoPorIndice(int indice) const {
    if (indice < 0 || indice >= cantidad_) {
        return nullptr;
    }
    return nodos_[indice];
}

const ListaAdyacencia *Grafo::obtenerAdyacenciaPorIndice(int indice) const {
    if (indice < 0 || indice >= cantidad_) {
        return nullptr;
    }
    return adyacencias_[indice];
}

int Grafo::obtenerIndiceNodo(int id) const {
    for (int i = 0; i < cantidad_; ++i) {
        if (nodos_[i]->obtenerId() == id) {
            return i;
        }
    }
    return -1;
}

} // namespace gps
