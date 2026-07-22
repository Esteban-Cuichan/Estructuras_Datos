#ifndef OSM_PARSER_H
#define OSM_PARSER_H

#include "Grafo.h"

namespace gps {

class OsmParser {
public:
    explicit OsmParser(const std::string &rutaArchivo);
    ~OsmParser();

    bool cargarGrafo(Grafo &grafo);

private:
    std::string rutaArchivo_;

    bool parsearNodo(const std::string &linea, Nodo *&nodo);
    bool parsearCamino(const std::string &linea, Arista *&arista);
    double calcularDistanciaEuclidiana(double lat1, double lon1, double lat2, double lon2) const;
};

} // namespace gps

#endif // OSM_PARSER_H
