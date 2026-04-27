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
        int profundidad;
        T*** elemento;
    public:
        Matriz(int f, int c, int p){
            this->fila=f;
            this->columna=c;
            this->profundidad=p;
            elemento=(T***)malloc(fila*sizeof(T**));
            for(int i=0;i<fila;i++){
                *(elemento+i)=(T**)malloc(columna*sizeof(T*));
                for(int j=0;j<columna;j++){
                    *(*(elemento+i)+j)=(T*)malloc(profundidad*sizeof(T));
                    for(int k=0;k<profundidad;k++){
                        *(*(*(elemento+i)+j)+k)=0;
                    } 
                }
            }
        }

        void setElemento(int fila,int columna,int profundidad,T valor){
            *(*(*(elemento+fila)+columna)+profundidad)=valor;
        }

        T getElemento(int fila,int columna,int profundidad)const{
            return *(*(*(elemento+fila)+columna)+profundidad);
        }

        void setMatriz(T*** matriz){
            this->elemento=matriz;
        }

        T*** getMatriz()const{
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

        int getProfundidad()const{
            return profundidad;
        }

        void setProfundidad(int p){
            this->profundidad=p;
        }

        /*Matriz<T> operator+(const Matriz<T>& otra)const{
            int i,j;
            Matriz<T> resultado(fila,columna);
            for(i=0;i<fila;i++){
                for(j=0;j<columna;j++){
                    T item=this->getElemento(i,j)+otra.getElemento(i,j);
                    resultado.setElemento(i,j,item);
                }
            }
            return resultado;
        }*/

        Matriz<T> operator+(const Matriz<T>& otra)const{
            int i,j,k;
            Matriz<T> suma(fila,columna,profundidad);
            for(i=0;i<fila;i++){
                for(j=0;j<columna;j++){
                    for(k=0;k<profundidad;k++){
                        T valor=this->getElemento(i,j,k)+otra.getElemento(i,j,k);
                        suma.setElemento(i,j,k,valor);
                    }
                }
            }
            return suma;
        }
        
        /*Matriz<T> operator*(const Matriz<T>& otra)const{
            int i,j,k;
            Matriz<T> resultado(this->fila,otra.columna,this->profundidad);
            for(k=0;k<profundidad;k++){
                for(i=0;i<fila;i++){
                    for(j=0;j<otra.columna;j++){
                        T suma=0;
                        for(int l=0;l<columna;l++){
                            suma+=this->getElemento(i,l,k)*otra.getElemento(l,j,k);
                        }
                        resultado.setElemento(i,j,k,suma);
                    }
                }
            }
            return resultado;
        }*/

        //MULTIPLICACIÓN POR RECURSIVIDAD
        Matriz<T> operator*(const Matriz<T>& otra)const{
            int i,j,k;
            Matriz<T> resultado(fila,columna,profundidad);
            for(k=0;k<profundidad;k++){
                for(i=0;i<fila;i++){
                    for(j=0;j<otra.getColumna();j++){
                        T valor=calcularRecursivo(*this,otra,i,j,k,0);
                        resultado.setElemento(i,j,k,valor);
                    }
                }
            }
            return resultado;
        }

        T calcularRecursivo(const Matriz<T>& A, const Matriz<T>& B, int i, int j, int k, int l)const{
            if(l>=A.getColumna()) return 0;
            return (A.getElemento(i,l,k)*B.getElemento(l,j,k)) + calcularRecursivo(A,B,i,j,k,l+1);
        }

        //revisar esta suma
        /*T sumaDiagonal(const Matriz<T>& mat,int f,int c,int p){
            int i,j,k;
            T suma;
            for(k=0;k<p;k++){
                for(i=0;i<fila;i++){
                    for(j=0;j<columna;j++){
                        if(i=j){
                            suma+=mat.getElemento(i,j,k);
                        }
                    }
                }
            }
            return suma;
        }*/

};
#endif