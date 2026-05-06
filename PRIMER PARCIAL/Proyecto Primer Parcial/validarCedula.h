#ifndef VALIDARCEDULA_H
#define VALIDARCEDULA_H
#include <string>
using namespace std;

class validarCedula{
    public:
        virtual bool validar(const char*)=0;
};
#endif