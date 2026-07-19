#ifndef INTERFAZ_GRAFICA_H
#define INTERFAZ_GRAFICA_H

#include "Grafo.h"
#include "Dijkstra.h"
#include "Renderizador.h"
#include "CargadorDatos.h"
#include "ReconstructorRutas.h"

class InterfazGrafica {
private:
    Grafo* grafo;
    Dijkstra* dijkstra;
    Renderizador* renderizador;
    CargadorDatos* cargador;
    bool ejecutando;
    int ultimoOrigen;
    int ultimoDestino;
    int* ultimaRuta;
    int ultimaLongitudRuta;

    void mostrarMenu() const;
    void procesarOpcion(int opcion);
    int seleccionarNodo(const char* mensaje) const;
    void mostrarGrafo() const;
    void calcularRuta();
    void mostrarInformacionNodo() const;
    void mostrarEstadisticas() const;
    void mostrarAyuda() const;
    void limpiarPantalla() const;
    void pausar() const;
    void mostrarError(const char* mensaje) const;
    void mostrarExito(const char* mensaje) const;
    void mostrarInfo(const char* mensaje) const;

public:
    InterfazGrafica();
    ~InterfazGrafica();
    bool inicializar(const char* archivoEdificios, const char* archivoConexiones);
    void ejecutar();

private:
    InterfazGrafica(const InterfazGrafica& otro);
    InterfazGrafica& operator=(const InterfazGrafica& otro);
};

#endif