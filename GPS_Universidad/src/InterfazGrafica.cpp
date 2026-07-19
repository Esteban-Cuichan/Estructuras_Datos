#include "../include/InterfazGrafica.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <limits>

InterfazGrafica::InterfazGrafica()
    : grafo(nullptr), dijkstra(nullptr), renderizador(nullptr),
      cargador(nullptr), ejecutando(false),
      ultimoOrigen(-1), ultimoDestino(-1),
      ultimaRuta(nullptr), ultimaLongitudRuta(0) {}

InterfazGrafica::~InterfazGrafica() {
    if (this->ultimaRuta!=nullptr) {
        delete[] this->ultimaRuta;
        this->ultimaRuta=nullptr;
    }
    delete this->dijkstra;
    delete this->renderizador;
    delete this->cargador;
    delete this->grafo;
}

bool InterfazGrafica::inicializar(const char* archivoEdificios, const char* archivoConexiones) {
    this->grafo=new Grafo();
    if (this->grafo==nullptr) {
        this->mostrarError("No se pudo crear el grafo");
        return false;
    }
    this->cargador=new CargadorDatos(this->grafo);
    if (this->cargador==nullptr) {
        this->mostrarError("No se pudo crear el cargador de datos");
        return false;
    }
    this->mostrarInfo("Cargando edificios...");
    if (!this->cargador->cargarEdificios(archivoEdificios)) {
        this->mostrarError("Error al cargar edificios");
        return false;
    }
    this->mostrarInfo("Cargando conexiones...");
    if (!this->cargador->cargarConexiones(archivoConexiones)) {
        this->mostrarError("Error al cargar conexiones");
        return false;
    }
    this->renderizador=new Renderizador(this->grafo, true);
    if (this->renderizador==nullptr) {
        this->mostrarError("No se pudo crear el renderizador");
        return false;
    }
    this->dijkstra=new Dijkstra(this->grafo);
    if (this->dijkstra==nullptr) {
        this->mostrarError("No se pudo crear el algoritmo Dijkstra");
        return false;
    }
    this->mostrarExito("Sistema inicializado correctamente");
    return true;
}

void InterfazGrafica::ejecutar() {
    if (this->grafo==nullptr) {
        this->mostrarError("El sistema no ha sido inicializado");
        return;
    }
    this->ejecutando=true;
    while (this->ejecutando) {
        this->limpiarPantalla();
        std::cout<<"╔═══════════════════════════════════════════════════╗"<<std::endl;
        std::cout<<"║           SISTEMA DE NAVEGACIÓN GPS               ║"<<std::endl;
        std::cout<<"║               CAMPUS UNIVERSITARIO                ║"<<std::endl;
        std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
        std::cout<<"\nGrafo: "<<this->grafo->getNumNodos() 
                 <<" nodos, "<<this->grafo->getNumAristas()<<" aristas"<<std::endl;
        if (this->ultimoOrigen!=-1 && this->ultimoDestino!=-1) {
            std::cout<<"Última ruta: ";
            Nodo* orig=this->grafo->getNodo(this->ultimoOrigen);
            Nodo* dest=this->grafo->getNodo(this->ultimoDestino);
            if (orig && dest) {
                std::cout<<orig->getNombre()<<" → "<<dest->getNombre();
            }
            std::cout<<std::endl;
        }
        this->mostrarMenu();
        int opcion;
        std::cout<<"\n👉 Seleccione una opción: ";
        std::cin>>opcion;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        this->procesarOpcion(opcion);
        if (this->ejecutando) {
            this->pausar();
        }
    }
}

void InterfazGrafica::mostrarMenu() const {
    std::cout<<"\n╔═══════════════════════════════════════════════════╗"<<std::endl;
    std::cout<<"║                       MENÚ                        ║"<<std::endl;
    std::cout<<"╠═══════════════════════════════════════════════════╣"<<std::endl;
    std::cout<<"║  1. Mostrar grafo completo                        ║"<<std::endl;
    std::cout<<"║  2. Calcular ruta (Dijkstra)                      ║"<<std::endl;
    std::cout<<"║  3. Mostrar información de un nodo                ║"<<std::endl;
    std::cout<<"║  4. Mostrar estadísticas del grafo                ║"<<std::endl;
    std::cout<<"║  5. Mostrar mapa del campus                       ║"<<std::endl;
    std::cout<<"║  6. Mostrar último camino calculado               ║"<<std::endl;
    std::cout<<"║  7. Cambiar modo color                            ║"<<std::endl;
    std::cout<<"║  8. Ayuda                                         ║"<<std::endl;
    std::cout<<"║  0. Salir                                         ║"<<std::endl;
    std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
}

