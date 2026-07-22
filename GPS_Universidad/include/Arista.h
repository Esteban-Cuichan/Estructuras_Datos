#ifndef ARISTA_H
#define ARISTA_H

namespace gps {

enum class TipoCamino {
    Peatonal,
    Vehicular,
    Ciclovia,
    Sendero,
    Pasarela,
    Escalera,
    Rampa,
    Desconocido
};

enum class EstadoCamino {
    Bueno,
    EnObras,
    Cerrado,
    Restringido,
    Desconocido
};

enum class Accesibilidad {
    Accesible,
    NoAccesible,
    Parcial,
    Desconocido
};

class Arista {
public:
    Arista(int origenId, int destinoId, double distancia,
           TipoCamino tipo = TipoCamino::Desconocido,
           EstadoCamino estado = EstadoCamino::Desconocido,
           Accesibilidad accesibilidad = Accesibilidad::Desconocido);
    ~Arista();

    int obtenerOrigenId() const;
    int obtenerDestinoId() const;
    double obtenerDistancia() const;
    TipoCamino obtenerTipo() const;
    EstadoCamino obtenerEstado() const;
    Accesibilidad obtenerAccesibilidad() const;

    void establecerDistancia(double distancia);
    void establecerTipo(TipoCamino tipo);
    void establecerEstado(EstadoCamino estado);
    void establecerAccesibilidad(Accesibilidad accesibilidad);

private:
    int origenId_;
    int destinoId_;
    double distancia_;
    TipoCamino tipo_;
    EstadoCamino estado_;
    Accesibilidad accesibilidad_;
};

} // namespace gps

#endif // ARISTA_H
