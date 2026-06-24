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

        template <typename F>
        Turno* busquedaHash(F extraerAtributo, const string& valorBuscar) {
            if (cabeza == nullptr) return nullptr;

            int tamano = 0;
            Nodo* actual = cabeza;
            do {
                tamano++;
                actual = actual->getSiguiente();
            } while (actual != cabeza);

            unsigned long hash = 5381;
            for (char c : valorBuscar) {
                hash = ((hash << 5) + hash) + c;
            }
            
            int indiceObjetivo = hash % tamano;

            actual = cabeza;
            for (int i = 0; i < indiceObjetivo; ++i) {
                actual = actual->getSiguiente();
            }

            Nodo* arranque = actual;
            do {
                if (extraerAtributo(actual->getTurno()) == valorBuscar) {
                    return actual->getTurno();
                }
                actual = actual->getSiguiente();
            } while (actual != arranque);

            return nullptr;
        }

        template <typename F>
        Turno* busquedaBinaria(F extraerAtributo, const string& valorBuscar) {
            if (cabeza == nullptr) return nullptr;

            int total = 0;
            Nodo* actual = cabeza;
            do {
                total++;
                actual = actual->getSiguiente();
            } while (actual != cabeza);

            Turno** arreglo = new Turno*[total];
            int indice = 0;
            actual = cabeza;
            do {
                *(arreglo + indice) = actual->getTurno();
                indice++;
                actual = actual->getSiguiente();
            } while (actual != cabeza);

            int izquierda = 0;
            int derecha = total - 1;
            Turno* encontrado = nullptr;

            while (izquierda <= derecha) {
                int medio = izquierda + (derecha - izquierda) / 2;
                string valorMedio = extraerAtributo(*(arreglo + medio));

                if (valorMedio == valorBuscar) {
                    encontrado = *(arreglo + medio);
                    break;
                }
                if (valorMedio < valorBuscar) {
                    izquierda = medio + 1;
                } else {
                    derecha = medio - 1;
                }
            }

            delete[] arreglo;
            return encontrado;
        }
};
#endif