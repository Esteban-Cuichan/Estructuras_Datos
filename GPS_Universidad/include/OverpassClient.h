#ifndef OVERPASS_CLIENT_H
#define OVERPASS_CLIENT_H

#include <string>

namespace gps {

class OverpassClient {
public:
    explicit OverpassClient(const std::string &endpoint = "https://overpass-api.de/api/interpreter");
    ~OverpassClient();

    std::string construirConsultaCampus(const std::string &area) const;
    bool descargarConsulta(const std::string &consulta, const std::string &rutaSalida) const;

private:
    std::string endpoint_;
};

} // namespace gps

#endif // OVERPASS_CLIENT_H
