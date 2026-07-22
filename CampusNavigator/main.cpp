#include <iostream>
#include "include/Grafo.h"

using namespace std;

int main()
{
    Grafo grafo;

    grafo.agregarNodo(1, "Biblioteca");
    grafo.agregarNodo(2, "Cruce Central");
    grafo.agregarNodo(3, "Cafeteria");
    grafo.agregarNodo(4, "Laboratorios");
    grafo.agregarNodo(5, "Auditorio");

    grafo.agregarConexionBidireccional(
        1,
        2,
        80.0,
        57.0
    );

    grafo.agregarConexionBidireccional(
        2,
        3,
        100.0,
        71.0
    );

    grafo.agregarConexionBidireccional(
        1,
        4,
        200.0,
        142.0
    );

    grafo.agregarConexionBidireccional(
        4,
        3,
        150.0,
        107.0
    );

    grafo.agregarConexionBidireccional(
        2,
        5,
        120.0,
        86.0
    );

    grafo.mostrarNodos();
    grafo.mostrarConexiones();

    return 0;
}