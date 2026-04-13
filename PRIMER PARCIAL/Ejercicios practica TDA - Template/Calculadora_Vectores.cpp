#include <iostream>
#include "Vector.h"
using namespace std;

int main(){
	Vector<float> v1;
	Vector<float> v2;
	cout<<"Ingrese los componentes del primer vector:"<<endl;
	float temp1,temp2;
	for(int i=0;i<3;i++){
		cout<<"Componente ["<<i<<"]: ";
		cin>>temp1;
		v1.setComponente(i,temp1);
	}
	
	cout<<"Ingrese los componentes del segundo vector:"<<endl;
	for(int i=0;i<3;i++){
		cout<<"Componente ["<<i<<"]: ";
		cin>>temp2;
		v2.setComponente(i,temp2);
	}
	
	cout<<"Vector 1: <"<<v1.getComponente(0)<<","<<v1.getComponente(1)<<","<<v1.getComponente(2)<<">"<<endl;
	cout<<"Vector 2: <"<<v2.getComponente(0)<<","<<v2.getComponente(1)<<","<<v2.getComponente(2)<<">"<<endl;
	
	float productoPunto=v1.calcular_PP(v2);
	float moduloV1=v1.calcular_mod();
	cout<<"---- RESULTADOS ----"<<endl;
	cout<<"Producto punto: "<<productoPunto<<endl;
	cout<<"Modulo de V1: "<<moduloV1<<endl;
	if(productoPunto==0){
		cout<<"Los vectores son ORTOGONALES"<<endl;
	}else{
		cout<<"Los vectores NO son ortogonales"<<endl;
	}
	return 0;
}
