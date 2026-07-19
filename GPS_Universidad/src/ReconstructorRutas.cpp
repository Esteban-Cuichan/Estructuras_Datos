#include "../include/ReconstructorRutas.h"
#include <cstdio>
#include <cstring>
#include <iostream>

ReconstructorRutas::ReconstructorRutas(Grafo* grafo, int* predecesores)
    : grafo(grafo), predecesores(predecesores), 
      numNodos(0), origen(-1), destino(-1),
      camino(nullptr), longitudCamino(0),
      distanciasTramo(nullptr), distanciaTotal(0.0) {
    if (grafo!=nullptr) {
        this->numNodos=grafo->getNumNodos();
    }
}

ReconstructorRutas::~ReconstructorRutas() {
    this->limpiarCamino();
}

bool ReconstructorRutas::reconstruir(int origen, int destino) {
    this->limpiarCamino();
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
    if (this->predecesores==nullptr) {
        std::cerr<<"Error: Predecesores nulos"<<std::endl;
        return false;
    }
    this->origen=origen;
    this->destino=destino;
    if (!this->reconstruirCamino(origen, destino)) {
        return false;
    }
    this->calcularDistanciasTramo();
    return true;
}

bool ReconstructorRutas::reconstruirCamino(int origen, int destino) {
    int contador=0;
    int actual=destino;
    while (actual!=-1 && actual!=origen) {
        contador++;
        actual=this->predecesores[actual];
    }
    if (actual!=origen) {
        return false;
    }
    contador++;
    this->camino=new int[contador];
    this->longitudCamino=contador;
    int* temp=new int[contador];
    actual=destino;
    int idx=contador-1;
    while (actual!=-1 && idx>=0) {
        temp[idx]=actual;
        idx--;
        actual=this->predecesores[actual];
    }
    for (int i=0; i<contador; i++) {
        this->camino[i]=temp[i];
    }
    delete[] temp;
    return true;
}

void ReconstructorRutas::calcularDistanciasTramo() {
    if (this->distanciasTramo!=nullptr) {
        delete[] this->distanciasTramo;
        this->distanciasTramo=nullptr;
    }
    if (this->camino==nullptr || this->longitudCamino<2) {
        this->distanciaTotal=0.0;
        return;
    }
    int numTramos=this->longitudCamino-1;
    this->distanciasTramo=new double[numTramos];
    this->distanciaTotal=0.0;
    for (int i=0; i<numTramos; i++) {
        int idOrigen=this->camino[i];
        int idDestino=this->camino[i+1];
        Nodo* nodoOrigen=this->grafo->getNodo(idOrigen);
        Nodo* nodoDestino=this->grafo->getNodo(idDestino);
        if (nodoOrigen!=nullptr && nodoDestino!=nullptr) {
            Arista* arista=this->grafo->buscarArista(nodoOrigen, nodoDestino);
            if (arista!=nullptr) {
                this->distanciasTramo[i]=arista->getDistancia();
            } else {
                arista=this->grafo->buscarArista(nodoDestino, nodoOrigen);
                if (arista!=nullptr) {
                    this->distanciasTramo[i]=arista->getDistancia();
                } else {
                    this->distanciasTramo[i]=0.0;
                }
            }
        } else {
            this->distanciasTramo[i]=0.0;
        }
        this->distanciaTotal+=this->distanciasTramo[i];
    }
}

void ReconstructorRutas::limpiarCamino() {
    if (this->camino!=nullptr) {
        delete[] this->camino;
        this->camino=nullptr;
    }
    if (this->distanciasTramo!=nullptr) {
        delete[] this->distanciasTramo;
        this->distanciasTramo=nullptr;
    }
    this->longitudCamino=0;
    this->distanciaTotal=0.0;
}

int* ReconstructorRutas::getCamino() const {
    return this->camino;
}

int ReconstructorRutas::getLongitudCamino() const {
    return this->longitudCamino;
}

double ReconstructorRutas::getDistanciaTotal() const {
    return this->distanciaTotal;
}

double ReconstructorRutas::getDistanciaTramo(int pos) const {
    if (this->distanciasTramo==nullptr || pos<0 || pos>=this->longitudCamino-1) {
        return 0.0;
    }
    return this->distanciasTramo[pos];
}

