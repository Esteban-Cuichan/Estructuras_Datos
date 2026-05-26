#pragma once
#include "Persona.h"
#include "Fecha.h"
#include "Vehiculo.h"

class Turno {
private:
    Persona usuario;
    Vehiculo vehiculo;
    Fecha fecha;
    int id_turno;
    bool estado;
    Turno* siguiente;
    Turno* anterior;

public:
    Turno();
    Turno(Persona,Vehiculo,Fecha,int,bool);
    Persona getUsuario();
    Vehiculo getVehiculo();
    Fecha getFecha();
    int getIDturno();
    bool getEstado();
    Turno* getSiguiente();
    Turno* getAnterior();
    void setUsuario(Persona);
    void setVehiculo(Vehiculo);
    void setFecha(Fecha);
    void setIDturnos(int);
    void setEstado(bool);
    void setSiguiente(Turno*);
    void setAnterior(Turno*);
};