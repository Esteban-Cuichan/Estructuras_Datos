#ifndef VECTOR_H
#define VECTOR_H

#include "IVector.h"
#include <cmath>

template <typename T>
class Vector : public IVector<T>{
	private:
		T componentes[3];
	public:
		Vector(T x=0, T y=0, T z=0){
			componentes[0]=x;
			componentes[1]=y;
			componentes[2]=z;
		}
		
		void setComponente(int posicion, T valor) override{
			if(posicion>=0 && posicion<3){
				componentes[posicion]=valor;
			}
		}
		
		T getComponente(int posicion) const override{
			return (posicion>=0 && posicion<3) ? componentes[posicion]:0;
		}
		
		T calcular_PP(Vector<T> otro) override{
			T suma=0;
			for (int i = 0; i < 3; i++) {
	            suma += componentes[i] * otro.getComponente(i);
	        }
	        return suma;
		}
		
		T calcular_mod() override{
			T sumaCuadrados = (componentes[0] * componentes[0]) + 
	                          (componentes[1] * componentes[1]) + 
	                          (componentes[2] * componentes[2]);
	        return std::sqrt(sumaCuadrados);
		}
};

#endif
