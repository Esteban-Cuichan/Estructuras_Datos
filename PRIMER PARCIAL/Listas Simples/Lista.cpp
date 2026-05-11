#include "Lista.h"
#include "Operaciones.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//include <regex>
//#include <cctype>
using namespace std;

string limpiarEspacios(string str){
    if(!str.empty()&&(str.back()=='\r' || str.back()=='\n')){
        str.pop_back();
    }
    return str;
}

Lista::Lista(){
    this->cabeza=nullptr;
    this->cola=nullptr;
}

void Lista::insertar(string cedula,string nombre){
    Nodo* nuevo=new Nodo(cedula,nombre,nullptr);
    if(cabeza==nullptr){
        cabeza=nuevo;
        cola=nuevo;
    }else{
        nuevo->setSiguiente(cabeza);
        cabeza=nuevo;
    }
}

void Lista::imprimir(){
    if(cabeza==nullptr){
        cout<<"No se ha podido encontrar ningun dato ingresado!"<<endl;
        cout<<"LISTA VACÍA!"<<endl;
    }else{
        Nodo* aux=cabeza;
        cout<<"Lista de registros:"<<endl;
        while(aux!=nullptr){
            cout<<"Cedula: "<<aux->getCedula()<<"| Nombre: "<<aux->getNombre()<<endl;
            aux=aux->getSiguiente();
        }
    }
}

Nodo* Lista::buscar(string cedula){
    string cedula_buscar=cedula;
    Nodo* aux=cabeza;
    while(aux!=nullptr){
        if(aux->getCedula()==cedula_buscar){
            return aux;
        }else{
            aux=aux->getSiguiente();
        }
    }
    return nullptr;
}

void Lista::eliminar(string cedula){
    Nodo* actual=cabeza;
    Nodo* anterior=nullptr;
    while(actual!=nullptr){
        if(actual->getCedula()==cedula){
            if(actual==cabeza){
                cabeza=cabeza->getSiguiente();
            }else{
                anterior->setSiguiente(actual->getSiguiente());
            }
            if(actual==cola){
                cola=anterior;
            }
            delete actual;
            return;
        }
        anterior=actual;
        actual=actual->getSiguiente();
    }
    cout<<"No se encontro ningun dato que coincida!"<<endl;
}

string Lista::buscarProvincia(string prefijo){
    ifstream archivo("provincias.txt");
    string prefijoArchivo;
    string provincia;
    if(!archivo.is_open()){
        cout<<"No se ha encontrado el archivo indicado"<<endl;
    }

    while(getline(archivo,provincia,',')){
        if(getline(archivo,prefijoArchivo)){
            if(prefijoArchivo==prefijo){
                archivo.close();
                return provincia;
            }
        }
    }

    archivo.close();
    return "Provincia no encontrada!";
}

int Lista::contarCedulas(string prefijo){
    if(cabeza==nullptr) return 0;
    int suma=0;
    Nodo* aux=cabeza;
    while(aux!=nullptr){
        if(aux->getCedula().substr(0,2)==prefijo){
            suma++;
        }
        aux=aux->getSiguiente();
    }
    return suma;
}

void Lista::conteoGeneral(){
    ifstream archivo("provincias.txt");
    string prefijo;
    string provincia;
    if(!archivo.is_open()){
        cout<<"No se ha podido encontrar el archivo indicado!"<<endl;
        return;
    }

    cout<<"==============================================="<<endl;
    cout<<"     PROVINCIA        ||       CANTIDAD        "<<endl;
    cout<<"-----------------------------------------------"<<endl;
    while(getline(archivo,provincia,',')){
        if(getline(archivo,prefijo)){
            prefijo=limpiarEspacios(prefijo);
            int cantidad=contarCedulas(prefijo);
            int conteo=0;
            ifstream usuarios("usuarios.txt");
            string nombre,cedula;
            if(archivo.is_open()){
                while(getline(usuarios,nombre,',')){
                    if(getline(usuarios,cedula)){
                        cedula=limpiarEspacios(cedula);
                        if(cedula.substr(0,2)==prefijo){
                            conteo++;
                        }
                    }
                }
                usuarios.close();             
            }
            printf(" %-20s ||      %d\n", provincia.c_str(), conteo);
        }
    }
    cout<<"==============================================="<<endl;
    archivo.close();
}

