#include "Arista.h"

namespace gps {

Arista::Arista(int origenId, int destinoId, double distancia,
               TipoCamino tipo, EstadoCamino estado, Accesibilidad accesibilidad)
    : origenId_(origenId), destinoId_(destinoId), distancia_(distancia),
      tipo_(tipo), estado_(estado), accesibilidad_(accesibilidad) {
}

Arista::~Arista() {
}

int Arista::obtenerOrigenId() const {
    return origenId_;
}

int Arista::obtenerDestinoId() const {
    return destinoId_;
}

double Arista::obtenerDistancia() const {
    return distancia_;
}

TipoCamino Arista::obtenerTipo() const {
    return tipo_;
}

EstadoCamino Arista::obtenerEstado() const {
    return estado_;
}

Accesibilidad Arista::obtenerAccesibilidad() const {
    return accesibilidad_;
}

void Arista::establecerDistancia(double distancia) {
    distancia_ = distancia;
}

void Arista::establecerTipo(TipoCamino tipo) {
    tipo_ = tipo;
}

void Arista::establecerEstado(EstadoCamino estado) {
    estado_ = estado;
}

void Arista::establecerAccesibilidad(Accesibilidad accesibilidad) {
    accesibilidad_ = accesibilidad;
}

} // namespace gps
