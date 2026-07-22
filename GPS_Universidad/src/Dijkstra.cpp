#include "Dijkstra.h"
#include <limits>

namespace gps {

Dijkstra::Dijkstra(const Grafo &grafo)
    : grafo_(grafo), distancias_(nullptr), predecesores_(nullptr), visitado_(nullptr),
      cantidadNodos_(grafo.obtenerCantidadNodos()), origenIndice_(-1), destinoIndice_(-1),
      distanciaTotal_(0.0), nodosRecorridos_(0) {
    inicializarEstructuras();
}

Dijkstra::~Dijkstra() {
    liberarEstructuras();
}

void Dijkstra::inicializarEstructuras() {
    distancias_ = new double[cantidadNodos_];
    predecesores_ = new int[cantidadNodos_];
    visitado_ = new bool[cantidadNodos_];

    for (int i = 0; i < cantidadNodos_; ++i) {
        distancias_[i] = std::numeric_limits<double>::infinity();
        predecesores_[i] = -1;
        visitado_[i] = false;
    }
}

void Dijkstra::liberarEstructuras() {
    delete[] distancias_;
    delete[] predecesores_;
    delete[] visitado_;
    distancias_ = nullptr;
    predecesores_ = nullptr;
    visitado_ = nullptr;
}

int Dijkstra::obtenerIndiceMenorDistancia() const {
    int indiceMinimo = -1;
    double distanciaMinima = std::numeric_limits<double>::infinity();

    for (int i = 0; i < cantidadNodos_; ++i) {
        if (!visitado_[i] && distancias_[i] < distanciaMinima) {
            distanciaMinima = distancias_[i];
            indiceMinimo = i;
        }
    }
    return indiceMinimo;
}

bool Dijkstra::calcularRuta(int origenId, int destinoId) {
    origenIndice_ = grafo_.obtenerIndiceNodo(origenId);
    destinoIndice_ = grafo_.obtenerIndiceNodo(destinoId);

    if (origenIndice_ == -1 || destinoIndice_ == -1) {
        return false;
    }

    for (int i = 0; i < cantidadNodos_; ++i) {
        distancias_[i] = std::numeric_limits<double>::infinity();
        predecesores_[i] = -1;
        visitado_[i] = false;
    }

    distancias_[origenIndice_] = 0.0;

    for (int i = 0; i < cantidadNodos_; ++i) {
        int actual = obtenerIndiceMenorDistancia();
        if (actual == -1) {
            break;
        }

        visitado_[actual] = true;
        if (actual == destinoIndice_) {
            break;
        }

        const ListaAdyacencia *adyacencia = grafo_.obtenerAdyacenciaPorIndice(actual);
        if (adyacencia == nullptr) {
            continue;
        }

        const ListaAdyacencia::NodoAdyacente *nodo = adyacencia->obtenerCabeza();
        while (nodo != nullptr) {
            int vecinoId = nodo->arista->obtenerDestinoId();
            int vecinoIndice = grafo_.obtenerIndiceNodo(vecinoId);
            if (vecinoIndice != -1 && !visitado_[vecinoIndice]) {
                double nuevaDistancia = distancias_[actual] + nodo->arista->obtenerDistancia();
                if (nuevaDistancia < distancias_[vecinoIndice]) {
                    distancias_[vecinoIndice] = nuevaDistancia;
                    predecesores_[vecinoIndice] = actual;
                }
            }
            nodo = nodo->siguiente;
        }
    }

    if (distancias_[destinoIndice_] == std::numeric_limits<double>::infinity()) {
        return false;
    }

    distanciaTotal_ = distancias_[destinoIndice_];
    nodosRecorridos_ = 0;
    int actual = destinoIndice_;
    while (actual != -1) {
        nodosRecorridos_++;
        actual = predecesores_[actual];
    }
    return true;
}

int Dijkstra::obtenerNodoSiguiente(int id) const {
    int indice = grafo_.obtenerIndiceNodo(id);
    if (indice == -1 || predecesores_[indice] == -1) {
        return -1;
    }
    return grafo_.obtenerNodoPorIndice(predecesores_[indice])->obtenerId();
}

double Dijkstra::obtenerDistanciaTotal() const {
    return distanciaTotal_;
}

int Dijkstra::obtenerCantidadNodosRecorridos() const {
    return nodosRecorridos_;
}

} // namespace gps