void Lista::guardarArchivoIngreso(string nombre,string cedula){
    ofstream archivo("usuarios.txt",ios::app);
    if(archivo.is_open()){
        archivo<<nombre<<","<<cedula<<endl;
        archivo.close();
        cout<<"Agregado correctamente al archivo.txt"<<endl;
    }else{
        cout<<"Error al crear el archivo!"<<endl;
    }
}

bool Lista::busquedaDuplicado(string cedula){
    ifstream archivo("usuarios.txt");
    string cedulaArchivo;
    string nombreUs;
    if(!archivo.is_open()){
        return false;
    }

    while(getline(archivo,nombreUs,',')){
        if(getline(archivo,cedulaArchivo)){
            cedulaArchivo=limpiarEspacios(cedulaArchivo);
            if(cedulaArchivo==cedula){
                archivo.close();
                return true;
            }
        }
    }
    archivo.close();
    return false;
}

void Lista::modificar(string cedula,string nombre){
    ifstream archivo("usuarios.txt");
    ofstream temporal("tempo.txt");
    string cedarchivo,nomarchivo;
    bool encontrado=false;

    if(!archivo.is_open()){
        cout<<"Error al abrir el archivo!"<<endl;
        return;
    }

    while(getline(archivo,nomarchivo,',')){
        if(getline(archivo,cedarchivo)){
            cedarchivo=limpiarEspacios(cedarchivo);
            if(cedarchivo==cedula){
                temporal<<nombre<<","<<cedarchivo<<endl;
            }else{
                temporal<<nomarchivo<<","<<cedarchivo<<endl;
            }
        }
        archivo.close();
        temporal.close();
        remove("usuarios.txt");
        rename("tempo.txt","usuarios.txt");
        if(encontrado){
            cout<<"Usuario modificado correctamente!"<<endl;
        }else{
            cout<<"No se encontro la cedula a modificar!"<<endl;
        }
    }
}

void Lista::eliminarUsuario(string cedulaAEliminar){
    ifstream archivoOriginal("usuarios.txt");
    ofstream archivoTemporal("temporal.txt");
    string nombreUs,cedulaUs;
    bool encontrado=false;

    if (!archivoOriginal.is_open()){
        cout<<"Error al abrir el archivo!"<<endl;
        return;
    }

    while (getline(archivoOriginal,nombreUs,',')) {
        if (getline(archivoOriginal,cedulaUs)) {
            cedulaUs=limpiarEspacios(cedulaUs);
            if(cedulaUs!=cedulaAEliminar) {
                archivoTemporal<<nombreUs<<","<<cedulaUs<<endl;
            }else{
                encontrado=true;
            }
        }
    }

    archivoOriginal.close();
    archivoTemporal.close();
    remove("usuarios.txt");
    rename("temporal.txt", "usuarios.txt");
    if(encontrado) {
        cout<<"Usuario con cedula"<<cedulaAEliminar<<" eliminado del archivo."<< endl;
    }else{
        cout<<"No se encontro la cedula en el archivo."<<endl;
    }
}

string Lista::buscarUsuario(string cedula){
    ifstream archivo("usuarios.txt");
    string cedulaArchivo;
    string nombre;
    if(!archivo.is_open()){
        cout<<"No se pudo encontrar el archivo"<<endl;
    }
    while(getline(archivo,nombre,',')){
        if(getline(archivo,cedulaArchivo)){
            cedulaArchivo=limpiarEspacios(cedulaArchivo);
            if(cedulaArchivo==cedula){
                archivo.close();
                return nombre;
            }
        }
    }
    archivo.close();
    return "Usuario no encontrado!";
}

void Lista::setCabeza(Nodo* cabeza){
    this->cabeza=cabeza;
}

void Lista::setCola(Nodo* cola){
    this->cola=cola;
}

Nodo* Lista::getCabeza(){
    return this->cabeza;
}

Nodo* Lista::getCola(){
    return this->cola;
}

void Lista::limpiarpantalla(){
    cout<<"Presione ENTER para continuar...";
    cin.ignore();
    cin.get();
    system("cls");
}

