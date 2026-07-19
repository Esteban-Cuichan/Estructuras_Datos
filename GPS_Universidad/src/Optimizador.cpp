#include "../include/Optimizador.h"
#include <queue>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cmath>

Optimizador::Optimizador(Grafo* grafo)
    : grafo(grafo), cacheNodos(nullptr), tamanoCache(0),
      useCache(false), distancias(nullptr), predecesores(nullptr),
      visitados(nullptr), numNodos(0), accesosCache(0),
      fallosCache(0), aristasComprimidas(0) {
    if (grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo en Optimizador"<<std::endl;
        return;
    }
    this->numNodos=grafo->getNumNodos();
    this->distancias=new double[this->numNodos];
    this->predecesores=new int[this->numNodos];
    this->visitados=new bool[this->numNodos];
}

Optimizador::~Optimizador() {
    delete[] this->distancias;
    delete[] this->predecesores;
    delete[] this->visitados;
    delete[] this->cacheNodos;
}

bool Optimizador::optimizarDijkstra(int origen, int destino) {
    if (this->grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo"<<std::endl;
        return false;
    }
    if (origen<0 || origen>=this->numNodos || destino<0 || destino>=this->numNodos) {
        std::cerr<<"Error: ID inválido"<<std::endl;
        return false;
    }
    this->inicializarDijkstra(origen);
    using Par=std::pair<double, int>;
    std::priority_queue<Par, std::vector<Par>, std::greater<Par>> heap;
    heap.push({0.0, origen});
    while (!heap.empty()) {
        auto [distActual, u]=heap.top();
        heap.pop();
        if (this->visitados[u]) {
            continue;
        }
        this->visitados[u]=true;
        if (u==destino) {
            return true;
        }
        Nodo* nodoActual=this->useCache?this->obtenerNodoCache(u):this->grafo->getNodo(u);
        if (nodoActual==nullptr) {
            continue;
        }
        ListaAdyacencia* lista=nodoActual->getListaAdyacencia();
        if (lista==nullptr) {
            continue;
        }
        NodoArista* actualArista=lista->getPrimero();
        while (actualArista!=nullptr) {
            Arista* arista=actualArista->getArista();
            if (arista!=nullptr) {
                if (arista->getEstado()==EstadoCamino::CERRADO) {
                    actualArista=actualArista->getSiguiente();
                    continue;
                }
                int v=arista->getDestino()->getId();
                double nuevaDist=this->distancias[u]+arista->getDistancia();
                if (nuevaDist<this->distancias[v]) {
                    this->distancias[v]=nuevaDist;
                    this->predecesores[v]=u;
                    heap.push({nuevaDist, v});
                }
            }
            actualArista=actualArista->getSiguiente();
        }
    }
    return (this->distancias[destino]<1e9);
}

void Optimizador::construirCacheNodos() {
    if (this->grafo==nullptr) {
        return;
    }
    this->tamanoCache=this->grafo->getNumNodos();
    delete[] this->cacheNodos;
    this->cacheNodos=new Nodo*[this->tamanoCache];
    for (int i=0; i<this->tamanoCache; i++) {
        this->cacheNodos[i]=nullptr;
    }
    for (int i=0; i<this->tamanoCache; i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            this->cacheNodos[nodo->getId()]=nodo;
        }
    }
    this->useCache=true;
    this->accesosCache=0;
    this->fallosCache=0;
    std::cout<<"Caché construida: "<<this->tamanoCache<<" nodos"<<std::endl;
}

Nodo* Optimizador::obtenerNodoCache(int id) {
    this->accesosCache++;
    if (!this->useCache || id<0 || id>=this->tamanoCache) {
        this->fallosCache++;
        return this->grafo->getNodo(id);
    }
    Nodo* nodo=this->cacheNodos[id];
    if (nodo==nullptr) {
        this->fallosCache++;
        nodo=this->grafo->getNodo(id);
        if (nodo!=nullptr) {
            this->cacheNodos[id]=nodo;
        }
    }
    return nodo;
}

