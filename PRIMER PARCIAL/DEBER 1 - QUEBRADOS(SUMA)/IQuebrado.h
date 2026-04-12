#ifndef IQUEBRADO_H
#define IQUEBRADO_H

template <typename T>
class Quebrado;

template <typename T>
class IQuebrado{
	public:
		virtual ~IQuebrado(){}
		virtual T getNumerador()=0;
		virtual void setNumerador(T num)=0;
		virtual T getDenominador()=0;
		virtual void setDenominador(T den)=0;
		virtual Quebrado<T> sumar(Quebrado<T> otro)=0;
};

#endif
