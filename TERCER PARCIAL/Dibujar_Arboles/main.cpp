#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <map>
#include <climits>
#include "Nodo.h"
#include "sacarInfPantalla.h"

using namespace std;

Nodo* insertar(Nodo* raiz,int valor){
    if(raiz==nullptr){
        return new Nodo(valor);
    }
    if(valor<raiz->getValor()){
        raiz->setIzquierdo(insertar(raiz->getIzquierdo(),valor));
    }else{
        raiz->setDerecho(insertar(raiz->getDerecho(),valor));
    }
    return raiz;
}

void calcularPosiciones(Nodo* raiz, int nivel, map<int,int>& nextX, map<Nodo*,int>& posX) {
    if (raiz == nullptr) return;
    int separacion = 130;
    if (nextX.find(nivel) == nextX.end()) nextX[nivel] = 0;
    calcularPosiciones(raiz->getIzquierdo(), nivel + 1, nextX, posX);
    int xIzq = (raiz->getIzquierdo() != nullptr) ? posX[raiz->getIzquierdo()] : -1;
    int slotActual = nextX[nivel];
    nextX[nivel] += separacion;
    calcularPosiciones(raiz->getDerecho(), nivel + 1, nextX, posX);
    int xDer = (raiz->getDerecho() != nullptr) ? posX[raiz->getDerecho()] : -1;
    if (xIzq != -1 && xDer != -1) {
        posX[raiz] = (xIzq + xDer) / 2;
    } else if (xIzq != -1) {
        // Solo hijo izquierdo: padre queda en su slot, hijo ya está a la izquierda
        posX[raiz] = slotActual;
    } else if (xDer != -1) {
        // Solo hijo derecho: padre queda en su slot, hijo ya está a la derecha  
        posX[raiz] = slotActual;
    } else {
        posX[raiz] = slotActual;
    }
}

void dibujarArbolRT(HDC hdc,Nodo* raiz,int nivel,int espacioVertical,map<Nodo*,int>& posX,int offsetX,int yInicial,int ultimoValor){
    if(raiz==nullptr)return;
    int radio=20;
    int x=posX[raiz]+offsetX;
    int y=yInicial+nivel*espacioVertical;
    if(raiz->getIzquierdo()!=nullptr){
        int xHijo=posX[raiz->getIzquierdo()]+offsetX;
        int yHijo=yInicial+(nivel+1)*espacioVertical;
        MoveToEx(hdc,x,y,nullptr);
        LineTo(hdc,xHijo,yHijo);
        dibujarArbolRT(hdc,raiz->getIzquierdo(),nivel+1,espacioVertical,posX,offsetX,yInicial,ultimoValor);
    }
    if(raiz->getDerecho()!=nullptr){
        int xHijo=posX[raiz->getDerecho()]+offsetX;
        int yHijo=yInicial+(nivel+1)*espacioVertical;
        MoveToEx(hdc,x,y,nullptr);
        LineTo(hdc,xHijo,yHijo);
        dibujarArbolRT(hdc,raiz->getDerecho(),nivel+1,espacioVertical,posX,offsetX,yInicial,ultimoValor);
    }
    HBRUSH fondoNodo;
    HPEN bordeNodo;
    if(raiz->getValor()==ultimoValor){
        fondoNodo=CreateSolidBrush(RGB(255,255,255));
        bordeNodo=CreatePen(PS_SOLID,2,RGB(255,255,255));
        SetTextColor(hdc,RGB(0,0,0));
    }else{
        fondoNodo=CreateSolidBrush(RGB(0,102,204));
        bordeNodo=CreatePen(PS_SOLID,2,RGB(51,153,255));
        SetTextColor(hdc,RGB(255,255,255));
    }
    SelectObject(hdc,fondoNodo);
    SelectObject(hdc,bordeNodo);
    Ellipse(hdc,x-radio,y-radio,x+radio,y+radio);
    string texto=to_string(raiz->getValor());
    SetBkMode(hdc,TRANSPARENT);
    int offset=(texto.length()>1)?8:4;
    TextOutA(hdc,x-offset,y-7,texto.c_str(),texto.length());
    DeleteObject(fondoNodo);
    DeleteObject(bordeNodo);
}

int main(){
    HWND hwnd=GetConsoleWindow();
    ShowWindow(hwnd,SW_MAXIMIZE);
    int anchoRealPantalla=GetSystemMetrics(SM_CXSCREEN);
    int altoRealPantalla=GetSystemMetrics(SM_CYSCREEN);
    obtenerYMostrarInfoPantalla(anchoRealPantalla,altoRealPantalla);
    cout<<"\nPresiona ENTER para iniciar el programa con el algoritmo optimizado...";
    cin.get();
    HDC hdc=GetDC(hwnd);
    HPEN lapizRamas=CreatePen(PS_SOLID,2,RGB(200,200,200));
    SelectObject(hdc,lapizRamas);
    Nodo* raiz=nullptr;
    int valor=0;
    int ultimoValor=-99999;
    while(true){
        system("cls");
        InvalidateRect(hwnd,nullptr,TRUE);
        UpdateWindow(hwnd);
        Sleep(40);
        if(raiz!=nullptr){
            map<int,int> nextX;
            map<Nodo*,int> posX;
            calcularPosiciones(raiz,0,nextX,posX);
            int minX=INT_MAX,maxX=INT_MIN;
            for(auto& par:posX){
                minX=min(minX,par.second);
                maxX=max(maxX,par.second);
            }
            int anchoArbol=maxX-minX;
            int offsetX=(anchoRealPantalla/2)-(anchoArbol/2)-minX;
            if(offsetX+minX<40){
                offsetX=40-minX;
            }
            dibujarArbolRT(hdc,raiz,0,75,posX,offsetX,90,ultimoValor);
        }
        HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord={0,0};
        SetConsoleCursorPosition(hConsole,coord);
        cout<<"[Escribe un numero y presiona ENTER. Usa -1 para salir]: ";
        if(cin>>valor){
            if(valor==-1){
                break;
            }
            raiz=insertar(raiz,valor);
            ultimoValor=valor;
        }else{
            cin.clear();
            cin.ignore(10000,'\n');
        }
    }
    DeleteObject(lapizRamas);
    ReleaseDC(hwnd,hdc);
    if(raiz!=nullptr)delete raiz;
    return 0;
}