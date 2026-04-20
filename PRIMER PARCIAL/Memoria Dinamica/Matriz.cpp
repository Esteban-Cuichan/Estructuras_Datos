#include <iostream>
#include "Matriz.h"

using namespace std;

int main() {
    int fila,columna;
    float elemento;
    cout<<"-------------- SUMA DE MATRICES --------------"<<endl;
    cout<<"Ingresa la fila> ";
    cin>>fila;
    cout<<"Ingresa la columna> ";
    cin>>columna;
    Matriz<float> matriz_1(fila,columna);
    Matriz<float> matriz_2(fila,columna);
    
    cout<<"Ingreso de la primera Matriz> "<<endl;
    for(int i=0;i<fila;i++){
    	for(int j=0;j<columna;j++){
    		cout<<"Ingreso del elemento ["<<i<<"]["<<j<<"]> ";
    		cin>>elemento;
    		matriz_1.setElemento(i,j,elemento);
		}
	}
	
	cout<<"Ingreso de la segunda Matriz> "<<endl;
    for(int i=0;i<fila;i++){
    	for(int j=0;j<columna;j++){
    		cout<<"Ingreso del elemento ["<<i<<"]["<<j<<"]> ";
    		cin>>elemento;
    		matriz_2.setElemento(i,j,elemento);
		}
	}
	
    Matriz<float> matrizRespuesta = matriz_1 + matriz_2; 

    cout<<"Resultado de A + B = "<<endl;
    for(int i=0;i<fila;i++){
        for(int j=0;j<columna;j++){
            cout<<matrizRespuesta.getElemento(i, j)<<" ";
        }
        cout<<endl;
    }
    return 0;
}
