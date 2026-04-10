/*********************************************
	UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE
	Nombre: CUICHAN MALES LENIN ESTEBAN
	NRC: 28971
	Fecha: 09/04/2026
**********************************************/
#ifndef QUEBRADO_H
#define QUEBRADO_H

class Quebrado{
	private:
		float numerador;
		float denominador;
	public:
		Quebrado(float num, float den);
		float getNumerador();
		void setNumerador(float num);
		void setDenominador(float den);
		float getDenominador();
		Quebrado sumar(Quebrado otro);
};

#endif
