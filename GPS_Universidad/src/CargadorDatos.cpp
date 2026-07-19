#include "../include/CargadorDatos.h"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>

CargadorDatos::CargadorDatos(Grafo* grafo) 
    : grafo(grafo) {
    if (grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo en CargadorDatos"<<std::endl;
    }
}

CargadorDatos::~CargadorDatos() {}

bool CargadorDatos::cargarEdificios(const char* archivo) {
    if (archivo==nullptr) {
        std::cerr<<"Error: Nombre de archivo nulo"<<std::endl;
        return false;
    }
    if (this->grafo==nullptr) {
        std::cerr<<"Error: Grafo no inicializado"<<std::endl;
        return false;
    }
    FILE* file=fopen(archivo, "r");
    if (file==nullptr) {
        std::cerr<<"Error: No se pudo abrir el archivo: "<<archivo<<std::endl;
        return false;
    }
    std::cout<<"Cargando edificios desde: "<<archivo<<std::endl;
    char buffer[MAX_LINEA];
    int numLinea=0;
    int nodosCargados=0;
    while (fgets(buffer, MAX_LINEA, file)!=nullptr) {
        numLinea++;
        buffer[strcspn(buffer, "\n")]='\0';
        buffer[strcspn(buffer, "\r")]='\0';
        if (buffer[0]=='\0') {
            continue;
        }
        if (buffer[0]=='#') {
            continue;
        }
        Nodo* nodo=this->parsearLineaNodo(buffer, numLinea);
        if (nodo!=nullptr) {
            if (this->grafo->agregarNodo(nodo)) {
                nodosCargados++;
                std::cout<<"  - Nodo cargado: "<<nodo->getNombre() 
                         <<" (ID: "<<nodo->getId()<<")"<<std::endl;
            } else {
                std::cerr<<"  - Error: No se pudo agregar nodo (ID " 
                         <<nodo->getId()<<")"<<std::endl;
                delete nodo;
            }
        }
    }
    fclose(file);
    std::cout<<"Total nodos cargados: "<<nodosCargados<<std::endl;
    return (nodosCargados>0);
}

bool CargadorDatos::cargarConexiones(const char* archivo) {
    if (archivo==nullptr) {
        std::cerr<<"Error: Nombre de archivo nulo"<<std::endl;
        return false;
    }
    if (this->grafo==nullptr) {
        std::cerr<<"Error: Grafo no inicializado"<<std::endl;
        return false;
    }
    FILE* file=fopen(archivo, "r");
    if (file==nullptr) {
        std::cerr<<"Error: No se pudo abrir el archivo: "<<archivo<<std::endl;
        return false;
    }
    std::cout<<"Cargando conexiones desde: "<<archivo<<std::endl;
    char buffer[MAX_LINEA];
    int numLinea=0;
    int aristasCargadas=0;
    while (fgets(buffer, MAX_LINEA, file)!=nullptr) {
        numLinea++;
        buffer[strcspn(buffer, "\n")]='\0';
        buffer[strcspn(buffer, "\r")]='\0';
        if (buffer[0]=='\0') {
            continue;
        }
        if (buffer[0]=='#') {
            continue;
        }
        Arista* arista=this->parsearLineaArista(buffer, numLinea);
        if (arista!=nullptr) {
            if (this->grafo->agregarArista(arista)) {
                aristasCargadas++;
                Nodo* origen=arista->getOrigen();
                Nodo* destino=arista->getDestino();
                std::cout<<"  - Arista cargada: " 
                         <<(origen?origen->getNombre():"NULL")
                         <<" -> " 
                         <<(destino?destino->getNombre():"NULL")
                         <<" (Distancia: "<<arista->getDistancia()<<"m)"<<std::endl;
            } else {
                std::cerr<<"  - Error: No se pudo agregar arista (ID " 
                         <<arista->getId()<<")"<<std::endl;
                delete arista;
            }
        }
    }
    fclose(file);
    std::cout<<"Total aristas cargadas: "<<aristasCargadas<<std::endl;
    return true;
}

