#ifndef VALIDARCEDULA_H
#define VALIDARCEDULA_H
#ifdef BUILDING_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif
using namespace std;

class DLL_EXPORT validarCedula{
    public:
        static bool validar(const char*);
};
#endif