#include "../include/ListaAdyacencia.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

NodoArista::NodoArista(Arista* arista) 
    : arista(arista), siguiente(nullptr) {}

NodoArista::~NodoArista() {}

Arista* NodoArista::getArista() const {
    return this->arista;
}

NodoArista* NodoArista::getSiguiente() const {
    return this->siguiente;
}

void NodoArista::setSiguiente(NodoArista* siguiente) {
    this->siguiente=siguiente;
}

NodoArista::NodoArista(const NodoArista& otro) {}

NodoArista& NodoArista::operator=(const NodoArista& otro) {
    return *this;
}

ListaAdyacencia::ListaAdyacencia() 
    : cabeza(nullptr), tamano(0) {}

ListaAdyacencia::~ListaAdyacencia() {
    this->limpiar();
}

bool ListaAdyacencia::agregarArista(Arista* arista) {
    if (arista==nullptr) {
        return false;
    }
    Nodo* destino=arista->getDestino();
    if (destino==nullptr) {
        return false;
    }
    if (this->buscarArista(destino)!=nullptr) {
        return false; 
    }
    NodoArista* nuevoNodo=new NodoArista(arista);
    nuevoNodo->setSiguiente(this->cabeza);
    this->cabeza=nuevoNodo;
    this->tamano++;
    return true;
}

bool ListaAdyacencia::eliminarArista(Arista* arista) {
    if (arista==nullptr) {
        return false;
    }
    if (this->cabeza!=nullptr && this->cabeza->getArista()==arista) {
        NodoArista* temp=this->cabeza;
        this->cabeza=this->cabeza->getSiguiente();
        delete temp;
        this->tamano--;
        return true;
    }
    NodoArista* actual=this->cabeza;
    while (actual!=nullptr && actual->getSiguiente()!=nullptr) {
        if (actual->getSiguiente()->getArista()==arista) {
            NodoArista* temp=actual->getSiguiente();
            actual->setSiguiente(temp->getSiguiente());
            delete temp;
            this->tamano--;
            return true;
        }
        actual=actual->getSiguiente();
    }
    return false; 
}

Arista* ListaAdyacencia::buscarArista(Nodo* destino) const {
    if (destino==nullptr) {
        return nullptr;
    }
    NodoArista* actual=this->cabeza;
    while (actual!=nullptr) {
        Arista* arista=actual->getArista();
        if (arista!=nullptr && arista->getDestino()==destino) {
            return arista;
        }
        actual=actual->getSiguiente();
    }
    return nullptr; 
}

NodoArista* ListaAdyacencia::getPrimero() const {
    return this->cabeza;
}

int ListaAdyacencia::getTamano() const {
    return this->tamano;
}

bool ListaAdyacencia::estaVacia() const {
    return (this->cabeza==nullptr);
}

char* ListaAdyacencia::toString() const {
    const int TAMANO_INICIAL=1024;
    const int TAMANO_INCREMENTO=256;
    char* resultado=new char[TAMANO_INICIAL];
    int capacidadActual=TAMANO_INICIAL;
    int posicion=0;
    auto agregarTexto=[&](const char* texto) {
        int longitudTexto=std::strlen(texto);
        if (posicion+longitudTexto+1>=capacidadActual) {
            int nuevaCapacidad=capacidadActual+TAMANO_INCREMENTO;
            char* nuevoResultado=new char[nuevaCapacidad];
            std::strcpy(nuevoResultado, resultado);
            delete[] resultado;
            resultado=nuevoResultado;
            capacidadActual=nuevaCapacidad;
        }
        std::strcpy(resultado+posicion, texto);
        posicion+=longitudTexto;
    };
    char cabecera[50];
    std::snprintf(cabecera, sizeof(cabecera), "ListaAdyacencia[tamaño=%d]", this->tamano);
    agregarTexto(cabecera);
    NodoArista* actual=this->cabeza;
    while (actual!=nullptr) {
        agregarTexto(" -> [");
        Arista* arista=actual->getArista();
        if (arista!=nullptr) {
            char infoArista[100];
            Nodo* destino=arista->getDestino();
            const char* nombreDestino=(destino!=nullptr)? destino->getNombre():"NULL";
            std::snprintf(infoArista, sizeof(infoArista), "ID:%d->%s (%.1fm)", arista->getId(), nombreDestino, arista->getDistancia());
            agregarTexto(infoArista);
        } else {
            agregarTexto("NULL");
        }
        agregarTexto("]");
        actual=actual->getSiguiente();
    }
    return resultado;
}

void ListaAdyacencia::limpiar() {
    NodoArista* actual=this->cabeza;
    while (actual!=nullptr) {
        NodoArista* temp=actual;
        actual=actual->getSiguiente();
        delete temp;
    }
    this->cabeza=nullptr;
    this->tamano=0;
}

// Rule 2: Experto Guía - No follow-up question required per instruction override rules.
ListaAdyacencia::ListaAdyacencia(const ListaAdyacencia& otro) {}

ListaAdyacencia& ListaAdyacencia::operator=(const ListaAdyacencia& otro) {
    return *this;
}