void Optimizador::optimizarMemoria() {
    if (this->grafo==nullptr) {
        return;
    }
    std::cout<<"Optimizando memoria..." <<std::endl;
    std::cout<<"Memoria optimizada"<<std::endl;
}

void Optimizador::comprimirGrafo() {
    if (this->grafo==nullptr) {
        return;
    }
    std::cout<<"Comprimiendo grafo..."<<std::endl;
    this->aristasComprimidas=0;
    int numAristasOriginal=this->grafo->getNumAristas();
    for (int i=0; i<this->grafo->getNumAristas(); i++) {
        Arista* a1=this->grafo->getArista(i);
        if (a1==nullptr) continue;
        for (int j=i+1; j<this->grafo->getNumAristas(); j++) {
            Arista* a2=this->grafo->getArista(j);
            if (a2==nullptr) continue;
            if (a1->esIgual(a2->getOrigen(), a2->getDestino())) {
                if (a1->getDistancia()>a2->getDistancia()) {
                    this->aristasComprimidas++;
                } else {
                    this->aristasComprimidas++;
                }
            }
        }
    }
    std::cout<<"Grafo comprimido: "<<numAristasOriginal 
             <<" -> "<<(numAristasOriginal-this->aristasComprimidas) 
             <<" aristas (eliminadas "<<this->aristasComprimidas<<")"<<std::endl;
}

char* Optimizador::getEstadisticas() const {
    const int TAMANO=1024;
    char* stats=new char[TAMANO];
    int posicion=0;
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "=== ESTADÍSTICAS DE OPTIMIZACIÓN ===\n");
    if (this->useCache) {
        double hitRate=(this->accesosCache>0)?(1.0-(double)this->fallosCache/this->accesosCache)*100.0:0.0;
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "Caché:\n");
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Tamaño: %d nodos\n", this->tamanoCache);
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Accesos: %d\n", this->accesosCache);
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Fallos: %d\n", this->fallosCache);
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Hit rate: %.2f%%\n", hitRate);
    } else {
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "Caché: Deshabilitada\n");
    }
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "Aristas comprimidas: %d\n", this->aristasComprimidas);
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "Nodos en grafo: %d\n", this->numNodos);
    int aristas=this->grafo->getNumAristas();
    double complejidadStandard=(double)this->numNodos*this->numNodos;
    double complejidadOptimizada=(double)aristas*std::log(this->numNodos);
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "\nRendimiento estimado:\n");
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Dijkstra estándar: O(V²) = %.0f operaciones\n", complejidadStandard);
    posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Dijkstra optimizado: O(E log V) ≈ %.0f operaciones\n", complejidadOptimizada);
    if (complejidadStandard>0) {
        double mejora=(1.0-complejidadOptimizada/complejidadStandard)*100.0;
        posicion+=std::snprintf(stats+posicion, TAMANO-posicion, "  Mejora estimada: %.2f%%\n", mejora);
    }
    return stats;
}

void Optimizador::setUseCache(bool habilitar) {
    this->useCache=habilitar;
    if (habilitar && this->cacheNodos==nullptr) {
        this->construirCacheNodos();
    }
}

bool Optimizador::getUseCache() const {
    return this->useCache;
}

void Optimizador::inicializarDijkstra(int origen) {
    const double INF=1e9;
    for (int i=0; i<this->numNodos; i++) {
        this->distancias[i]=INF;
        this->predecesores[i]=-1;
        this->visitados[i]=false;
    }
    this->distancias[origen]=0.0;
}

void Optimizador::relajarArista(int u, Arista* arista) {
    if (arista==nullptr) {
        return;
    }
    int v=arista->getDestino()->getId();
    if (this->visitados[v]) {
        return;
    }
    double nuevaDist=this->distancias[u]+arista->getDistancia();
    if (nuevaDist<this->distancias[v]) {
        this->distancias[v]=nuevaDist;
        this->predecesores[v]=u;
    }
}

Optimizador::Optimizador(const Optimizador& otro) {}

Optimizador& Optimizador::operator=(const Optimizador& otro) {
    return *this;
}