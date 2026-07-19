#include "../include/Renderizador.h"
#include <iostream>
#include <iomanip>
#include <cmath>


Renderizador::Renderizador(Grafo* grafo, bool usarColor)
    : grafo(grafo), ruta(nullptr), longitudRuta(0), modoColor(usarColor) {
    if (grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo en Renderizador"<<std::endl;
    }
}

Renderizador::~Renderizador() {}

void Renderizador::setRuta(int* ruta, int longitud) {
    this->ruta=ruta;
    this->longitudRuta=longitud;
}

void Renderizador::renderizarGrafo() const {
    if (this->grafo==nullptr) {
        std::cout<<"Error: Grafo nulo"<<std::endl;
        return;
    }
    this->imprimirLinea('=', 60);
    std::cout<<"          GRAFO DEL CAMPUS UNIVERSITARIO"<<std::endl;
    this->imprimirLinea('=', 60);
    this->renderizarInformacion();
    std::cout<<"\n--- NODOS ("<<this->grafo->getNumNodos()<<") ---"<<std::endl;
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            this->renderizarNodo(nodo, false);
        }
    }
    std::cout<<"\n--- ARISTAS ("<<this->grafo->getNumAristas()<<") ---"<<std::endl;
    for (int i=0; i<this->grafo->getNumAristas(); i++) {
        Arista* arista=this->grafo->getArista(i);
        if (arista!=nullptr) {
            this->renderizarArista(arista, false);
        }
    }
    this->renderizarLeyenda();
    this->imprimirLinea('=', 60);
}

void Renderizador::renderizarRuta() const {
    if (this->grafo==nullptr) {
        std::cout<<"Error: Grafo nulo"<<std::endl;
        return;
    }
    if (this->ruta==nullptr || this->longitudRuta==0) {
        std::cout<<"No hay ruta establecida"<<std::endl;
        return;
    }
    this->imprimirLinea('=', 60);
    std::cout<<"          RUTA OPTIMA ENCONTRADA"<<std::endl;
    this->imprimirLinea('=', 60);
    std::cout<<"\nRuta con "<<this->longitudRuta<<" nodos:"<<std::endl;
    for (int i=0; i<this->longitudRuta; i++) {
        Nodo* nodo=this->grafo->getNodo(this->ruta[i]);
        if (nodo!=nullptr) {
            if (this->modoColor) {
                std::cout<<this->obtenerColorRuta();
            }
            std::cout<<"  "<<(i+1)<<". "<<nodo->getNombre();
            if (this->modoColor) {
                std::cout<<"";
            }
            if (i<this->longitudRuta-1) {
                Nodo* siguiente=this->grafo->getNodo(this->ruta[i+1]);
                if (siguiente!=nullptr) {
                    Arista* arista=this->grafo->buscarArista(nodo, siguiente);
                    if (arista!=nullptr) {
                        std::cout<<" --["<<arista->getDistancia()<<"m]--> ";
                    } else {
                        std::cout<<" --> ";
                    }
                }
            }
        }
    }
    std::cout<<"\n"<<std::endl;
    std::cout<<"--- GRAFO CON RUTA RESALTADA ---"<<std::endl;
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            bool enRuta=this->estaEnRuta(nodo->getId());
            this->renderizarNodo(nodo, enRuta);
        }
    }
    this->renderizarLeyenda();
    this->imprimirLinea('=', 60);
}

void Renderizador::renderizarNodo(Nodo* nodo, bool resaltar) const {
    if (nodo==nullptr) return;
    if (this->modoColor) {
        if (resaltar) {
            std::cout<<this->obtenerColorRuta();
            std::cout<<"";
        } else {
            std::cout<<this->obtenerColorTipo(nodo->getTipo());
        }
    }
    const char* simbolo=this->obtenerSimboloTipo(nodo->getTipo());
    std::cout<<"  "<<simbolo<<" "<<nodo->getNombre();
    if (this->modoColor) {
        std::cout<<"";
    }
    std::cout<<" (ID: "<<nodo->getId()<<", ";
    if (this->modoColor) {
        std::cout<<this->obtenerColorTipo(nodo->getTipo());
    }
    std::cout<<"Tipo: "<<static_cast<int>(nodo->getTipo());
    if (this->modoColor) {
        std::cout<<"";
    }
    std::cout<<", Coord: "<<std::fixed<<std::setprecision(4)
             <<nodo->getLatitud()<<", "<<nodo->getLongitud()<<")";
    ListaAdyacencia* lista=nodo->getListaAdyacencia();
    if (lista!=nullptr) {
        std::cout<<" [Grado: "<<lista->getTamano()<<"]";
    }
    if (resaltar) {
        std::cout<<" <<< EN RUTA";
    }
    std::cout<<std::endl;
}

