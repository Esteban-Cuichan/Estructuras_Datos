#include "validarCedula.h"
#include "C:\Users\Esteban\Documents\Documentos\PREGRADO SI-ABR26-AGO26\ESTRUCTURAS DE DATOS\PRIMER PARCIAL\Librerias\validarExpresion\validarExpresion.h"
#include <string>
using namespace std;

bool validarCedula::validar(const char* cedula){
    if(cedula==nullptr) return false;
    int longitud=0;
    while (*(cedula+longitud)!='\0'){
        longitud++;
    }
    
    if(!validarExpresion::validarCedulanum(cedula)){return false;}

    int provincia=(*(cedula+0)-'0')*10+(*(cedula+1)-'0');
    int tercer=*(cedula+2)-'0';

    if (!((provincia >= 1 && provincia <= 24) || provincia == 30)) return false;
    if (tercer>=6) return false;

    int suma=0;
    //Aplicado el bit a bit
    for(int i=0;i<9;i++){
        int digito=*(cedula+i)-'0';
        int coeficiente=(i & 1) ? 1:2;
        int producto=(coeficiente==2) ? (digito<<1):digito;
        if (producto>9) producto -= 9;
        suma += producto;
    }
    //Hasta aqui

    int verificadorReal=*(cedula + 9)-'0';
    int residuo=suma % 10;
    int verificadorCalculado=(residuo == 0) ? 0 : 10 - residuo;
    return verificadorCalculado == verificadorReal;
}
