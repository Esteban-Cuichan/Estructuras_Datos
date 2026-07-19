#include "../include/Grafo.h"
#include "../include/ListaAdyacencia.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

Grafo::Grafo():numNodos(0),numAristas(0),capacidadNodos(CAPACIDAD_INICIAL),capacidadAristas(CAPACIDAD_INICIAL){
    this->nodos=new Nodo*[this->capacidadNodos];
    this->aristas=new Arista*[this->capacidadAristas];
    for (int i=0; i<this->capacidadNodos; i++) {
        this->nodos[i]=nullptr;
    }
    for (int i=0; i<this->capacidadAristas; i++) {
        this->aristas[i]=nullptr;
    }
}

Grafo::~Grafo() {
    for (int i=0; i<this->numNodos; i++) {
        delete this->nodos[i];
        this->nodos[i]=nullptr;
    }
    for (int i=0; i<this->numAristas; i++) {
        delete this->aristas[i];
        this->aristas[i]=nullptr;
    }
    delete[] this->nodos;
    delete[] this->aristas;
    this->numNodos=0;
    this->numAristas=0;
    this->capacidadNodos=0;
    this->capacidadAristas=0;
}

bool Grafo::redimensionarNodos() {
    int nuevaCapacidad=this->capacidadNodos*FACTOR_INCREMENTO;
    Nodo** nuevosNodos=new Nodo*[nuevaCapacidad];
    if (nuevosNodos==nullptr) {
        return false;
    }
    for (int i=0; i<nuevaCapacidad; i++) {
        nuevosNodos[i]=nullptr;
    }
    for (int i=0; i<this->numNodos; i++) {
        nuevosNodos[i]=this->nodos[i];
    }
    delete[] this->nodos;
    this->nodos=nuevosNodos;
    this->capacidadNodos=nuevaCapacidad;
    return true;
}

bool Grafo::redimensionarAristas() {
    int nuevaCapacidad=this->capacidadAristas*FACTOR_INCREMENTO;
    Arista** nuevasAristas=new Arista*[nuevaCapacidad];
    if (nuevasAristas==nullptr) {
        return false;
    }
    for (int i=0; i<nuevaCapacidad; i++) {
        nuevasAristas[i]=nullptr;
    }
    for (int i=0; i<this->numAristas; i++) {
        nuevasAristas[i]=this->aristas[i];
    }
    delete[] this->aristas;
    this->aristas=nuevasAristas;
    this->capacidadAristas=nuevaCapacidad;
    return true;
}

bool Grafo::agregarNodo(Nodo* nodo) {
    if (nodo==nullptr) {
        return false;
    }
    if (this->existeNodo(nodo->getId())) {
        return false;
    }
    if (this->numNodos>=this->capacidadNodos) {
        if (!this->redimensionarNodos()) {
            return false;
        }
    }
    this->nodos[this->numNodos]=nodo;
    this->numNodos++;
    return true;
}

bool Grafo::agregarArista(Arista* arista) {
    if (arista==nullptr) {
        return false;
    }
    Nodo* origen=arista->getOrigen();
    Nodo* destino=arista->getDestino();
    if (origen==nullptr || destino==nullptr) {
        return false;
    }
    if (!this->existeNodo(origen->getId()) || !this->existeNodo(destino->getId())) {
        return false;
    }
    if (this->existeArista(origen, destino)) {
        return false;
    }
    if (this->numAristas>=this->capacidadAristas) {
        if (!this->redimensionarAristas()) {
            return false;
        }
    }
    this->aristas[this->numAristas]=arista;
    this->numAristas++;
    ListaAdyacencia* listaOrigen=origen->getListaAdyacencia();
    if (listaOrigen!=nullptr) {
        listaOrigen->agregarArista(arista);
    }
    return true;
}

Nodo* Grafo::buscarNodoPorId(int id) const {
    for (int i=0; i<this->numNodos; i++) {
        if (this->nodos[i]->getId()==id) {
            return this->nodos[i];
        }
    }
    return nullptr;
}

Nodo* Grafo::buscarNodoPorNombre(const char* nombre) const {
    if (nombre==nullptr) {
        return nullptr;
    }
    for (int i=0; i<this->numNodos; i++) {
        if (std::strcmp(this->nodos[i]->getNombre(), nombre)==0) {
            return this->nodos[i];
        }
    }
    return nullptr;
}

Arista* Grafo::buscarArista(Nodo* origen, Nodo* destino) const {
    if (origen==nullptr || destino==nullptr) {
        return nullptr;
    }
    for (int i=0; i<this->numAristas; i++) {
        if (this->aristas[i]->esIgual(origen, destino)) {
            return this->aristas[i];
        }
    }
    return nullptr;
}

int Grafo::getNumNodos() const {
    return this->numNodos;
}

Nodo* Grafo::getNodo(int indice) const {
    if (indice<0 || indice>=this->numNodos) {
        return nullptr;
    }
    return this->nodos[indice];
}

int Grafo::getNumAristas() const {
    return this->numAristas;
}

Arista* Grafo::getArista(int indice) const {
    if (indice<0 || indice>=this->numAristas) {
        return nullptr;
    }
    return this->aristas[indice];
}

bool Grafo::existeNodo(int id) const {
    return (this->buscarNodoPorId(id)!=nullptr);
}

bool Grafo::existeArista(Nodo* origen, Nodo* destino) const {
    return (this->buscarArista(origen, destino)!=nullptr);
}

char* Grafo::toString() const {
    const int TAMANO_INICIAL=1024;
    const int TAMANO_INCREMENTO=512;
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
    char cabecera[100];
    std::snprintf(cabecera, sizeof(cabecera), "Grafo con %d nodos y %d aristas:\n", this->numNodos, this->numAristas);
    agregarTexto(cabecera);
    agregarTexto("Nodos:\n");
    for (int i=0; i<this->numNodos; i++) {
        char* infoNodo=this->nodos[i]->toString();
        agregarTexto("  - ");
        agregarTexto(infoNodo);
        agregarTexto("\n");
        delete[] infoNodo;
    }
    agregarTexto("Aristas:\n");
    for (int i=0; i<this->numAristas; i++) {
        char* infoArista=this->aristas[i]->toString();
        agregarTexto("  - ");
        agregarTexto(infoArista);
        agregarTexto("\n");
        delete[] infoArista;
    }
    return resultado;
}

void Grafo::limpiar() {
    for (int i=0; i<this->numNodos; i++) {
        delete this->nodos[i];
        this->nodos[i]=nullptr;
    }
    for (int i=0; i<this->numAristas; i++) {
        delete this->aristas[i];
        this->aristas[i]=nullptr;
    }
    this->numNodos=0;
    this->numAristas=0;
}

Grafo::Grafo(const Grafo& otro) {}

Grafo& Grafo::operator=(const Grafo& otro) {
    return *this;
}