void InterfazGrafica::procesarOpcion(int opcion) {
    switch (opcion) {
        case 1:
            this->limpiarPantalla();
            this->mostrarGrafo();
            break;
        case 2:
            this->limpiarPantalla();
            this->calcularRuta();
            break;
        case 3:
            this->limpiarPantalla();
            this->mostrarInformacionNodo();
            break;
        case 4:
            this->limpiarPantalla();
            this->mostrarEstadisticas();
            break;
        case 5:
            this->limpiarPantalla();
            if (this->renderizador) {
                this->renderizador->renderizarMapaSimple();
            }
            break;
        case 6:
            this->limpiarPantalla();
            if (this->ultimaRuta!=nullptr && this->ultimaLongitudRuta>0) {
                this->renderizador->setRuta(this->ultimaRuta, this->ultimaLongitudRuta);
                this->renderizador->renderizarRuta();
            } else {
                this->mostrarError("No hay una ruta calculada previamente");
            }
            break;
        case 7:
            if (this->renderizador) {
                bool actual=this->renderizador->getModoColor();
                this->renderizador->setModoColor(!actual);
                this->mostrarExito(actual?"Colores desactivados":"Colores activados");
            }
            break;
        case 8:
            this->limpiarPantalla();
            this->mostrarAyuda();
            break;
        case 0:
            this->mostrarInfo("Saliendo del sistema...");
            this->ejecutando=false;
            break;
        default:
            this->mostrarError("Opción no válida");
            break;
    }
}

void InterfazGrafica::mostrarGrafo() const {
    if (this->renderizador) {
        this->renderizador->renderizarGrafo();
    }
}

void InterfazGrafica::calcularRuta() {
    int origen=this->seleccionarNodo("Seleccione el nodo ORIGEN");
    if (origen==-1) {
        this->mostrarError("Selección cancelada");
        return;
    }
    int destino=this->seleccionarNodo("Seleccione el nodo DESTINO");
    if (destino==-1) {
        this->mostrarError("Selección cancelada");
        return;
    }
    if (origen==destino) {
        this->mostrarError("El origen y destino no pueden ser el mismo");
        return;
    }
    this->mostrarInfo("Calculando ruta...");
    bool exito=this->dijkstra->ejecutar(origen, destino);
    if (!exito) {
        this->mostrarError("No se encontró un camino entre los nodos");
        return;
    }
    this->ultimoOrigen=origen;
    this->ultimoDestino=destino;
    if (this->ultimaRuta!=nullptr) {
        delete[] this->ultimaRuta;
        this->ultimaRuta=nullptr;
    }
    this->ultimaRuta=this->dijkstra->getCaminoCompleto(destino);
    if (this->ultimaRuta==nullptr) {
        this->mostrarError("Error al reconstruir la ruta");
        return;
    }
    this->ultimaLongitudRuta=0;
    int temp=destino;
    while (temp!=-1) {
        this->ultimaLongitudRuta++;
        temp=this->dijkstra->getPredecesor(temp);
    }
    this->renderizador->setRuta(this->ultimaRuta, this->ultimaLongitudRuta);
    this->renderizador->renderizarRuta();
    std::cout<<"\n--- INSTRUCCIONES DE NAVEGACIÓN ---"<<std::endl;
    for (int i=0; i<this->ultimaLongitudRuta; i++) {
        Nodo* nodo=this->grafo->getNodo(this->ultimaRuta[i]);
        if (nodo) {
            std::cout<<"  "<<(i+1)<<". "<<nodo->getNombre();
            if (i<this->ultimaLongitudRuta-1) {
                double dist=this->dijkstra->getDistancia(this->ultimaRuta[i+1])-this->dijkstra->getDistancia(this->ultimaRuta[i]);
                if (dist>0) {
                    std::cout<<" ("<<dist<<" m)";
                }
            }
            std::cout<<std::endl;
        }
    }
    double distanciaTotal=this->dijkstra->getDistancia(destino);
    this->mostrarExito(("Ruta encontrada! Distancia total: "+std::to_string(distanciaTotal)+" metros").c_str());
}

void InterfazGrafica::mostrarInformacionNodo() const {
    int id=this->seleccionarNodo("Seleccione el nodo a consultar");
    if (id==-1) {
        this->mostrarError("Selección cancelada");
        return;
    }
    Nodo* nodo=this->grafo->getNodo(id);
    if (nodo==nullptr) {
        this->mostrarError("Nodo no encontrado");
        return;
    }
    std::cout<<"\n╔═══════════════════════════════════════════════════╗"<<std::endl;
    std::cout<<"║           INFORMACIÓN DEL NODO                    ║"<<std::endl;
    std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
    char* info=nodo->toString();
    std::cout<<"  "<<info<<std::endl;
    delete[] info;
    ListaAdyacencia* lista=nodo->getListaAdyacencia();
    if (lista!=nullptr) {
        std::cout<<"\nConexiones ("<<lista->getTamano()<<"):"<<std::endl;
        NodoArista* actual=lista->getPrimero();
        int contador=0;
        while (actual!=nullptr) {
            Arista* arista=actual->getArista();
            if (arista!=nullptr) {
                Nodo* destinoNode=arista->getDestino();
                if (destinoNode!=nullptr) {
                    std::cout<<"  "<<(++contador)<<". → "<<destinoNode->getNombre()
                             <<" ("<<arista->getDistancia()<<" m, "
                             <<"Estado: "<<static_cast<int>(arista->getEstado())<<")"
                             <<std::endl;
                }
            }
            actual=actual->getSiguiente();
        }
    }
}

