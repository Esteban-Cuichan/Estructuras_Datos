#include "../include/OSMParser.h"
#include "../include/Grafo.h"
#include "../include/tinyxml2.h"
#include "../include/ListaPuntoInteres.h"
#include <iostream>
#include <string>

using namespace std;
using namespace tinyxml2;

OSMParser::OSMParser()
{
}

double OSMParser::calcularDistancia(
    double latitud1,
    double longitud1,
    double latitud2,
    double longitud2
) const
{
    return 0.0;
}

double OSMParser::calcularTiempo(
    double distancia
) const
{
    return 0.0;
}

static string obtenerEtiqueta(
    tinyxml2::XMLElement* elemento,
    const string& claveBuscada
)
{
    tinyxml2::XMLElement* etiqueta =
        elemento->FirstChildElement("tag");

    while (etiqueta != nullptr)
    {
        const char* clave = etiqueta->Attribute("k");
        const char* valor = etiqueta->Attribute("v");

        if (
            clave != nullptr &&
            valor != nullptr &&
            string(clave) == claveBuscada
        )
        {
            return valor;
        }

        etiqueta =
            etiqueta->NextSiblingElement("tag");
    }

    return "";
}

bool OSMParser::cargarMapa(
    const string& rutaArchivo,
    Grafo* grafo,
    ListaPuntoInteres* listaPuntos
)
{
    XMLDocument documento;

    XMLError resultado =
        documento.LoadFile(rutaArchivo.c_str());

    if (resultado != XML_SUCCESS)
    {
        cout << "Error al cargar el archivo OSM." << endl;
        cout << documento.ErrorStr() << endl;

        return false;
    }

    XMLElement* osm =
        documento.FirstChildElement("osm");

    if (osm == nullptr)
    {
        cout << "No se encontro el elemento <osm>."
             << endl;

        return false;
    }

    int nodosConNombre = 0;

    XMLElement* elementoNodo =
        osm->FirstChildElement("node");

    while (elementoNodo != nullptr)
    {
        long long idOSM = 0;
        double latitud = 0.0;
        double longitud = 0.0;

        bool datosValidos =
            elementoNodo->QueryInt64Attribute(
                "id",
                &idOSM
            ) == XML_SUCCESS
            &&
            elementoNodo->QueryDoubleAttribute(
                "lat",
                &latitud
            ) == XML_SUCCESS
            &&
            elementoNodo->QueryDoubleAttribute(
                "lon",
                &longitud
            ) == XML_SUCCESS;

        if (datosValidos)
        {
            string nombre;

            XMLElement* etiqueta =
                elementoNodo->FirstChildElement("tag");

            while (etiqueta != nullptr)
            {
                const char* clave =
                    etiqueta->Attribute("k");

                const char* valor =
                    etiqueta->Attribute("v");

                if (
                    clave != nullptr &&
                    valor != nullptr &&
                    string(clave) == "name"
                )
                {
                    nombre = valor;
                    break;
                }

                etiqueta =
                    etiqueta->NextSiblingElement("tag");
            }

            if (!nombre.empty())
            {
                string tipo = "node";

                string amenity =
                    obtenerEtiqueta(elementoNodo, "amenity");

                string edificio =
                    obtenerEtiqueta(elementoNodo, "building");

                string leisure =
                    obtenerEtiqueta(elementoNodo, "leisure");

                if (!amenity.empty())
                {
                    tipo = amenity;
                }
                else if (!edificio.empty())
                {
                    tipo = "building";
                }
                else if (!leisure.empty())
                {
                    tipo = leisure;
                }

                if (
                    listaPuntos != nullptr &&
                    !listaPuntos->existeNombre(nombre)
                )
                {
                    PuntoInteres punto(
                        idOSM,
                        nombre,
                        tipo,
                        latitud,
                        longitud
                    );

                    listaPuntos->agregar(punto);
                    nodosConNombre++;
                }
            }
        }

        elementoNodo =
            elementoNodo->NextSiblingElement("node");
    }

    cout << "\nTotal de nodos con nombre: "
         << nodosConNombre
         << endl;

    int viasConNombre = 0;
    int destinosWayAgregados = 0;

    XMLElement* elementoWay =
        osm->FirstChildElement("way");

    while (elementoWay != nullptr)
    {
        long long idWay = 0;

        elementoWay->QueryInt64Attribute(
            "id",
            &idWay
        );

        string nombre =
            obtenerEtiqueta(elementoWay, "name");

        if (!nombre.empty())
        {
            string tipo;
            string valorTipo;

            string edificio =
                obtenerEtiqueta(elementoWay, "building");

            string camino =
                obtenerEtiqueta(elementoWay, "highway");

            string amenity =
                obtenerEtiqueta(elementoWay, "amenity");

            string leisure =
                obtenerEtiqueta(elementoWay, "leisure");

            if (!edificio.empty())
            {
                tipo = "building";
                valorTipo = edificio;
            }
            else if (!camino.empty())
            {
                tipo = "highway";
                valorTipo = camino;
            }
            else if (!amenity.empty())
            {
                tipo = "amenity";
                valorTipo = amenity;
            }
            else if (!leisure.empty())
            {
                tipo = "leisure";
                valorTipo = leisure;
            }
            else
            {
                tipo = "otro";
                valorTipo = "sin clasificacion";
            }

            if (
                esDestinoValido(tipo, valorTipo) &&
                listaPuntos != nullptr &&
                !listaPuntos->existeNombre(nombre)
            )
            {
                double sumaLatitudes = 0.0;
                double sumaLongitudes = 0.0;

                int nodosEncontrados = 0;

                XMLElement* referenciaCentro =
                    elementoWay->FirstChildElement("nd");

                while (referenciaCentro != nullptr)
                {
                    long long idNodoReferencia = 0;

                    if (
                        referenciaCentro->QueryInt64Attribute(
                            "ref",
                            &idNodoReferencia
                        ) == XML_SUCCESS
                    )
                    {
                        XMLElement* nodoBuscado =
                            osm->FirstChildElement("node");

                        while (nodoBuscado != nullptr)
                        {
                            long long idNodoActual = 0;

                            nodoBuscado->QueryInt64Attribute(
                                "id",
                                &idNodoActual
                            );

                            if (idNodoActual == idNodoReferencia)
                            {
                                double latitudNodo = 0.0;
                                double longitudNodo = 0.0;

                                bool coordenadasValidas =
                                    nodoBuscado->QueryDoubleAttribute(
                                        "lat",
                                        &latitudNodo
                                    ) == XML_SUCCESS
                                    &&
                                    nodoBuscado->QueryDoubleAttribute(
                                        "lon",
                                        &longitudNodo
                                    ) == XML_SUCCESS;

                                if (coordenadasValidas)
                                {
                                    sumaLatitudes += latitudNodo;
                                    sumaLongitudes += longitudNodo;

                                    nodosEncontrados++;
                                }

                                break;
                            }

                            nodoBuscado =
                                nodoBuscado->NextSiblingElement("node");
                        }
                    }

                    referenciaCentro =
                        referenciaCentro->NextSiblingElement("nd");
                }

                if (nodosEncontrados > 0)
                {
                    double latitudCentro =
                        sumaLatitudes / nodosEncontrados;

                    double longitudCentro =
                        sumaLongitudes / nodosEncontrados;

                    PuntoInteres punto(
                        idWay,
                        nombre,
                        valorTipo,
                        latitudCentro,
                        longitudCentro
                    );

                    listaPuntos->agregar(punto);
                    destinosWayAgregados++;
                }
            }

            int cantidadReferencias = 0;

            XMLElement* referencia =
                elementoWay->FirstChildElement("nd");

            while (referencia != nullptr)
            {
                cantidadReferencias++;

                referencia =
                    referencia->NextSiblingElement("nd");
            }

            viasConNombre++;

            cout << "\nWay con nombre encontrado" << endl;
            cout << "ID OSM: " << idWay << endl;
            cout << "Nombre: " << nombre << endl;
            cout << "Tipo: " << tipo << endl;
            cout << "Valor: " << valorTipo << endl;
            cout << "Cantidad de nodos: "
                << cantidadReferencias
                << endl;

            cout << "------------------------" << endl;
        }

        elementoWay =
            elementoWay->NextSiblingElement("way");
    }

    cout << "\nTotal de ways con nombre: "
        << viasConNombre
        << endl;
    cout << "Destinos agregados desde ways: "
        << destinosWayAgregados
        << endl;

    cout << "\nTotal de ways con nombre: "
        << viasConNombre
        << endl;

    return true;
}

bool OSMParser::esDestinoValido(
    const string& tipo,
    const string& valorTipo
) const
{
    if (tipo == "building")
    {
        return true;
    }

    if (tipo == "amenity")
    {
        return true;
    }

    if (tipo == "leisure")
    {
        return (
            valorTipo == "park" ||
            valorTipo == "sports_centre" ||
            valorTipo == "pitch"
        );
    }

    return false;
}