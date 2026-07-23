#include <iostream>
#include "include/Grafo.h"
#include "include/Dijkstra.h"

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
    cout << "\n==========================" << endl;
    cout << "PRUEBA DEL ALGORITMO DIJKSTRA" << endl;
    cout << "==========================\n" << endl;

    Dijkstra algoritmo(&grafo);

    int origen;
    int destino;

    cout << "Ingrese nodo origen: ";
    cin >> origen;

    cout << "Ingrese nodo destino: ";
    cin >> destino;

    if (algoritmo.calcular(origen, destino))
    {
        algoritmo.imprimirRuta(destino);
    }
    else
    {
        cout << "No existe una ruta entre esos nodos." << endl;
    }

        return 0;
    }