void Renderizador::renderizarArista(Arista* arista, bool enRuta) const {
    if (arista==nullptr) return;
    Nodo* origen=arista->getOrigen();
    Nodo* destino=arista->getDestino();
    if (origen==nullptr || destino==nullptr) return;
    if (this->modoColor) {
        if (enRuta) {
            std::cout<<this->obtenerColorRuta();
        } else {
            std::cout<<this->obtenerColorEstado(arista->getEstado());
        }
    }
    std::cout<<"  "<<origen->getNombre()<<" -> "<<destino->getNombre();
    std::cout<<" [Dist: "<<arista->getDistancia()<<"m, ";
    if (this->modoColor) {
        std::cout<<this->obtenerColorEstado(arista->getEstado());
    }
    std::cout<<"Estado: "<<static_cast<int>(arista->getEstado());
    if (this->modoColor) {
        std::cout<<"";
    }
    if (enRuta) {
        std::cout<<" <<< EN RUTA";
    }
    std::cout<<"]"<<std::endl;
}

void Renderizador::renderizarLeyenda() const {
    std::cout<<"\n--- LEYENDA ---"<<std::endl;
    std::cout<<"Nodos:"<<std::endl;
    const char* tipos[]={"EDIFICIO", "GARITA", "ENTRADA", "CRUCE", 
                          "ESCALERA", "RAMPA", "PARQUEADERO", "CAFETERIA",
                          "BIBLIOTECA", "LABORATORIO", "AUDITORIO"};
    const char* simbolos[]={"[E]", "[G]", "[P]", "[C]", 
                             "[S]", "[R]", "[A]", "[F]",
                             "[B]", "[L]", "[H]"};
    for (int i=0; i<11; i++) {
        if (this->modoColor) {
            std::cout<<this->obtenerColorTipo(static_cast<TipoNodo>(i));
        }
        std::cout<<"  "<<simbolos[i]<<" "<<tipos[i];
        if (this->modoColor) {
            std::cout<<"";
        }
        std::cout<<std::endl;
    }
    std::cout<<"\nEstados de caminos:"<<std::endl;
    if (this->modoColor) {
        std::cout<<""<<"  ABIERTO"<<""<<std::endl;
        std::cout<<""<<"  CERRADO"<<""<<std::endl;
        std::cout<<""<<"  MANTENIMIENTO"<<""<<std::endl;
        std::cout<<""<<"  RESTRINGIDO"<<""<<std::endl;
    } else {
        std::cout<<"  ABIERTO"<<std::endl;
        std::cout<<"  CERRADO"<<std::endl;
        std::cout<<"  MANTENIMIENTO"<<std::endl;
        std::cout<<"  RESTRINGIDO"<<std::endl;
    }
    if (this->modoColor) {
        std::cout<<"\n"<<this->obtenerColorRuta() 
                 <<"  RUTA RESALTADA"<<""<<std::endl;
    }
}

void Renderizador::renderizarInformacion() const {
    if (this->grafo==nullptr) return;
    std::cout<<"\n--- INFORMACIÓN DEL GRAFO ---"<<std::endl;
    std::cout<<"  Nodos: "<<this->grafo->getNumNodos()<<std::endl;
    std::cout<<"  Aristas: "<<this->grafo->getNumAristas()<<std::endl;
    int totalGrado=0;
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            ListaAdyacencia* lista=nodo->getListaAdyacencia();
            if (lista!=nullptr) {
                totalGrado+=lista->getTamano();
            }
        }
    }
    if (this->grafo->getNumNodos()>0) {
        double gradoPromedio=static_cast<double>(totalGrado)/this->grafo->getNumNodos();
        std::cout<<"  Grado promedio: "<<gradoPromedio<<std::endl;
    }
    std::cout<<"  Distribución por tipo:"<<std::endl;
    int conteoTipos[11]={0};
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            int tipo=static_cast<int>(nodo->getTipo());
            if (tipo>=0 && tipo<11) {
                conteoTipos[tipo]++;
            }
        }
    }
    const char* nombresTipos[]={"EDIFICIO", "GARITA", "ENTRADA", "CRUCE", 
                                  "ESCALERA", "RAMPA", "PARQUEADERO", "CAFETERIA",
                                  "BIBLIOTECA", "LABORATORIO", "AUDITORIO"};
    for (int i=0; i<11; i++) {
        if (conteoTipos[i]>0) {
            std::cout<<"    "<<nombresTipos[i]<<": "<<conteoTipos[i]<<std::endl;
        }
    }
}

