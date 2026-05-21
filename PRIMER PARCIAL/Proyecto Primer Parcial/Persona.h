#ifndef PERSONA_H
#define PERSONA_H
#include <iostream>
#include <string>
using namespace std;

class Persona{
    private:
        string nombre;
        string apellido;
        string CI;
        string correo;
        string telefono;
    public:
        Persona();
        Persona(string,string,string,string,string);
        ~Persona(){}
        string getNombre();
        string getApellido();
        string getCI();
        string getCorreo();
        string getTelefono();
        void setNombre(string);
        void setApellido(string);
        void setCI(string);
        void setCorreo(string);
        void setTelefono(string);
};

#endif