char* ReconstructorRutas::getInstrucciones() const {
    if (this->camino==nullptr || this->longitudCamino<2) {
        char* resultado=new char[100];
        std::strcpy(resultado, "No hay ruta disponible");
        return resultado;
    }
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
    agregarTexto("=== INSTRUCCIONES DE NAVEGACIÓN ===\n\n");
    for (int i=0; i<this->longitudCamino; i++) {
        Nodo* nodo=this->grafo->getNodo(this->camino[i]);
        if (nodo==nullptr) continue;
        char paso[200];
        if (i==0) {
            std::snprintf(paso, sizeof(paso), "PASO %d: INICIO en %s\n", i+1, nodo->getNombre());
        } else if (i==this->longitudCamino-1) {
            std::snprintf(paso, sizeof(paso), "PASO %d: LLEGADA a %s (Distancia total: %.2f m)\n", i+1, nodo->getNombre(), this->distanciaTotal);
        } else {
            double distTramo=(i-1>=0 && i-1<this->longitudCamino-1)?this->distanciasTramo[i-1]:0.0;
            Nodo* siguiente=this->grafo->getNodo(this->camino[i+1]);
            const char* nombreSiguiente=(siguiente!=nullptr)?siguiente->getNombre():"Destino";
            std::snprintf(paso, sizeof(paso), "PASO %d: CONTINUAR hacia %s (%.2f m, %d de %d)\n", i+1, nombreSiguiente, distTramo, i, this->longitudCamino-1);
        }
        agregarTexto(paso);
        char tipoInfo[100];
        std::snprintf(tipoInfo, sizeof(tipoInfo), "       Tipo: %d | Coord: (%.6f, %.6f)\n", static_cast<int>(nodo->getTipo()), nodo->getLatitud(), nodo->getLongitud());
        agregarTexto(tipoInfo);
        if (i<this->longitudCamino-1) {
            agregarTexto("       ↓\n");
        }
    }
    char resumen[200];
    std::snprintf(resumen, sizeof(resumen), "\nRESUMEN: %d pasos, %.2f metros totales\n", this->longitudCamino, this->distanciaTotal);
    agregarTexto(resumen);
    return resultado;
}

char* ReconstructorRutas::toString() const {
    if (this->camino==nullptr || this->longitudCamino<2) {
        char* resultado=new char[100];
        std::strcpy(resultado, "No hay ruta reconstruida");
        return resultado;
    }
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
    agregarTexto("=== RUTA RECONSTRUIDA ===\n\n");
    char info[200];
    std::snprintf(info, sizeof(info), "Origen: %s (ID: %d)\n", this->grafo->getNodo(this->origen)->getNombre(), this->origen);
    agregarTexto(info);
    std::snprintf(info, sizeof(info), "Destino: %s (ID: %d)\n", this->grafo->getNodo(this->destino)->getNombre(), this->destino);
    agregarTexto(info);
    std::snprintf(info, sizeof(info), "Total nodos: %d\n", this->longitudCamino);
    agregarTexto(info);
    std::snprintf(info, sizeof(info), "Distancia total: %.2f metros\n\n", this->distanciaTotal);
    agregarTexto(info);
    agregarTexto("ID  | Nombre                     | Distancia acumulada\n");
    agregarTexto("----|----------------------------|--------------------\n");
    double distanciaAcumulada=0.0;
    for (int i=0; i<this->longitudCamino; i++) {
        Nodo* nodo=this->grafo->getNodo(this->camino[i]);
        if (nodo==nullptr) continue;
        char linea[200];
        if (i==0) {
            std::snprintf(linea, sizeof(linea), "%3d | %-26s | %-18.2f m\n", i+1, nodo->getNombre(), 0.0);
        } else {
            distanciaAcumulada+=this->distanciasTramo[i-1];
            std::snprintf(linea, sizeof(linea), "%3d | %-26s | %-18.2f m\n", i+1, nodo->getNombre(), distanciaAcumulada);
        }
        agregarTexto(linea);
    }
    agregarTexto("\n--- DETALLES DE TRAMOS ---\n");
    for (int i=0; i<this->longitudCamino-1; i++) {
        Nodo* origenNode=this->grafo->getNodo(this->camino[i]);
        Nodo* destinoNode=this->grafo->getNodo(this->camino[i+1]);
        if (origenNode==nullptr || destinoNode==nullptr) continue;
        char detalle[200];
        std::snprintf(detalle, sizeof(detalle), "Tramo %2d: %s -> %s: %.2f m\n", i+1, origenNode->getNombre(), destinoNode->getNombre(), this->distanciasTramo[i]);
        agregarTexto(detalle);
    }
    return resultado;
}

void ReconstructorRutas::imprimirRuta() const {
    if (this->camino==nullptr || this->longitudCamino<2) {
        std::cout<<"No hay ruta disponible"<<std::endl;
        return;
    }
    std::cout<<"\n=== RUTA ENCONTRADA ==="<<std::endl;
    std::cout<<"Origen: "<<this->grafo->getNodo(this->origen)->getNombre()<<std::endl;
    std::cout<<"Destino: "<<this->grafo->getNodo(this->destino)->getNombre()<<std::endl;
    std::cout<<"Distancia total: "<<this->distanciaTotal<<" metros"<<std::endl;
    std::cout<<"Número de pasos: "<<this->longitudCamino<<std::endl;
    std::cout<<"\n--- Camino ---"<<std::endl;
    for (int i=0; i<this->longitudCamino; i++) {
        Nodo* nodo=this->grafo->getNodo(this->camino[i]);
        if (nodo==nullptr) continue;
        std::cout<<"  "<<i+1<<". "<<nodo->getNombre();
        if (i<this->longitudCamino-1) {
            std::cout<<" ("<<this->distanciasTramo[i]<<" m)";
        }
        std::cout<<std::endl;
    }
    std::cout<<"=================="<<std::endl;
}

ReconstructorRutas::ReconstructorRutas(const ReconstructorRutas& otro) {}

ReconstructorRutas& ReconstructorRutas::operator=(const ReconstructorRutas& otro) {
    return *this;
}