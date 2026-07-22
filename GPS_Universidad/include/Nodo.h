#ifndef NODO_H
#define NODO_H

#include <string>

namespace gps {

enum class TipoNodo {
    Garita,
    Biblioteca,
    Bloque,
    Laboratorio,
    Cafeteria,
    Parqueadero,
    Entrada,
    Cruce,
    Escalera,
    Rampa,
    Auditorio,
    Desconocido
};

class Nodo {
public:
    Nodo(int id, const std::string &nombre, double latitud, double longitud, TipoNodo tipo = TipoNodo::Desconocido);
    ~Nodo();

    int obtenerId() const;
    const std::string &obtenerNombre() const;
    double obtenerLatitud() const;
    double obtenerLongitud() const;
    TipoNodo obtenerTipo() const;

    void establecerNombre(const std::string &nombre);
    void establecerTipo(TipoNodo tipo);

private:
    int id_;
    std::string nombre_;
    double latitud_;
    double longitud_;
    TipoNodo tipo_;
};

} // namespace gps

#endif // NODO_H
