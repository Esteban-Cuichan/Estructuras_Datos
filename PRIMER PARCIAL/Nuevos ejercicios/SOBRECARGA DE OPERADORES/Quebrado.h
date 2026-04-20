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
		T getNumerador()const{
			return numerador;
		}
		void setNumerador(T num){
			numerador=num;
		}
		T getDenominador()const{
			return denominador;
		}
		void setDenominador(T den){
			denominador=den;
		}
		
		T dec(){
			return (numerador/denominador);
		}
		
		Quebrado<T> sumar(Quebrado<T> otro) override{
			T n=(this->numerador*otro.getDenominador())+(this->denominador*otro.getNumerador());
			T d=(this->denominador*otro.getDenominador());
			return Quebrado(n,d);
		}
		
		bool operator>(const Quebrado<T>& otro)const {
	        return this.dec()>otro.dec();
	    }
};

#endif
