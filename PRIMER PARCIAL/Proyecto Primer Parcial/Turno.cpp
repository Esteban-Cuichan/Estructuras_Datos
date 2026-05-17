#include "Turno.h"

Turno::Turno(Persona u,Vehiculo v,Fecha f,int id,bool est) {
    usuario = u;
    vehiculo = v;
    fecha = f;
    id_turno = id;
    estado = est;
    siguiente = nullptr;
    anterior = nullptr;
}

Persona Turno::getUsuario() { return usuario; }
Vehiculo Turno::getVehiculo() { return vehiculo; }
Fecha Turno::getFecha() { return fecha; }
int Turno::getIDturno() { return id_turno; }
bool Turno::getEstado() { return estado; }
Turno* Turno::getSiguiente() { return siguiente; }
Turno* Turno::getAnterior() { return anterior; }
void Turno::setUsuario(Persona u) { usuario = u; }
void Turno::setVehiculo(Vehiculo v) { vehiculo = v; }
void Turno::setFecha(Fecha f) { fecha = f; }
void Turno::setIDturnos(int id) { id_turno = id; }
void Turno::setEstado(bool est) { estado = est; }
void Turno::setSiguiente(Turno* s) { siguiente = s; }
void Turno::setAnterior(Turno* a) { anterior = a; }