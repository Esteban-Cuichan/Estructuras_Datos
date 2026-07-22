#include "OsmParser.h"
#include "Nodo.h"
#include "Arista.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <unordered_map>

namespace gps {

OsmParser::OsmParser(const std::string &rutaArchivo)
    : rutaArchivo_(rutaArchivo) {
}

OsmParser::~OsmParser() {
}

bool OsmParser::cargarGrafo(Grafo &grafo) {
    std::ifstream archivo(rutaArchivo_);
    if (!archivo.is_open()) {
        return false;
    }

    std::unordered_map<int, Nodo *> nodosMap;
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.find("<node") != std::string::npos) {
            Nodo *nodo = nullptr;
            if (parsearNodo(linea, nodo) && nodo != nullptr) {
                grafo.agregarNodo(nodo);
                nodosMap[nodo->obtenerId()] = nodo;
            }
        } else if (linea.find("<way") != std::string::npos) {
            std::vector<int> miembros;
            double latAnterior = 0.0;
            double lonAnterior = 0.0;
            while (std::getline(archivo, linea) && linea.find("</way>") == std::string::npos) {
                if (linea.find("<nd ref=") != std::string::npos) {
                    std::istringstream stream(linea);
                    std::string token;
                    while (stream >> token) {
                        if (token.find("ref=") != std::string::npos) {
                            int refId = std::stoi(token.substr(token.find('=') + 2));
                            miembros.push_back(refId);
                        }
                    }
                }
            }
            for (size_t i = 1; i < miembros.size(); ++i) {
                int origenId = miembros[i - 1];
                int destinoId = miembros[i];
                const Nodo *nodoOrigen = nodosMap.count(origenId) ? nodosMap[origenId] : nullptr;
                const Nodo *nodoDestino = nodosMap.count(destinoId) ? nodosMap[destinoId] : nullptr;
                if (nodoOrigen != nullptr && nodoDestino != nullptr) {
                    double distancia = calcularDistanciaEuclidiana(
                        nodoOrigen->obtenerLatitud(), nodoOrigen->obtenerLongitud(),
                        nodoDestino->obtenerLatitud(), nodoDestino->obtenerLongitud());
                    Arista *arista = new Arista(origenId, destinoId, distancia);
                    grafo.agregarArista(arista);
                }
            }
        }
    }
    return true;
}

bool OsmParser::parsearNodo(const std::string &linea, Nodo *&nodo) {
    int id = -1;
    double lat = 0.0;
    double lon = 0.0;
    std::string nombre = "Nodo";

    std::istringstream stream(linea);
    std::string token;
    while (stream >> token) {
        if (token.find("id=") != std::string::npos) {
            id = std::stoi(token.substr(token.find('=') + 2));
        } else if (token.find("lat=") != std::string::npos) {
            lat = std::stod(token.substr(token.find('=') + 5));
        } else if (token.find("lon=") != std::string::npos) {
            lon = std::stod(token.substr(token.find('=') + 5));
        }
    }

    if (id == -1) {
        return false;
    }

    nodo = new Nodo(id, nombre, lat, lon);
    return true;
}

bool OsmParser::parsearCamino(const std::string &linea, Arista *&arista) {
    (void)linea;
    (void)arista;
    return false;
}

double OsmParser::calcularDistanciaEuclidiana(double lat1, double lon1, double lat2, double lon2) const {
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    return std::sqrt(dLat * dLat + dLon * dLon) * 111000.0;
}

} // namespace gps
