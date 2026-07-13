#ifndef ARBOL_ROJO_NEGRO_H
#define ARBOL_ROJO_NEGRO_H

#include "Nodo.h"
#include <map>
#include <vector>
#include <memory>

class ArbolRojoNegro
{
private:
    Nodo* raiz;
    Nodo* ultimoInsertado;
    int cantidadNodos;
    int cantidadRojos;
    int cantidadNegros;

    // Rotaciones
    void rotacionIzquierda(Nodo* nodo);
    void rotacionDerecha(Nodo* nodo);

    // Inserción
    void insertarBST(Nodo* raizActual, Nodo* nodo);
    void arreglarInsercion(Nodo* nodo);

    // Métodos auxiliares
    void actualizarContadores();
    void contarColores(Nodo* nodo);  // <-- AGREGAR ESTA LÍNEA
    void destruirArbol(Nodo* nodo);
    Nodo* minimo(Nodo* nodo) const;
    Nodo* maximo(Nodo* nodo) const;
    int obtenerAltura(Nodo* nodo) const;

    // Recorridos
    void inorden(Nodo* nodo, std::vector<int>& resultado) const;
    void recolectarNodos(Nodo* nodo, std::map<int, std::pair<int, Nodo*>>& mapa, 
                        int x, int y, int nivel) const;

public:
    // Constructor y destructor
    ArbolRojoNegro();
    ~ArbolRojoNegro();

    // Operaciones principales
    void insertar(int valor);
    bool buscar(int valor) const;
    void limpiar();

    // Getters
    Nodo* getRaiz() const;
    Nodo* getUltimoInsertado() const;
    int getCantidadNodos() const;
    int getCantidadRojos() const;
    int getCantidadNegros() const;
    int getAltura() const;

    // Para visualización
    void obtenerPosicionesNodos(std::map<int, std::pair<int, Nodo*>>& mapa) const;
    void obtenerRecorridoInorden(std::vector<int>& resultado) const;
};

#endif