/*********************************************
	UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE
	Nombre: CUICHAN MALES LENIN ESTEBAN
	NRC: 28971
	Fecha: 09/04/2026
**********************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include "Quebrado.h"
using namespace std;

int main(){
	/*float num1,num2,den1,den2;
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
	Quebrado<float> quebrado1(num1,den1);
	Quebrado<float> quebrado2(num2,den2);
	cout<<"Quebrado 1: "<<num1<<"/"<<den1<<endl;
	cout<<"Quebrado 2: "<<num2<<"/"<<den2<<endl;
	Quebrado<float> quebrado3=quebrado1.sumar(quebrado2);
	cout<<"Quebrado 3: "<<quebrado3.getNumerador()<<"/"<<quebrado3.getDenominador()<<endl;
	Quebrado<double> quebrado4(3.5,4.5);
	cout<<"Quebrado 4: "<<quebrado4.getNumerador()<<"/"<<quebrado4.getDenominador()<<endl;
	Quebrado<Quebrado<float>> quebrado5=quebrado5.procesar(quebrado4);
	cout<<"Quebrado 5: "<<quebrado5.getNumerador()<<"/"<<quebrado5.getDenominador()<<endl;
	*/
	int a, *p;
	a=7;
	p=&a;
	printf("valor de a: %d",a);
	printf("\ndireccion de a: %p",&a);
	printf("\nvalor de a por medio de p: %d",*p);
	printf("\ndireccion de a por medio de p: %p",p);
	printf("\nDireccion de p: %p",&p);
	return 0;
}


