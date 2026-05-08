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

    int suma=0;
    for(int i=0;i<9;i++){
        int digito=*(cedula+i)-'0';
        int coeficiente;
        if(i % 2 == 0){
            coeficiente = 2;
        }else{
            coeficiente = 1;
        }
        int producto=digito*coeficiente;
        if (producto>9) producto -= 9;
        suma += producto;
    }

    int verificadorReal=*(cedula + 9)-'0';
    int residuo=suma % 10;
    int verificadorCalculado=(residuo == 0) ? 0 : 10 - residuo;
    return verificadorCalculado == verificadorReal;
}
