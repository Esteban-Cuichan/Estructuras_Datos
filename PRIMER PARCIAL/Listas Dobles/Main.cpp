#include "Lista.h"
#include <iostream>
using namespace std;

int main(){
    string cedula,nombre;
    char aux;
    bool continuar=true;
    Lista* miLista=new Lista();
    while(continuar!=false){
        cout<<"-------- INGRESO DE DATOS ---------"<<endl;
        cout<<"Ingrese su cedula: ";
        cin>>cedula;
        cout<<"Ingrese su nombre: ";
        cin.ignore();
        getline(cin,nombre);
        miLista->insertar(cedula,nombre);
        cout<<"Desea agregar otro usuario? [Y/n]: ";
        cin>>aux;
        if(aux=='n' || aux=='N'){
            continuar=false;
        }else if(aux=='Y' || aux=='y'){
            continuar=true;
        }else{
            cout<<"Operador invalido! Saliendo por seguridad..."<<endl;
            return false;
        }
    }
    miLista->imprimir();
    delete miLista;
    return 0;
}