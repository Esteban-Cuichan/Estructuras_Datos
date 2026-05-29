#ifndef VALIDAR_EXPRESION_H
#define VALIDAR_EXPRESION_H
#include <iostream>
#include <string>
using namespace std;

class validarExpresion{
    public:
        static bool validarNombre(const string&);
        static bool validarTelefono(const string&);
        static bool validarCorreo(const string&);
        static bool validarCedulanum(const string&);
        static bool validarPlaca(const string&);
};
#endif
