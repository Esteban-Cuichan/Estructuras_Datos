#ifndef LISTADCE_H
#define LISTADCE_H
#include "Nodo.h"
#include <iostream>
using namespace std;

class ListaDCE{
    private:
        Nodo* cabeza;
        Nodo* cola;
    public:
        ListaDCE();
        ListaDCE(Nodo*,Nodo*);
        ~ListaDCE();
        Nodo* getCabeza();
        Nodo* getCola();
        void setCabeza(Nodo*);
        void setCola(Nodo*);
        void insertar(Turno*);
        bool eliminarPorID(int);
        Nodo* buscarPorID(int);
        bool modificar(int,Fecha);
        void mostrarLista();
        void guardarListaEnArchivo();
        void guardarOrdenamiento();
        int generarSiguienteID();
        void cargarListaDesdeArchivo();
        void cifrarPlacas();
        template<typename F>
        void ordenamiento(F extraerAtributo){
            if(cabeza==nullptr||cabeza->getSiguiente()==cabeza)return;
            bool intercambio;
            int recorrido=1;
            do{
                intercambio=false;
                Nodo* actual=cabeza;
                do{
                    Nodo* siguiente=actual->getSiguiente();
                    system("cls");
                    cout<<"================================================"<<endl;
                    cout<<"               ORDENAMIENTO ANIMADO             "<<endl;
                    cout<<"================================================"<<endl;
                    cout<<"No. de veces que se recorrio el bucle: "<<recorrido<<endl;
                    Nodo* temp=cabeza;
                    do{
                        if(temp==actual||temp==siguiente){
                            cout<<"[*] ID: "<<temp->getTurno()->getIDturno()<<" - Propietario: "<<temp->getTurno()->getUsuario().getApellido()<<endl;
                        }else{
                            cout<<"    ID: "<<temp->getTurno()->getIDturno()<<" - Propietario: "<<temp->getTurno()->getUsuario().getApellido()<<endl;
                        }
                        temp=temp->getSiguiente();
                    }while(temp!=cabeza);
                    Sleep(500);
                    if(extraerAtributo(actual->getTurno())>extraerAtributo(siguiente->getTurno())){
                        Sleep(200);
                        uintptr_t ptr1=reinterpret_cast<uintptr_t>(actual->getTurno());
                        uintptr_t ptr2=reinterpret_cast<uintptr_t>(siguiente->getTurno());
                        ptr1=ptr1^ptr2;ptr2=ptr1^ptr2;ptr1=ptr1^ptr2;
                        actual->setTurno(reinterpret_cast<Turno*>(ptr1));
                        siguiente->setTurno(reinterpret_cast<Turno*>(ptr2));
                        intercambio=true;
                    }
                    actual=actual->getSiguiente();
                }while(actual->getSiguiente()!=cabeza);
                recorrido++;
            }while(intercambio);
            guardarOrdenamiento();
        }

        template<typename F,typename V>
        Turno* busquedaBinaria(F extraerAtributo,V valor){
            if(cabeza==nullptr)return nullptr;
            int n=0;
            Nodo* inicio=cabeza;
            do{
                n++;
                temp=temp->getSiguiente();
            }while(temp!=cabeza);
            uintptr_t* tabla=new uintptr_t[n];
            temp=cabeza;
            for(int i=0;i<n;i++){
                *(tabla+i)=reinterpret_cast<uintptr_t>(temp);
                temp=temp->getSiguiente();
            }
            int izquierda=0,derecha=n-1;
            Turno* resultado=nullptr;
            while(izquierda<=derecha){
                int medio=(izquierda+derecha)/2;
                Nodo* nodoMedio=reinterpret_cast<Nodo*>(*(tabla+medio));
                auto atributoActual=extraerAtributo(nodoMedio->getTurno());
                if(atributoActual==valor){
                    resultado=nodoMedio->getTurno();
                    break;
                }else if(atributoActual<valor){
                    izquierda=medio+1;
                }else{
                    derecha=medio-1;
                }
            }
            delete[] tabla;
            return resultado;
        }
};
#endif