bool CargadorDatos::cargarConfiguracion(const char* archivo) {
    if (archivo==nullptr) {
        std::cerr<<"Error: Nombre de archivo nulo"<<std::endl;
        return false;
    }
    FILE* file=fopen(archivo, "r");
    if (file==nullptr) {
        std::cerr<<"Error: No se pudo abrir el archivo: "<<archivo<<std::endl;
        return false;
    }
    std::cout<<"Cargando configuración desde: "<<archivo<<std::endl;
    char buffer[MAX_LINEA];
    int numLinea=0;
    while (fgets(buffer, MAX_LINEA, file)!=nullptr) {
        numLinea++;
        buffer[strcspn(buffer, "\n")]='\0';
        buffer[strcspn(buffer, "\r")]='\0';
        if (buffer[0]=='\0' || buffer[0]=='#') {
            continue;
        }
        char* igual=strchr(buffer, '=');
        if (igual==nullptr) {
            std::cerr<<"  - Línea "<<numLinea<<": Formato inválido (falta '=')"<<std::endl;
            continue;
        }
        *igual='\0';
        char* clave=this->trim(buffer);
        char* valor=this->trim(igual+1);
        if (clave!=nullptr && valor!=nullptr) {
            std::cout<<"  - Config: "<<clave<<" = "<<valor<<std::endl;
        }
    }
    fclose(file);
    return true;
}

