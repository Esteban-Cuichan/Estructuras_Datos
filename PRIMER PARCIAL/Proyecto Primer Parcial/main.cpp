#include "validarCedula.h"
#include <iostream>
using namespace std;

int main(void){
    char* pDato = new char;
    char* cedula = (char*)operator new(11); 
    char continuar;
    do {
        cout << "\n--- VALIDACION CEDULA---" << endl;
        cout << "Ingrese la cedula: ";
        cin >> cedula;
        if(validarCedula::validar(cedula)) {
            cout << ">> RESULTADO: Valida" << endl;
        } else {
            cout << ">> RESULTADO: Invalida" << endl;
        }
        cout << "Desea probar otra? (s/n): ";
        cin >> continuar;
    } while (continuar == 's' || continuar == 'S');
    
    return 0;
}