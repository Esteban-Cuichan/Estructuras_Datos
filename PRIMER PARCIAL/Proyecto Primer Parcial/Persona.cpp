#include "Persona.h"

Persona::Persona(string nombre, string apellido, string CI, string telefono, string correo){
    this->nombre=nombre;
    this->apellido=apellido;
    this->CI=CI;
    this->telefono=telefono;
    this->correo=correo;
}

Persona::Persona(){
    this->nombre="";
    this->apellido="";
    this->CI="";
    this->telefono="";
    this->correo="";
}

string Persona::getNombre(){
    return nombre;
}

string Persona::getApellido(){
    return apellido;
}

string Persona::getCI(){
    return CI;
}

string Persona::getTelefono(){
    return telefono;
}

string Persona::getCorreo(){
    return correo;
}

string Persona::getTelefono(){
    return telefono;
}

void Persona::setApellido(string apellido){
    this->apellido=apellido;
}

void Persona::setNombre(string nombre){
    this->nombre=nombre;
}

void Persona::setCI(string CI){
    this->CI=CI;
}

void Persona::setCorreo(string correo){
    this->correo=correo;
}

void Persona::setTelefono(string telefono){
    this->telefono=telefono;
}