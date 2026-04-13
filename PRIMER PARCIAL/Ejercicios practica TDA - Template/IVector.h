#ifndef IVECTOR_H
#define IVECTOR_H

template <typename T>
class Vector;

template <typename T>
class IVector{
public:
	virtual ~IVector() {}
	virtual T calcular_PP(Vector<T> otro)=0;
	virtual T calcular_mod()=0;
	virtual void setComponente(int posicion, T valor)=0;
	virtual T getComponente(int posicion) const=0;
};

#endif
