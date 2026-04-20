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
		T getNumerador(){
			return numerador;
		}
		void setNumerador(T num){
			numerador=num;
		}
		T getDenominador(){
			return denominador;
		}
		void setDenominador(T den){
			denominador=den;
		}
		Quebrado<T> sumar(Quebrado<T> otro){
			T n=(this->numerador*otro.getNumerador())+(this->denominador*otro.getDenominador());
			T d=(this->denominador*otro.getDenominador());
			return Quebrado(n,d);
		}
};

#endif
