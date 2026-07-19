#include "../include/Dijkstra.h"
#include <cstdio>
#include <cstring>
#include <iostream>

const double Dijkstra::INFINITO=1e9;

Dijkstra::Dijkstra(Grafo* grafo) 
    : grafo(grafo), numNodos(0), origen(-1), destino(-1), 
      caminoEncontrado(false) {
    if (grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo en Dijkstra"<<std::endl;
        return;
    }
    this->numNodos=grafo->getNumNodos();
    this->distancias=new double[this->numNodos];
    this->predecesores=new int[this->numNodos];
    this->visitados=new bool[this->numNodos];
    for (int i=0; i<this->numNodos; i++) {
        this->distancias[i]=INFINITO;
        this->predecesores[i]=-1;
        this->visitados[i]=false;
    }
}

Dijkstra::~Dijkstra() {
    delete[] this->distancias;
    delete[] this->predecesores;
    delete[] this->visitados;
}

bool Dijkstra::ejecutar(int origen, int destino) {
    if (this->grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo"<<std::endl;
        return false;
    }
    if (origen<0 || origen>=this->numNodos) {
        std::cerr<<"Error: Origen inválido: "<<origen<<std::endl;
        return false;
    }
    if (destino<0 || destino>=this->numNodos) {
        std::cerr<<"Error: Destino inválido: "<<destino<<std::endl;
        return false;
    }
    this->origen=origen;
    this->destino=destino;
    this->caminoEncontrado=false;
    this->inicializar(origen);
    int nodosProcesados=0;
    while (nodosProcesados<this->numNodos) {
        int u=this->encontrarMinimo();
        if (u==-1) {
            break;
        }
        if (u==destino) {
            this->caminoEncontrado=true;
            break;
        }
        this->visitados[u]=true;
        nodosProcesados++;
        Nodo* nodoActual=this->grafo->getNodo(u);
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
                this->relajarArista(u, arista);
            }
            actualArista=actualArista->getSiguiente();
        }
    }
    if (!this->caminoEncontrado) {
        if (this->distancias[destino]<INFINITO) {
            this->caminoEncontrado=true;
        }
    }
    return this->caminoEncontrado;
}

void Dijkstra::inicializar(int origen) {
    for (int i=0; i<this->numNodos; i++) {
        this->distancias[i]=INFINITO;
        this->predecesores[i]=-1;
        this->visitados[i]=false;
    }
    this->distancias[origen]=0.0;
}

int Dijkstra::encontrarMinimo() {
    double minDistancia=INFINITO;
    int minNodo=-1;
    for (int i=0; i<this->numNodos; i++) {
        if (!this->visitados[i]) {
            if (this->distancias[i]<minDistancia) {
                minDistancia=this->distancias[i];
                minNodo=i;
            }
        }
    }
    return minNodo;
}

void Dijkstra::relajarArista(int actual, Arista* arista) {
    if (arista==nullptr) {
        return;
    }
    EstadoCamino estado=arista->getEstado();
    if (estado==EstadoCamino::CERRADO) {
        return; 
    }
    Nodo* nodoDestino=arista->getDestino();
    if (nodoDestino==nullptr) {
        return;
    }
    int idDestino=nodoDestino->getId();
    if (idDestino<0 || idDestino>=this->numNodos) {
        return;
    }
    if (this->visitados[idDestino]) {
        return;
    }
    double nuevaDistancia=this->distancias[actual]+arista->getDistancia();
    if (nuevaDistancia<this->distancias[idDestino]) {
        this->distancias[idDestino]=nuevaDistancia;
        this->predecesores[idDestino]=actual;
    }
}

double Dijkstra::getDistancia(int nodo) const {
    if (nodo<0 || nodo>=this->numNodos) {
        return INFINITO;
    }
    return this->distancias[nodo];
}

int Dijkstra::getPredecesor(int nodo) const {
    if (nodo<0 || nodo>=this->numNodos) {
        return -1;
    }
    return this->predecesores[nodo];
}

int* Dijkstra::getCaminoCompleto(int destino) const {
    if (destino<0 || destino>=this->numNodos) {
        return nullptr;
    }
    if (this->distancias[destino]>=INFINITO) {
        return nullptr;
    }
    int contador=0;
    int actual=destino;
    while (actual!=-1) {
        contador++;
        actual=this->predecesores[actual];
    }
    if (contador<=1 && destino!=this->origen) {
        return nullptr;
    }
    int* camino=new int[contador];
    int* temp=new int[contador];
    actual=destino;
    int idx=contador-1;
    while (actual!=-1 && idx>=0) {
        temp[idx]=actual;
        idx--;
        actual=this->predecesores[actual];
    }
    for (int i=0; i<contador; i++) {
        camino[i]=temp[i];
    }
    delete[] temp;
    return camino;
}

int Dijkstra::getNumNodos() const {
    return this->numNodos;
}

bool Dijkstra::hayCamino() const {
    return this->caminoEncontrado;
}

void Dijkstra::imprimirDistancias() const {
    std::cout<<"=== Distancias desde el nodo "<<this->origen<<" ==="<<std::endl;
    for (int i=0; i<this->numNodos; i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        const char* nombre=(nodo!=nullptr)?nodo->getNombre():"Nodo";
        if (this->distancias[i]>=INFINITO) {
            std::cout<<"  "<<nombre<<" (ID: "<<i<<"): INFINITO"<<std::endl;
        } else {
            std::cout<<"  "<<nombre<<" (ID: "<<i<<"): " 
                     <<this->distancias[i]<<" m"<<std::endl;
        }
    }
}

char* Dijkstra::toString() const {
    const int TAMANO_INICIAL=2048;
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
    std::snprintf(cabecera, sizeof(cabecera), "=== DIJKSTRA (Origen: %d, Destino: %d) ===\n", this->origen, this->destino);
    agregarTexto(cabecera);
    agregarTexto("Estado: ");
    if (this->caminoEncontrado) {
        agregarTexto("Camino encontrado\n");
    } else {
        agregarTexto("No hay camino\n");
    }
    agregarTexto("\nDistancias:\n");
    for (int i=0; i<this->numNodos; i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        const char* nombre=(nodo!=nullptr)?nodo->getNombre():"Nodo";
        char linea[200];
        if (this->distancias[i]>=INFINITO) {
            std::snprintf(linea, sizeof(linea), "  %s (ID: %d): INFINITO\n", nombre, i);
        } else {
            std::snprintf(linea, sizeof(linea), "  %s (ID: %d): %.2f m\n", nombre, i, this->distancias[i]);
        }
        agregarTexto(linea);
    }
    agregarTexto("\nPredecesores:\n");
    for (int i=0; i<this->numNodos; i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        const char* nombre=(nodo!=nullptr)?nodo->getNombre():"Nodo";
        char linea[200];
        if (this->predecesores[i]==-1) {
            std::snprintf(linea, sizeof(linea), "  %s (ID: %d): NINGUNO\n", nombre, i);
        } else {
            Nodo* pred=this->grafo->getNodo(this->predecesores[i]);
            const char* nombrePred=(pred!=nullptr)?pred->getNombre():"Nodo";
            std::snprintf(linea, sizeof(linea), "  %s (ID: %d): %s (ID: %d)\n", nombre, i, nombrePred, this->predecesores[i]);
        }
        agregarTexto(linea);
    }
    return resultado;
}

Dijkstra::Dijkstra(const Dijkstra& otro) {}

Dijkstra& Dijkstra::operator=(const Dijkstra& otro) {
    return *this;
}