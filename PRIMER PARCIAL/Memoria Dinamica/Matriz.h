#ifndef MATRIZ_H
#define MATRIZ_H
#include "IMatriz.h"
#include<iostream>
#include <cstdlib>
using namespace std;

template <typename T>
class Matriz : public IMatriz<T>{
	private:
		int fila;
		int columna;
		T** elemento;
	public:
		Matriz(int fil, int col){
			this->fila=fil;
			this->columna=col;
			elemento=(T**)malloc(fila*sizeof(T*));
			if(elemento==nullptr){
				cout<<"Error: No se ha podido asignar memoria!"<<endl;
			}
		
			for(int i=0;i<fila;i++){
				*(elemento+i)=(T*)malloc(columna*sizeof(T));
				if (*(elemento + i) == nullptr)if(elemento==nullptr){
					cout<<"Error: No se ha podido asignar memoria!"<<endl;
				}
				for(int j=0; j<columna; j++){
					*(*(elemento+i)+j)=0;
				}
			}
		}
		
		~Matriz(){
			if(elemento!=nullptr){
				for(int i=0;i<fila;i++){
					free(*(elemento+i));
				}
				free(elemento);
				elemento=nullptr;
			}
		}

		void setElemento(int fil, int col, T valor){
			*(*(elemento+fil)+col)=valor;
		}
		
		T getElemento(int fil, int col)const{
			return *(*(elemento+fil)+col);
		}
		
		void setMatriz(T** mtr){
			this->elemento=mtr;
		}
		
		T** getMatriz()const{
			return this->elemento;
		}
		
		int getFila()const{
			return fila;
		}
		
		int getColumna()const{
			return columna;
		}
		
		Matriz<T> operator+(const Matriz<T>& otra)const{
			if(this->fila!=otra.getFila() || this->columna!=otra.getColumna()){
				cout<<"Dimensiones incompatibles."<<endl;
			}
			Matriz<T> resultado(fila,columna);
			for(int i=0;i<fila;i++){
				for(int j=0;j<columna;j++){
					T valor=this->getElemento(i,j)+otra.getElemento(i,j);
					resultado.setElemento(i,j,valor);
				}
			}
			return resultado;
		}
};

#endif
