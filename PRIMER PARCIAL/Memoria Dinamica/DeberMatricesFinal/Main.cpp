#include <iostream>
#include "Matriz.h"

using namespace std;

int main(){
    int fila, columna,i,j;
    float elemento;
    cout<<"-------- SUMA DE MATRICES --------"<<endl;
    cout<<"Ingrese el numero de filas de su matriz> ";
    cin>>fila;
    cout<<"Ingrese el numero de columnas de su matriz> ";
    cin>>columna;
    if(fila!=columna){
        cout<<"ERROR: Para la suma de matrices el numero de filas y columnas debe ser igual!"<<endl;
        return 1;
    }else{
        Matriz<float> matriz_1(fila, columna);
        Matriz<float> matriz_2(fila, columna);
        cout<<"Ingreso de la primera matriz: "<<endl;
        for(i=0;i<fila;i++){
            for(j=0;j<columna;j++){
                cout<<"Ingrese el elemento en la posicion ["<<i<<"]["<<j<<"]> "<<endl;
                cin>>elemento;
                matriz_1.setElemento(i,j,elemento);
            }
        }
        cout<<"Ingreso de la segunda matriz: "<<endl;
        for(i=0;i<fila;i++){
            for(j=0;j<columna;j++){
                cout<<"Ingrese el elemento en la posicion ["<<i<<"]["<<j<<"]> "<<endl;
                cin>>elemento;
                matriz_2.setElemento(i,j,elemento);
            }
        }

        Matriz<float> suma=matriz_1+matriz_2;
        cout<<"Resultado de A + B = "<<endl;
        for(i=0;i<fila;i++){
            for(j=0;j<columna;j++){
                cout<<suma.getElemento(i, j)<<" ";
            }
            cout<<endl;
        }
    }

    return 0;
}