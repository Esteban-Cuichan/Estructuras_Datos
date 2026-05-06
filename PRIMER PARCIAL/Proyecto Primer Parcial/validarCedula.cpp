#include "validarCedula.h"
#include <string>
using namespace std;

bool validarCedula::validar(const char* cedula){
    int longitud=0;
    while (*(cedula+longitud)!='\0'){
        longitud++;
    }
    
    if(longitud!=10) return false;

    for(int i=0;i<10;i++){
        char c=*(cedula+i);
        if(c<'0' || c>'9') return false;
    }

    int provincia=(*(cedula+0)-'0')*10+(*(cedula+1)-'0');
    int tercer=*(cedula+2)-'0';

    if (!((provincia >= 1 && provincia <= 24) || provincia == 30)) return false;
    if (tercer>=6) return false;

    int suma = 0;
    for(int i = 0; i < 9; i++){
        int digito = *(cedula + i) - '0';
        int coeficiente;
        if(i % 2 == 0){
            coeficiente = 2;
        }else{
            coeficiente = 1;
        }
        int producto = digito * coeficiente;
        if (producto > 9) producto -= 9;
        suma += producto;
    }

    int verificadorReal = *(cedula + 9)-'0';
    int residuo = suma % 10;
    int verificadorCalculado=(residuo == 0) ? 0 : 10 - residuo;
    return verificadorCalculado == verificadorReal;
}

/*
#include <iostream>

using namespace std;

// Función para validar la cédula usando solo aritmética de punteros
bool validarCedula(char* pCedula) {
    // 1. Calcular longitud manualmente
    int longitud = 0;
    while (*(pCedula + longitud) != '\0') {
        longitud++;
    }

    if (longitud != 10) return false;

    // 2. Validar que sean números
    for (int i = 0; i < 10; i++) {
        char c = *(pCedula + i);
        if (c < '0' || c > '9') return false;
    }

    // 3. Provincia y Tercer dígito
    int prov = (*(pCedula + 0) - '0') * 10 + (*(pCedula + 1) - '0');
    int tercer = *(pCedula + 2) - '0';

    if (!((prov >= 1 && prov <= 24) || prov == 30)) return false;
    if (tercer >= 6) return false;

    // 4. Algoritmo Modulo 10
    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int digito = *(pCedula + i) - '0';
        int coeficiente;

        // Determinamos el coeficiente sin usar un arreglo
        // Posiciones impares (0, 2, 4, 6, 8) por 2, pares (1, 3, 5, 7) por 1
        if (i % 2 == 0) coeficiente = 2;
        else coeficiente = 1;

        int producto = digito * coeficiente;
        if (producto > 9) producto -= 9;
        
        suma += producto;
    }

    int verificadorReal = *(pCedula + 9) - '0';
    int residuo = suma % 10;
    int verificadorCalculado = (residuo == 0) ? 0 : 10 - residuo;

    return verificadorCalculado == verificadorReal;
}

int main() {
    // Reservamos memoria para 11 caracteres (espacio para el \0)
    char* pDato = new char; 
    // Nota: 'new char' solo reserva 1 byte. Para una cadena usamos:
    char* cedula = (char*)operator new(11); 

    char continuar;

    do {
        cout << "\n--- VALIDACION (PUNTEROS Y MEMORIA DINAMICA) ---" << endl;
        cout << "Ingrese la cedula: ";
        cin >> cedula;

        if (validarCedula(cedula)) {
            cout << ">> RESULTADO: Valida" << endl;
        } else {
            cout << ">> RESULTADO: Invalida" << endl;
        }

        cout << "Desea probar otra? (s/n): ";
        cin >> continuar;

    } while (continuar == 's' || continuar == 'S');

    // Liberación manual
    operator delete(cedula);
    
    return 0;
}
*/