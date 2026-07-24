#include <iostream>
#include <windows.h>
#include <clocale>

#include "Grafo.h"
#include "OSMParser.h"
#include "ListaPuntoInteres.h"

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");

    Grafo grafo;
    OSMParser parser;
    ListaPuntoInteres listaPuntos;

    if (!parser.cargarMapa(
            "data/mapa.osm",
            &grafo,
            &listaPuntos
        ))
    {
        cout << "No se pudo leer el mapa."
             << endl;

        return 1;
    }

    cout << "\nPuntos de interes cargados: "
         << listaPuntos.getCantidad()
         << endl;

    listaPuntos.mostrarTodos();

    return 0;
}