void InterfazGrafica::mostrarEstadisticas() const {
    if (this->renderizador) {
        this->renderizador->renderizarInformacion();
    }
}

void InterfazGrafica::mostrarAyuda() const {
    std::cout<<"\n╔═══════════════════════════════════════════════════╗"<<std::endl;
    std::cout<<"║                      AYUDA                        ║"<<std::endl;
    std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
    std::cout<<"\nSISTEMA DE NAVEGACIÓN GPS - CAMPUS UNIVERSITARIO"<<std::endl;
    std::cout<<"\n¿Cómo usar el sistema?"<<std::endl;
    std::cout<<"  1. Seleccione 'Calcular ruta' del menú principal"<<std::endl;
    std::cout<<"  2. Elija el nodo de origen (por nombre o ID)"<<std::endl;
    std::cout<<"  3. Elija el nodo de destino"<<std::endl;
    std::cout<<"  4. El sistema mostrará la ruta óptima"<<std::endl;
    std::cout<<"\nCaracterísticas del sistema:"<<std::endl;
    std::cout<<"  • Algoritmo de Dijkstra para rutas más cortas"<<std::endl;
    std::cout<<"  • Visualización del grafo con colores"<<std::endl;
    std::cout<<"  • Mapa simplificado del campus"<<std::endl;
    std::cout<<"  • Información detallada de nodos"<<std::endl;
    std::cout<<"  • Estadísticas del grafo"<<std::endl;
    std::cout<<"  • Soporte para caminos cerrados/mantenimiento"<<std::endl;
    std::cout<<"\nTipos de nodos disponibles:"<<std::endl;
    std::cout<<"  • EDIFICIO, GARITA, ENTRADA, CRUCE"<<std::endl;
    std::cout<<"  • ESCALERA, RAMPA, PARQUEADERO"<<std::endl;
    std::cout<<"  • CAFETERIA, BIBLIOTECA, LABORATORIO, AUDITORIO"<<std::endl;
    std::cout<<"\nOpciones del menú:"<<std::endl;
    std::cout<<"  1. Ver todo el grafo"<<std::endl;
    std::cout<<"  2. Calcular ruta"<<std::endl;
    std::cout<<"  3. Ver información de un nodo"<<std::endl;
    std::cout<<"  4. Ver estadísticas"<<std::endl;
    std::cout<<"  5. Ver mapa del campus"<<std::endl;
    std::cout<<"  6. Ver última ruta"<<std::endl;
    std::cout<<"  7. Activar/desactivar colores"<<std::endl;
    std::cout<<"  8. Ayuda"<<std::endl;
    std::cout<<"  0. Salir"<<std::endl;
}

int InterfazGrafica::seleccionarNodo(const char* mensaje) const {
    std::cout<<"\n╔═══════════════════════════════════════════════════╗"<<std::endl;
    std::cout<<"║  "<<mensaje<<"  ║"<<std::endl;
    std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
    std::cout<<"\nNodos disponibles:"<<std::endl;
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            std::cout<<"  "<<nodo->getId()<<". "<<nodo->getNombre() 
                     <<" ("<<static_cast<int>(nodo->getTipo())<<")"<<std::endl;
        }
    }
    std::cout<<"\nIngrese ID del nodo (0 para cancelar): ";
    int id;
    std::cin>>id;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (id==0) {
        return -1;
    }
    Nodo* nodo=this->grafo->getNodo(id);
    if (nodo==nullptr) {
        this->mostrarError("ID de nodo no válido");
        return -1;
    }
    return id;
}

void InterfazGrafica::limpiarPantalla() const {
#ifdef _WIN32
    system("cls");
#else
    for (int i=0;i<50;i++) std::cout<<'\n';
#endif
}

void InterfazGrafica::pausar() const {
    std::cout<<"\nPresione Enter para continuar...";
    std::cin.get();
}

void InterfazGrafica::mostrarError(const char* mensaje) const {
    std::cout<<"ERROR: "<<mensaje<<std::endl;
}

void InterfazGrafica::mostrarExito(const char* mensaje) const {
    std::cout<<mensaje<<std::endl;
}

void InterfazGrafica::mostrarInfo(const char* mensaje) const {
    std::cout<<mensaje<<std::endl;
}

InterfazGrafica::InterfazGrafica(const InterfazGrafica& otro) {}

InterfazGrafica& InterfazGrafica::operator=(const InterfazGrafica& otro) {
    return *this;
}