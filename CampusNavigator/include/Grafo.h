#ifndef GRAFO_H
#define GRAFO_H

#include <string>

using namespace std;

class Nodo;
class ListaNodos;

class Grafo {
private:
    ListaNodos* nodos;

public:
    Grafo();
    ~Grafo();

    bool agregarNodo(int id, const string& nombre);

    bool agregarConexion(int idOrigen,int idDestino,double distancia,double tiempo);
    bool agregarConexionBidireccional(int idNodoA,int idNodoB,double distancia,double tiempo);

    Nodo* buscarNodo(int id) const;

    ListaNodos* getNodos() const;

    int getCantidadNodos() const;
    bool estaVacio() const;

    void reiniciarNodos();
    void mostrarNodos() const;
    void mostrarConexiones() const;
};

#endif