#include "ListaAdyacencia.h"

namespace gps {

ListaAdyacencia::NodoAdyacente::NodoAdyacente(Arista *arista)
    : arista(arista), siguiente(nullptr) {
}

ListaAdyacencia::NodoAdyacente::~NodoAdyacente() {
    delete arista;
}

ListaAdyacencia::ListaAdyacencia()
    : cabeza_(nullptr), tamano_(0) {
}

ListaAdyacencia::~ListaAdyacencia() {
    NodoAdyacente *actual = cabeza_;
    while (actual != nullptr) {
        NodoAdyacente *siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

void ListaAdyacencia::agregarArista(Arista *arista) {
    NodoAdyacente *nuevoNodo = new NodoAdyacente(arista);
    nuevoNodo->siguiente = cabeza_;
    cabeza_ = nuevoNodo;
    ++tamano_;
}

bool ListaAdyacencia::eliminarArista(int destinoId) {
    NodoAdyacente *actual = cabeza_;
    NodoAdyacente *anterior = nullptr;

    while (actual != nullptr) {
        if (actual->arista->obtenerDestinoId() == destinoId) {
            if (anterior != nullptr) {
                anterior->siguiente = actual->siguiente;
            } else {
                cabeza_ = actual->siguiente;
            }
            delete actual;
            --tamano_;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

bool ListaAdyacencia::estaVacia() const {
    return cabeza_ == nullptr;
}

int ListaAdyacencia::obtenerTamano() const {
    return tamano_;
}

const ListaAdyacencia::NodoAdyacente *ListaAdyacencia::obtenerCabeza() const {
    return cabeza_;
}

} // namespace gps
