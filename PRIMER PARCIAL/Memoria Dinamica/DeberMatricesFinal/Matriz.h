#ifndef MATRIZ_H
#define MATRIZ_H
#include "IMatriz.h"
#include <iostream>
#include <cstdlib>
using namespace std;

template <typename T>
class Matriz : public IMatriz<T>{
    private:
        int fila;
        int columna;
        T** elemento;
    public:
        Matriz(int f, int c){
            this->fila=f;
            this->columna=c;
            elemento=(T**)malloc(fila*sizeof(T));
            for(int i=0;i<fila;i++){
                *(elemento+i)=(T*)malloc(columna*sizeof(T*));
                for(int j=0;j<columna;j++){
                    *(*(elemento+i)+j)=0;
                }
            }
        }

        void encerar(T** elemento,int fila,int columna){
            int i,j;
            for(i=0;i<fila;i++){
                for(j=0;j<columna;j++){
                    elemento[i][j]=0;
                }
            }
        }

        void setElemento(int fila,int columna,T valor){
            *(*(elemento+fila)+columna)=valor;
        }

        T getElemento(int fila,int columna)const{
            return *(*(elemento+fila)+columna);
        }

        void setMatriz(T** matriz){
            this->elemento=matriz;
        }

        T** getMatriz()const{
            return this->elemento;
        }

        int getFila()const{
            return fila;
        }

        void setFila(int f){
            this->fila=f;
        }

        int getColumna()const{
            return columna;
        }

        void setColumna(int c){
            this->columna=c;
        }

        Matriz<T> operator+(const Matriz<T>& otra)const{
            int i,j;
            Matriz<T> resultado(fila,columna);
            for(i=0;i<fila;i++){
                for(j=0;j<columna;j++){
                    T item=this->getElemento(i,j)+otra.getElemento(i,j);
                    resultado.setElemento(i,j,item);
                }
            }
            return resultado;
        }
        
};
#endif