Nodo* CargadorDatos::parsearLineaNodo(char* linea, int numLinea) {
    char** tokens=nullptr;
    int numTokens=0;
    if (!this->split(linea, '|', &tokens, &numTokens)) {
        std::cerr<<"  - Línea "<<numLinea<<": Error al parsear"<<std::endl;
        return nullptr;
    }
    if (numTokens!=5) {
        std::cerr<<"  - Línea "<<numLinea 
                 <<": Se esperaban 5 campos, se encontraron "<<numTokens<<std::endl;
        this->liberarTokens(tokens, numTokens);
        return nullptr;
    }
    try {
        int id=std::atoi(tokens[0]);
        if (id<=0) {
            std::cerr<<"  - Línea "<<numLinea<<": ID inválido: "<<tokens[0]<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        char* nombre=this->trim(tokens[1]);
        if (nombre==nullptr || nombre[0]=='\0') {
            std::cerr<<"  - Línea "<<numLinea<<": Nombre vacío"<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        double latitud=std::atof(tokens[2]);
        double longitud=std::atof(tokens[3]);
        char* tipoStr=this->trim(tokens[4]);
        TipoNodo tipo=this->obtenerTipoNodo(tipoStr);
        Nodo* nodo=new Nodo(id, nombre, latitud, longitud);
        nodo->setTipo(tipo);
        this->liberarTokens(tokens, numTokens);
        return nodo;
    } catch (...) {
        std::cerr<<"  - Línea "<<numLinea<<": Excepción al parsear"<<std::endl;
        this->liberarTokens(tokens, numTokens);
        return nullptr;
    }
}

Arista* CargadorDatos::parsearLineaArista(char* linea, int numLinea) {
    char** tokens=nullptr;
    int numTokens=0;
    if (!this->split(linea, '|', &tokens, &numTokens)) {
        std::cerr<<"  - Línea "<<numLinea<<": Error al parsear"<<std::endl;
        return nullptr;
    }
    if (numTokens!=6) {
        std::cerr<<"  - Línea "<<numLinea 
                 <<": Se esperaban 6 campos, se encontraron "<<numTokens<<std::endl;
        this->liberarTokens(tokens, numTokens);
        return nullptr;
    }
    try {
        int id=std::atoi(tokens[0]);
        if (id<=0) {
            std::cerr<<"  - Línea "<<numLinea<<": ID inválido: "<<tokens[0]<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        int idOrigen=std::atoi(tokens[1]);
        Nodo* origen=this->grafo->buscarNodoPorId(idOrigen);
        if (origen==nullptr) {
            std::cerr<<"  - Línea "<<numLinea 
                     <<": Nodo origen no existe (ID: "<<idOrigen<<")"<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        int idDestino=std::atoi(tokens[2]);
        Nodo* destino=this->grafo->buscarNodoPorId(idDestino);
        if (destino==nullptr) {
            std::cerr<<"  - Línea "<<numLinea 
                     <<": Nodo destino no existe (ID: "<<idDestino<<")"<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        double distancia=std::atof(tokens[3]);
        if (distancia<=0) {
            std::cerr<<"  - Línea "<<numLinea 
                     <<": Distancia inválida: "<<tokens[3]<<std::endl;
            this->liberarTokens(tokens, numTokens);
            return nullptr;
        }
        char* tipoStr=this->trim(tokens[4]);
        TipoCamino tipo=this->obtenerTipoCamino(tipoStr);
        char* estadoStr=this->trim(tokens[5]);
        EstadoCamino estado=this->obtenerEstadoCamino(estadoStr);
        Arista* arista=new Arista(id, origen, destino, distancia, tipo);
        arista->setEstado(estado);
        this->liberarTokens(tokens, numTokens);
        return arista;
    } catch (...) {
        std::cerr<<"  - Línea "<<numLinea<<": Excepción al parsear"<<std::endl;
        this->liberarTokens(tokens, numTokens);
        return nullptr;
    }
}

char* CargadorDatos::trim(char* str) {
    if (str==nullptr) {
        return nullptr;
    }
    while (std::isspace(*str)) {
        str++;
    }
    if (*str=='\0') {
        return str;
    }
    char* end=str+std::strlen(str)-1;
    while (end>str && std::isspace(*end)) {
        end--;
    }
    *(end+1)='\0';
    return str;
}

bool CargadorDatos::split(const char* str, char delim, char*** tokens, int* count) {
    if (str==nullptr || tokens==nullptr || count==nullptr) {
        return false;
    }
    int numTokens=1;
    const char* p=str;
    while (*p!='\0') {
        if (*p==delim) {
            numTokens++;
        }
        p++;
    }
    char** resultado=new char*[numTokens];
    int idx=0;
    char* copia=new char[std::strlen(str)+1];
    std::strcpy(copia, str);
    char* token=std::strtok(copia, &delim);
    while (token!=nullptr && idx<numTokens) {
        resultado[idx]=new char[std::strlen(token)+1];
        std::strcpy(resultado[idx], token);
        idx++;
        token=std::strtok(nullptr, &delim);
    }
    *count=idx;
    *tokens=resultado;
    delete[] copia;
    return true;
}

void CargadorDatos::liberarTokens(char** tokens, int count) {
    if (tokens==nullptr) {
        return;
    }
    for (int i=0; i<count; i++) {
        delete[] tokens[i];
    }
    delete[] tokens;
}

TipoNodo CargadorDatos::obtenerTipoNodo(const char* tipo) {
    if (tipo==nullptr) {
        return TipoNodo::BLOQUE;
    }
    if (std::strcmp(tipo, "GARITA")==0) return TipoNodo::GARITA;
    if (std::strcmp(tipo, "ENTRADA")==0) return TipoNodo::ENTRADA;
    if (std::strcmp(tipo, "CRUCE")==0) return TipoNodo::CRUCE;
    if (std::strcmp(tipo, "ESCALERA")==0) return TipoNodo::ESCALERA;
    if (std::strcmp(tipo, "RAMPA")==0) return TipoNodo::RAMPA;
    if (std::strcmp(tipo, "PARQUEADERO")==0) return TipoNodo::PARQUEADERO;
    if (std::strcmp(tipo, "BAR")==0) return TipoNodo::BAR;
    if (std::strcmp(tipo, "BIBLIOTECA")==0) return TipoNodo::BIBLIOTECA;
    if (std::strcmp(tipo, "LABORATORIO")==0) return TipoNodo::LABORATORIO;
    if (std::strcmp(tipo, "AUDITORIO")==0) return TipoNodo::AUDITORIO;
    return TipoNodo::BLOQUE;
}

TipoCamino CargadorDatos::obtenerTipoCamino(const char* tipo) {
    if (tipo==nullptr) {
        return TipoCamino::PEATONAL;
    }
    if (std::strcmp(tipo, "VEHICULAR")==0) return TipoCamino::VEHICULAR;
    if (std::strcmp(tipo, "RAMPA")==0) return TipoCamino::RAMPA;
    if (std::strcmp(tipo, "ESCALERAS")==0) return TipoCamino::ESCALERAS;
    if (std::strcmp(tipo, "MIXTO")==0) return TipoCamino::MIXTO;
    return TipoCamino::PEATONAL;
}

EstadoCamino CargadorDatos::obtenerEstadoCamino(const char* estado) {
    if (estado==nullptr) {
        return EstadoCamino::ABIERTO;
    }
    if (std::strcmp(estado, "CERRADO")==0) return EstadoCamino::CERRADO;
    if (std::strcmp(estado, "MANTENIMIENTO")==0) return EstadoCamino::MANTENIMIENTO;
    if (std::strcmp(estado, "RESTRINGIDO")==0) return EstadoCamino::RESTRINGIDO;
    return EstadoCamino::ABIERTO;
}

CargadorDatos::CargadorDatos(const CargadorDatos& otro) {}

CargadorDatos& CargadorDatos::operator=(const CargadorDatos& otro) {
    return *this;
}