#ifndef NODO_H
#define NODO_H
#include <cstring>

class ListaAdyacencia;

enum class TipoNodo{
    BLOQUE,
    GARITA,
    ENTRADA,
    CRUCE,
    ESCALERA,
    RAMPA,
    PARQUEADERO,
    BAR,
    BIBLIOTECA,
    LABORATORIO,
    AUDITORIO
};

class Nodo {
    private:
        int id;
        char* nombre;
        double longitud;
        double latitud;
        TipoNodo tipo;
        ListaAdyacencia* listaAdyacencia;
    public:
        Nodo(int,const char*,double,double);
        ~Nodo();
        int getId() const;
        const char* getNombre() const;
        double getLongitud() const;
        double getLatitud() const;
        TipoNodo getTipo() const;
        void setTipo(TipoNodo);
        ListaAdyacencia* getListaAdyacencia() const;
        void setListaAdyacencia(ListaAdyacencia*);
        char* toString() const;
    private:
        Nodo(const Nodo&);
        Nodo& operator=(const Nodo&);
};
#endif 