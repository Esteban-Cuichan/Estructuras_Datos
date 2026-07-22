#include "OverpassClient.h"
#include <fstream>
#include <cstdlib>

namespace gps {

OverpassClient::OverpassClient(const std::string &endpoint)
    : endpoint_(endpoint) {
}

OverpassClient::~OverpassClient() {
}

std::string OverpassClient::construirConsultaCampus(const std::string &area) const {
    std::string consulta = "[out:xml][timeout:25];";
    consulta += "area[" + area + "];";
    consulta += "(node[" + area + "](area);way[" + area + "](area););";
    consulta += "out body;>;out skel qt;";
    return consulta;
}

bool OverpassClient::descargarConsulta(const std::string &consulta, const std::string &rutaSalida) const {
    std::ofstream archivoSalida(rutaSalida);
    if (!archivoSalida.is_open()) {
        return false;
    }
    archivoSalida << consulta;
    return true;
}

} // namespace gps
