#ifndef PERSONA_H
#define PERSONA_H
#include <iostream>
using namespace std;

class Persona{
    private:
        string nombre;
        string apellido;
        //Fecha fechaNacimiento;
        string CI;
        string correo;
        string telefono;
        //Vehiculo vehiculo
    public:
        Persona();
        Persona(string,string,string,string,string);
        Persona(){}
        string getNombre();
        string getApellido();
        //Fecha getFecha();
        string getCI();
        string getCorreo();
        string getTelefono();
        //Vehiculo getVehiculo();
        void setNombre(string);
        void setApellido(string);
        //void setFecha(Fecha);
        void setCI(string);
        void setCorreo(string);
        void setTelefono(string);
        //void setVehiculo(Vehiculo);
};

#endif