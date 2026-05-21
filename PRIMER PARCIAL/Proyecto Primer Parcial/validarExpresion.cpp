#include <iostream>
#include <string>
#include <regex>
#include "validarExpresion.h"
using namespace std;

bool validarExpresion::validarNombre(const string& nombre){
    regex comando("^[a-zA-ZáéíóúÁÉÍÓÚñÑ\\s]+$");
    return regex_match(nombre,comando);
}

bool validarExpresion::validarTelefono(const string& telefono){
    regex comando("^\\d{10}$");
    return regex_match(telefono,comando);
}

bool validarExpresion::validarCorreo(const string& correo){
    regex comando("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex_match(correo,comando);
}

bool validarExpresion::validarCedulanum(const string& cedula){
    regex comando("^\\d{10}$");
    return regex_match(cedula,comando);
}

bool validarExpresion::validarPlaca(const string& placa){
    regex comando("^[A-Z]{3}\\d{3,4}$");
    return regex_match(placa,comando);
}