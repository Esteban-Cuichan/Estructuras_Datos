/*********************************************
	UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE
	Nombre: CUICHAN MALES LENIN ESTEBAN
	NRC: 28971
	Fecha: 09/04/2026
**********************************************/
#include "Quebrado.h"
#include <iostream>

Quebrado::Quebrado(float num, float den){
	numerador=num;
	denominador=den;
}

float Quebrado::getNumerador(){
	return numerador;
}

float Quebrado::getDenominador(){
	return denominador;
}

void Quebrado::setNumerador(float num){
	numerador=num;
}

void Quebrado::setDenominador(float den){
	denominador=den;
}

Quebrado Quebrado::sumar(Quebrado otro){
	float n=(this->numerador*otro.getDenominador())+(this->denominador*otro.getNumerador());
	float d=(this->denominador*otro.getDenominador());
	return Quebrado(n,d);
}



