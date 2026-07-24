#include "../include/GestorArchivos.h"
#include "../include/Grafo.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <exception>

using namespace std;

bool GestorArchivos::cargarNodos(
    const string& rutaArchivo,
    Grafo* grafo
)
{
    if (grafo == nullptr)
    {
        cout << "Error: el grafo no existe." << endl;
        return false;
    }

    ifstream archivo(rutaArchivo);

    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo de nodos: "
             << rutaArchivo
             << endl;

        return false;
    }

    string linea;
    int numeroLinea = 0;
    int nodosCargados = 0;

    while (getline(archivo, linea))
    {
        numeroLinea++;

        if (linea.empty())
        {
            continue;
        }

        stringstream lector(linea);

        string textoId;
        string nombre;
        string textoLatitud;
        string textoLongitud;
        cout << "Leyendo: [" << linea << "]" << endl;
        if (!getline(lector, textoId, ';') ||
            !getline(lector, nombre, ';') ||
            !getline(lector, textoLatitud, ';') ||
            !getline(lector, textoLongitud))
        {
            cout << "Linea invalida en nodos.txt: "
                 << numeroLinea
                 << endl;

            continue;
        }

        try
        {
            int id = stoi(textoId);
            double latitud = stod(textoLatitud);
            double longitud = stod(textoLongitud);

            if (nombre.empty())
            {
                cout << "Nombre vacio en la linea "
                     << numeroLinea
                     << endl;

                continue;
            }

            if (grafo->agregarNodo(
                    id,
                    nombre,
                    latitud,
                    longitud
                ))
            {
                nodosCargados++;
            }
            else
            {
                cout << "No se pudo agregar el nodo de la linea "
                     << numeroLinea
                     << ". Revise el ID, nombre o coordenadas."
                     << endl;
            }
        }
        catch (const exception&)
        {
            cout << "Datos invalidos en la linea "
                 << numeroLinea
                 << " de nodos.txt."
                 << endl;
        }
    }

    archivo.close();

    cout << "Nodos cargados: "
         << nodosCargados
         << endl;

    return nodosCargados > 0;
}

bool GestorArchivos::cargarAristas(
    const string& rutaArchivo,
    Grafo* grafo
)
{
    if (grafo == nullptr)
    {
        cout << "Error: el grafo no existe." << endl;
        return false;
    }

    ifstream archivo(rutaArchivo);

    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo de aristas: "
             << rutaArchivo
             << endl;

        return false;
    }

    string linea;
    int numeroLinea = 0;
    int aristasCargadas = 0;

    while (getline(archivo, linea))
    {
        numeroLinea++;

        if (linea.empty())
        {
            continue;
        }

        stringstream lector(linea);

        string textoOrigen;
        string textoDestino;
        string textoDistancia;
        string textoTiempo;

        if (!getline(lector, textoOrigen, ';') ||
            !getline(lector, textoDestino, ';') ||
            !getline(lector, textoDistancia, ';') ||
            !getline(lector, textoTiempo))
        {
            cout << "Linea invalida en aristas.txt: "
                 << numeroLinea
                 << endl;

            continue;
        }

        try
        {
            int idOrigen = stoi(textoOrigen);
            int idDestino = stoi(textoDestino);

            double distancia = stod(textoDistancia);
            double tiempo = stod(textoTiempo);

            if (grafo->agregarConexionBidireccional(
                    idOrigen,
                    idDestino,
                    distancia,
                    tiempo
                ))
            {
                aristasCargadas++;
            }
            else
            {
                cout << "No se pudo agregar la conexion de la linea "
                     << numeroLinea
                     << ". Revise los IDs o los valores."
                     << endl;
            }
        }
        catch (...)
        {
            cout << "Datos numericos invalidos en la linea "
                 << numeroLinea
                 << endl;
        }
    }

    archivo.close();

    cout << "Conexiones cargadas: "
         << aristasCargadas
         << endl;

    return aristasCargadas > 0;
}