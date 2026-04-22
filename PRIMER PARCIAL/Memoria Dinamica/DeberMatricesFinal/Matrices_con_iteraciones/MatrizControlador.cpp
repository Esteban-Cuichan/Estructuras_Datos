#include "MatrizControlador.h"
#include <iostream>
using namespace std;

void MatrizControlador::sumar(){
    int fil,col,pro;
    cout<<"Ingrese FILAS, COLUMNAS y PROFUNDIDAD: ";
    cin>>fil>>col>>pro;
    Matriz<float> matrizA(fil,col,pro);
    Matriz<float> matrizB(fil,col,pro);
    cout<<"Ingrese la primera matriz> "<<endl;
    ingreso(matrizA);
    cout<<"MATRIZ A: "<<endl;
    imprimir(matrizA);
    cout<<"Ingrese la segunda matriz> "<<endl;
    ingreso(matrizB);
    cout<<"MATRIZ B: "<<endl;
    imprimir(matrizB);
    Matriz<float> suma=matrizA+matrizB;
    cout<<"La matriz resultante es> "<<endl;
    imprimir(suma);
    cout<<"Encerando matrices"<<endl;
    encerar(matrizA);
    encerar(matrizB);
    encerar(suma);
}

void MatrizControlador::ingreso(Matriz<float>& m){
    int i,j,k;
    float item;
    for(i=0;i<m.getFila();i++){
            for(j=0;j<m.getColumna();j++){
                for(k=0;k<m.getProfundidad();k++){
                    cout<<"Ingrese el elemento en la posicion ["<<i<<"]["<<j<<"]["<<k<<"]> "<<endl;
                    cin>>item;
                    m.setElemento(i,j,k,item);
                }
            }
        }
}

/*void MatrizControlador::imprimir(const Matriz<float>& m){
    int i,j,k;
    for(i=0;i<m.getFila();i++){
            for(j=0;j<m.getColumna();j++){
                for(k=0;k<m.getProfundidad();k++){
                    cout<<m.getElemento(i,j,k)<<" ";
                }
            }
            cout<<endl;
    }
}*/

void MatrizControlador::encerar(Matriz<float>& m){
    int i,j,k;
    for(i=0;i<m.getFila();i++){
            for(j=0;j<m.getColumna();j++){
                for(k=0;k<m.getProfundidad();k++){
                    m.setElemento(i,j,k,0);
                }
            }
    }
}

void MatrizControlador::multiplicar(){
    int fil,col,pro;
    cout<<"Ingrese FILAS, COLUMNAS y PROFUNDIDAD: ";
    cin>>fil>>col>>pro;
    Matriz<float> matrizA(fil,col,pro);
    Matriz<float> matrizB(fil,col,pro);
    cout<<"Ingrese la primera matriz> "<<endl;
    ingreso(matrizA);
    cout<<"MATRIZ A: "<<endl;
    imprimir(matrizA);
    cout<<"Ingrese la segunda matriz> "<<endl;
    ingreso(matrizB);
    cout<<"MATRIZ B: "<<endl;
    imprimir(matrizB);
    Matriz<float> producto=matrizA*matrizB;
    cout<<"La matriz resultante es> "<<endl;
    imprimir(producto);
    cout<<"Encerando matrices"<<endl;
    encerar(matrizA);
    encerar(matrizB);
    encerar(producto);
}

void MatrizControlador::imprimir(const Matriz<float>& mat){
    for (int p = 0;p<mat.getProfundidad();p++){
        cout<<"Capa "<<p<<":"<<endl;
        for (int i=0;i<mat.getFila();i++) {
            for (int j=0;j<mat.getColumna();j++) {
                cout<<mat.getElemento(i,j,p)<<"\t";
            }
            cout<<endl;
        }
        cout<<"-------------------"<<endl;
    }
}