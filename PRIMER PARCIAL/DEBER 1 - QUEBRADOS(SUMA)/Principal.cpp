/*********************************************
	UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE
	Nombre: CUICHAN MALES LENIN ESTEBAN
	NRC: 28971
	Fecha: 09/04/2026
**********************************************/
#include <iostream>
#include "Quebrado.h"
using namespace std;

int main(){
	float num1,num2,den1,den2;
	cout<<"INGRESO DEL PRIMER QUEBRADO"<<endl;
	cout<<"Ingrese el numerador del primer quebrado> ";
	cin>>num1;
	cout<<"Ingrese el denominador del primer quebrado> ";
	cin>>den1;
	cout<<"INGRESO DEL SEGUNDO QUEBRADO"<<endl;
	cout<<"Ingrese el numerador del segundo quebrado> ";
	cin>>num2;
	cout<<"Ingrese el denominador del segundo quebrado> ";
	cin>>den2;
	Quebrado quebrado1(num1,den1);
	Quebrado quebrado2(num2,den2);
	cout<<"Quebrado 1: "<<num1<<"/"<<den1<<endl;
	cout<<"Quebrado 2: "<<num2<<"/"<<den2<<endl;
	Quebrado quebrado3=quebrado1.sumar(quebrado2);
	cout<<"Quebrado 3: "<<quebrado3.getNumerador()<<"/"<<quebrado3.getDenominador()<<endl;
	return 0;
}


