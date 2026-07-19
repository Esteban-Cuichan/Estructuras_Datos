#ifndef RENDERIZADOR_H
#define RENDERIZADOR_H

#include "Grafo.h"
#include "ListaAdyacencia.h"

class Renderizador {
private:
    Grafo* grafo;
    int* ruta;
    int longitudRuta;
    bool modoColor;

    const char* obtenerSimboloTipo(TipoNodo tipo) const;
    const char* obtenerColorTipo(TipoNodo tipo) const;
    const char* obtenerColorEstado(EstadoCamino estado) const;
    const char* obtenerColorRuta() const;
    bool estaEnRuta(int id) const;
    void imprimirLinea(char caracter, int cantidad) const;

public:
    Renderizador(Grafo* grafo, bool usarColor=true);
    ~Renderizador();
    void setRuta(int* ruta, int longitud);
    void renderizarGrafo() const;
    void renderizarRuta() const;
    void renderizarNodo(Nodo* nodo, bool resaltar=false) const;
    void renderizarArista(Arista* arista, bool enRuta=false) const;
    void renderizarLeyenda() const;
    void renderizarInformacion() const;
    void renderizarMapaSimple() const;
    void setModoColor(bool habilitar);
    bool getModoColor() const;

private:
    Renderizador(const Renderizador& otro);
    Renderizador& operator=(const Renderizador& otro);
};

#endif