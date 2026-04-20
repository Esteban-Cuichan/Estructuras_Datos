/*********************************************
	UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE
	Nombre: CUICHAN MALES LENIN ESTEBAN
	NRC: 28971
	Fecha: 09/04/2026
**********************************************/
#ifndef QUEBRADO_H
#define QUEBRADO_H

#include "IQuebrado.h"

template <typename T>
class Quebrado : public IQuebrado<T>{
	private:
		T numerador;
		T denominador;
	public:
		Quebrado(T num, T den){
			this->numerador=num;
			this->denominador=den;
		}
		T getNumerador() override {
			return numerador;
		}
		void setNumerador(T num) override{
			numerador=num;
		}
		void setDenominador(T den) override{
			denominador=den;
		}
		T getDenominador() override{
			return denominador;
		}
		Quebrado<T> sumar(Quebrado<T> otro) override{
			T n=(this->numerador*otro.getDenominador())+(this->denominador*otro.getNumerador());
			T d=(this->denominador*otro.getDenominador());
			return Quebrado(n,d);
		}
		
		Quebrado<T> procesar(T q){
			return q;
		}
};

#endif


