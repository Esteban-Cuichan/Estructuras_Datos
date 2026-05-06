#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
int ingresoNumero(char *);
int ingresoNumero(char *msj){
    cout<<msj<<endl;
    char dat[11];
    char c;
    int i=0;
    while((c=getch())!=13){
        if(c>='0' && c<='9'){
            cout<<c;
            dat[i++]=c;
        }
    }
    return 0;
}

int main(){
    ingresoNumero("Ingrese un numero [ENTERO]: ");
    return 0;
}