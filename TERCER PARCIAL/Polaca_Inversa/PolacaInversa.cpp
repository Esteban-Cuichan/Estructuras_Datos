#include <iostream>
using namespace std;

int longitud(const char* expresion){
    int contador=0;
    while(*(expresion+contador)!='\0'){
        contador++;
    }
    return contador;
}

char* sacarOperador(const char* expresion, int &totalOperadores){
    totalOperadores=0;
    for(int i=0; i<longitud(expresion); i++){
        char actual=*(expresion+i);
        if(actual=='+' || actual=='-' || actual=='*' || actual=='/' || actual=='^'){
            totalOperadores++;

        }
    }
    char* operadores=new char[totalOperadores+1];
    int indice=0;
    for(int i=0;i<longitud(expresion);i++){
        char actual=*(expresion+i);
        if(actual=='+' || actual=='-' || actual=='*' || actual=='/' || actual=='^'){
            *(operadores+indice)=actual;
            indice++;
        }
    }
    *(operadores+indice)='\0';
    return operadores;
}

char* expresionPostFija(const char* expresion,const char* operadores){
    
}

int main(){
    const char* expresion="A*B+C-D/E^F";
    int cantidadOperadores=0;
    char* todosLosOperadores=sacarOperador(expresion, cantidadOperadores);
    cout<<"Expresion Ingresada: "<<expresion<<endl;
    cout<<"Operadores encontrados: "<<todosLosOperadores<<endl;
    cout<<"Cantidad de operadores: "<<cantidadOperadores<<endl;
    delete[] todosLosOperadores;
    return 0;
}