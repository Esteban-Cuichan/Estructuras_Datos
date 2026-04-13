#ifndef CONTENEDOR_H
#define CONTENEDOR_H
#include "IContenedor.h"
#include <iostream>
using namespace std;

template <typename T>
class Contenedor : public IContenedor<T>{
	private:
		T valor;
		bool ocupado;
	public:
		Contenedor() : ocupado(false){}
		void guardar(T dato){
			this->valor = dato;
			this->ocupado=true;
			cout<<"Dato guardado correctamente"<<endl;
		}
		
		T recuperar(){
			if(!ocupado){
				cout<<"Aviso: El contenedor se encuenta vacio!"<<endl;
			}
			return valor;
		}
		
		void limpiar(){
			ocupado = false;
		}
		
		bool estVacio(){
			return !ocupado;
		}
};

#endif
