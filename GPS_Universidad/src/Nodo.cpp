#include "Nodo.h"

namespace gps {

Nodo::Nodo(int id, const std::string &nombre, double latitud, double longitud, TipoNodo tipo)
    : id_(id), nombre_(nombre), latitud_(latitud), longitud_(longitud), tipo_(tipo) {
}

Nodo::~Nodo() {
}

int Nodo::obtenerId() const {
    return id_;
}

const std::string &Nodo::obtenerNombre() const {
    return nombre_;
}

double Nodo::obtenerLatitud() const {
    return latitud_;
}

double Nodo::obtenerLongitud() const {
    return longitud_;
}

TipoNodo Nodo::obtenerTipo() const {
    return tipo_;
}

void Nodo::establecerNombre(const std::string &nombre) {
    nombre_ = nombre;
}

void Nodo::establecerTipo(TipoNodo tipo) {
    tipo_ = tipo;
}

} // namespace gps