void Renderizador::renderizarMapaSimple() const {
    if (this->grafo==nullptr) return;
    std::cout<<"\n--- MAPA SIMPLIFICADO DEL CAMPUS ---"<<std::endl;
    std::cout<<"(Representación aproximada de ubicaciones)"<<std::endl;
    double minLat=90.0, maxLat=-90.0;
    double minLon=180.0, maxLon=-180.0;
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            double lat=nodo->getLatitud();
            double lon=nodo->getLongitud();
            if (lat<minLat) minLat=lat;
            if (lat>maxLat) maxLat=lat;
            if (lon<minLon) minLon=lon;
            if (lon>maxLon) maxLon=lon;
        }
    }
    const int ANCHO=40;
    const int ALTO=20;
    char mapa[ALTO][ANCHO];
    for (int y=0; y<ALTO; y++) {
        for (int x=0; x<ANCHO; x++) {
            mapa[y][x]=' ';
        }
    }
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            double lat=nodo->getLatitud();
            double lon=nodo->getLongitud();
            int x=static_cast<int>(((lon-minLon)/(maxLon-minLon))*(ANCHO-1));
            int y=static_cast<int>(((maxLat-lat)/(maxLat-minLat))*(ALTO-1));
            if (x<0) x=0;
            if (x>=ANCHO) x=ANCHO-1;
            if (y<0) y=0;
            if (y>=ALTO) y=ALTO-1;
            bool enRuta=this->estaEnRuta(nodo->getId());
            char simbolo=enRuta?'*':'o';
            mapa[y][x]=simbolo;
            if (this->ruta!=nullptr && this->longitudRuta>0) {
                if (nodo->getId()==this->ruta[0]) {
                    mapa[y][x]='S';
                } else if (nodo->getId()==this->ruta[this->longitudRuta-1]) {
                    mapa[y][x]='E';
                }
            }
        }
    }
    std::cout<<"  ";
    for (int x=0; x<ANCHO; x++) {
        std::cout<<"-";
    }
    std::cout<<std::endl;
    for (int y=0; y<ALTO; y++) {
        std::cout<<"  |";
        for (int x=0; x<ANCHO; x++) {
            if (this->modoColor && mapa[y][x]=='*') {
                std::cout<<this->obtenerColorRuta()<<mapa[y][x]<<"";
            } else if (this->modoColor && mapa[y][x]=='S') {
                std::cout<<""<<'S'<<"";
            } else if (this->modoColor && mapa[y][x]=='E') {
                std::cout<<""<<'E'<<"";
            } else {
                std::cout<<mapa[y][x];
            }
        }
        std::cout<<"|"<<std::endl;
    }
    std::cout<<"  ";
    for (int x=0; x<ANCHO; x++) {
        std::cout<<"-";
    }
    std::cout<<std::endl;
    std::cout<<"  Leyenda: o = Nodo, * = Nodo en ruta, S = Inicio, E = Fin"<<std::endl;
}

const char* Renderizador::obtenerSimboloTipo(TipoNodo tipo) const {
    switch (tipo) {
        case TipoNodo::BLOQUE:     return "[E]";
        case TipoNodo::GARITA:       return "[G]";
        case TipoNodo::ENTRADA:      return "[P]";
        case TipoNodo::CRUCE:        return "[C]";
        case TipoNodo::ESCALERA:     return "[S]";
        case TipoNodo::RAMPA:        return "[R]";
        case TipoNodo::PARQUEADERO:  return "[A]";
        case TipoNodo::BAR:    return "[F]";
        case TipoNodo::BIBLIOTECA:   return "[B]";
        case TipoNodo::LABORATORIO:  return "[L]";
        case TipoNodo::AUDITORIO:    return "[H]";
        default:                     return "[?]";
    }
}

const char* Renderizador::obtenerColorTipo(TipoNodo tipo) const {
    if (!this->modoColor) return "";
    switch (tipo) {
        case TipoNodo::BLOQUE:     return "";
        case TipoNodo::GARITA:       return "";
        case TipoNodo::ENTRADA:      return "";
        case TipoNodo::CRUCE:        return "";
        case TipoNodo::ESCALERA:     return "";
        case TipoNodo::RAMPA:        return "";
        case TipoNodo::PARQUEADERO:  return "";
        case TipoNodo::BAR:    return "";
        case TipoNodo::BIBLIOTECA:   return "";
        case TipoNodo::LABORATORIO:  return "";
        case TipoNodo::AUDITORIO:    return "";
        default:                     return "";
    }
}

const char* Renderizador::obtenerColorEstado(EstadoCamino estado) const {
    if (!this->modoColor) return "";
    switch (estado) {
        case EstadoCamino::ABIERTO:        return "";
        case EstadoCamino::CERRADO:        return "";
        case EstadoCamino::MANTENIMIENTO:  return "";
        case EstadoCamino::RESTRINGIDO:    return "";
        default:                           return "";
    }
}

const char* Renderizador::obtenerColorRuta() const {
    return "";
}

bool Renderizador::estaEnRuta(int id) const {
    if (this->ruta==nullptr || this->longitudRuta==0) {
        return false;
    }
    for (int i=0; i<this->longitudRuta; i++) {
        if (this->ruta[i]==id) {
            return true;
        }
    }
    return false;
}

void Renderizador::imprimirLinea(char caracter, int cantidad) const {
    for (int i=0; i<cantidad; i++) {
        std::cout<<caracter;
    }
    std::cout<<std::endl;
}

void Renderizador::setModoColor(bool habilitar) {
    this->modoColor=habilitar;
}

bool Renderizador::getModoColor() const {
    return this->modoColor;
}

Renderizador::Renderizador(const Renderizador& otro) {}

Renderizador& Renderizador::operator=(const Renderizador& otro) {
    return *this;
}