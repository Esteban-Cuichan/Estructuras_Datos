#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int TAM_CASILLA=60;

struct DatosVentana{
    int** matriz;
    int tamano;
    int alfilFila;
    int alfilCol;
    int** movimientosValidos;
    int cantidadMovimientos;
};

int** crearMatriz(int tamano){
    int** matriz=new int*[tamano];
    for(int i=0;i<tamano;i++){
        *(matriz+i)=new int[tamano];
        for(int j=0;j<tamano;j++){
            *(*(matriz+i)+j)=0;
        }
    }
    return matriz;
}

void liberarMatriz(int**& matriz,int tamano){
    if(matriz){
        for(int i=0;i<tamano;i++){
            delete[] *(matriz+i);
        }
        delete[] matriz;
        matriz=nullptr;
    }
}

bool crearArchivoTablero(int tamano){
    ofstream archivo("tablero.txt");
    if(!archivo.is_open()){
        cout<<"Error al crear el archivo de texto."<<endl;
        return false;
    }
    for(int i=0; i<tamano; i++){
        for(int j=0; j<tamano; j++){
            if((i+j)%2==0){
                archivo<<"1 ";
            }else{
                archivo<<"0 ";
            }
        }
        archivo<<"\n";
    }
    archivo.close();
    cout<<"[OK] Archivo 'tablero.txt' generado correctamente."<<endl;
    return true;
}

bool leerArchivoTablero(int** matriz,int tamano){
    if(!matriz) return false;
    ifstream archivo("tablero.txt");
    if(!archivo.is_open()){
        cout<<"Error al abrir el archivo 'tablero.txt' para lectura."<<endl;
        return false;
    }
    int valor;
    for(int i=0; i<tamano; i++){
        for(int j=0; j<tamano; j++){
            if(archivo>>valor){
                *(*(matriz+i)+j)=valor;
            }else{
                *(*(matriz+i)+j)=0;
            }
        }
    }
    archivo.close();
    cout<<"[OK] Archivo 'tablero.txt' leido correctamente."<<endl;
    return true;
}

void calcularMovimientosAlfil(int fila,int col,int tamano,int**& movimientos,int& cantidad){
    if(movimientos){
        for(int i=0; i<cantidad; i++){
            delete[] *(movimientos+i);
        }
        delete[] movimientos;
    }
    
    int desplazamientos[4][2]={{-1,-1},{-1,1},{1,-1},{1,1}};
    
    cantidad=0;
    for(int d=0; d<4; d++){
        int nuevaFila=fila+desplazamientos[d][0];
        int nuevoCol=col+desplazamientos[d][1];
        
        while(nuevaFila>=0 && nuevaFila<tamano && nuevoCol>=0 && nuevoCol<tamano){
            cantidad++;
            nuevaFila+=desplazamientos[d][0];
            nuevoCol+=desplazamientos[d][1];
        }
    }
    
    movimientos=new int*[cantidad];
    for(int i=0; i<cantidad; i++){
        *(movimientos+i)=new int[2];
    }
    int indice=0;
    for(int d=0; d<4; d++){
        int nuevaFila=fila+desplazamientos[d][0];
        int nuevoCol=col+desplazamientos[d][1];
        
        while(nuevaFila>=0 && nuevaFila<tamano && nuevoCol>=0 && nuevoCol<tamano){
            *(*(movimientos+indice)+0)=nuevaFila;
            *(*(movimientos+indice)+1)=nuevoCol;
            indice++;
            nuevaFila+=desplazamientos[d][0];
            nuevoCol+=desplazamientos[d][1];
        }
    }
}

void liberarMovimientos(int**& movimientos,int cantidad){
    if(movimientos){
        for(int i=0; i<cantidad; i++){
            delete[] *(movimientos+i);
        }
        delete[] movimientos;
        movimientos=nullptr;
    }
}

void dibujarAlfil(HDC hdc,int fila,int col,int tamano,int margenX,int margenY){
    int espacioEtiquetas=30;
    int margenXConEtiquetas=margenX+espacioEtiquetas;
    int margenYConEtiquetas=margenY+espacioEtiquetas;
    
    int x=margenXConEtiquetas+col*TAM_CASILLA+TAM_CASILLA/2;
    int y=margenYConEtiquetas+fila*TAM_CASILLA+TAM_CASILLA/2;
    
    HBITMAP hBitmap=(HBITMAP)LoadImageA(NULL,"Alfil.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    if(hBitmap==NULL){
        return;
    }
    
    BITMAP bm;
    GetObject(hBitmap,sizeof(bm),&bm);
    
    HDC hdcMem=CreateCompatibleDC(hdc);
    HBITMAP oldBitmap=(HBITMAP)SelectObject(hdcMem,hBitmap);
    
    int anchoImagen=TAM_CASILLA-10;
    int altoImagen=TAM_CASILLA-10;
    
    StretchBlt(hdc,x-anchoImagen/2,y-altoImagen/2,anchoImagen,altoImagen,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
    
    SelectObject(hdcMem,oldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(hBitmap);
}

void dibujarOrigenAlfil(HDC hdc,int fila,int col,int tamano,int margenX,int margenY){
    int espacioEtiquetas=30;
    int margenXConEtiquetas=margenX+espacioEtiquetas;
    int margenYConEtiquetas=margenY+espacioEtiquetas;
    
    int x=margenXConEtiquetas+col*TAM_CASILLA+TAM_CASILLA/2;
    int y=margenYConEtiquetas+fila*TAM_CASILLA+TAM_CASILLA/2;
    
    HBRUSH colorOrigen=CreateSolidBrush(RGB(255,255,0));
    HPEN penOrigen=CreatePen(PS_SOLID,2,RGB(200,200,0));
    HPEN oldPen=(HPEN)SelectObject(hdc,penOrigen);
    HBRUSH oldBrush=(HBRUSH)SelectObject(hdc,colorOrigen);
    
    Ellipse(hdc,x-8,y-8,x+8,y+8);
    
    SelectObject(hdc,oldBrush);
    SelectObject(hdc,oldPen);
    DeleteObject(colorOrigen);
    DeleteObject(penOrigen);
}

void dibujarMovimientosValidos(HDC hdc,int** movimientos,int cantidad,int tamano,int margenX,int margenY){
    int espacioEtiquetas=30;
    int margenXConEtiquetas=margenX+espacioEtiquetas;
    int margenYConEtiquetas=margenY+espacioEtiquetas;
    
    HBRUSH colorMovimiento=CreateSolidBrush(RGB(0,255,0));
    HPEN penMovimiento=CreatePen(PS_SOLID,2,RGB(0,200,0));
    HPEN oldPen=(HPEN)SelectObject(hdc,penMovimiento);
    HBRUSH oldBrush=(HBRUSH)SelectObject(hdc,colorMovimiento);
    
    for(int i=0; i<cantidad; i++){
        int fila=*(*(movimientos+i)+0);
        int col=*(*(movimientos+i)+1);
        
        int x=margenXConEtiquetas+col*TAM_CASILLA+TAM_CASILLA/2;
        int y=margenYConEtiquetas+fila*TAM_CASILLA+TAM_CASILLA/2;
        
        Ellipse(hdc,x-6,y-6,x+6,y+6);
    }
    
    SelectObject(hdc,oldBrush);
    SelectObject(hdc,oldPen);
    DeleteObject(colorMovimiento);
    DeleteObject(penMovimiento);
}

void dibujarTablero(HWND hwnd,HDC hdc,int** matriz,int tamano){
    if(!matriz || tamano<=0) return;
    HBRUSH colorBlanco=CreateSolidBrush(RGB(255,255,255));
    HBRUSH colorNegro=CreateSolidBrush(RGB(0,0,0));
    RECT rectangulo;
    GetClientRect(hwnd,&rectangulo);
    int anchoTablero=tamano*TAM_CASILLA;
    int altoTablero=tamano*TAM_CASILLA;
    int margenX=(rectangulo.right-rectangulo.left-anchoTablero)/2;
    int margenY=(rectangulo.bottom-rectangulo.top-altoTablero)/2;
    if(margenX<0) margenX=0;
    if(margenY<0) margenY=0;
    
    int espacioEtiquetas=30;
    int margenXConEtiquetas=margenX+espacioEtiquetas;
    int margenYConEtiquetas=margenY+espacioEtiquetas;
    
    HPEN borde=CreatePen(PS_SOLID,1,RGB(100,100,100));
    HPEN oldPen=(HPEN)SelectObject(hdc,borde);
    SelectObject(hdc,GetStockObject(NULL_BRUSH));
    for(int fila=0; fila<tamano; fila++){
        for(int col=0; col<tamano; col++){
            RECT casilla;
            casilla.left=margenXConEtiquetas+col*TAM_CASILLA;
            casilla.top=margenYConEtiquetas+fila*TAM_CASILLA;
            casilla.right=casilla.left+TAM_CASILLA;
            casilla.bottom=casilla.top+TAM_CASILLA;
            if(*(*(matriz+fila)+col)==1){
                FillRect(hdc,&casilla,colorBlanco);
            }else{
                FillRect(hdc,&casilla,colorNegro);
            }
            Rectangle(hdc,casilla.left,casilla.top,casilla.right,casilla.bottom);
        }
    }
    
    SetBkMode(hdc,TRANSPARENT);
    SetTextColor(hdc,RGB(0,0,0));
    HFONT hFont=CreateFont(16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,NULL);
    HFONT oldFont=(HFONT)SelectObject(hdc,hFont);
    for(int col=0; col<tamano; col++){
        char buffer[10];
        sprintf_s(buffer,sizeof(buffer),"%d",col+1);
        int x=margenXConEtiquetas+col*TAM_CASILLA+TAM_CASILLA/2;
        int y=margenY+5;
        RECT rectCol={x-15,y,x+15,y+20};
        DrawTextA(hdc,buffer,-1,&rectCol,DT_CENTER|DT_VCENTER);
    }
    
    for(int fila=0; fila<tamano; fila++){
        char buffer[10];
        sprintf_s(buffer,sizeof(buffer),"%d",tamano-fila);
        int x=margenX+5;
        int y=margenYConEtiquetas+fila*TAM_CASILLA+TAM_CASILLA/2;
        RECT rectFila={x,y-10,x+20,y+10};
        DrawTextA(hdc,buffer,-1,&rectFila,DT_CENTER|DT_VCENTER);
    }
    
    SelectObject(hdc,oldFont);
    DeleteObject(hFont);
    SelectObject(hdc,oldPen);
    DeleteObject(borde);
    DeleteObject(colorBlanco);
    DeleteObject(colorNegro);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
    switch(message){
        case WM_CREATE:{
            CREATESTRUCT* pCreate=(CREATESTRUCT*)lParam;
            DatosVentana* pDatos=(DatosVentana*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pDatos);
            break;
        }
        case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc=BeginPaint(hwnd,&ps);
            DatosVentana* pDatos=(DatosVentana*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
            if(pDatos && pDatos->matriz){
                dibujarTablero(hwnd,hdc,pDatos->matriz,pDatos->tamano);
                if(pDatos->alfilFila>=0 && pDatos->alfilCol>=0){
                    RECT rectangulo;
                    GetClientRect(hwnd,&rectangulo);
                    int anchoTablero=pDatos->tamano*TAM_CASILLA;
                    int altoTablero=pDatos->tamano*TAM_CASILLA;
                    int margenX=(rectangulo.right-rectangulo.left-anchoTablero)/2;
                    int margenY=(rectangulo.bottom-rectangulo.top-altoTablero)/2;
                    if(margenX<0) margenX=0;
                    if(margenY<0) margenY=0;
                    
                    dibujarOrigenAlfil(hdc,pDatos->alfilFila,pDatos->alfilCol,pDatos->tamano,margenX,margenY);
                    
                    if(pDatos->movimientosValidos && pDatos->cantidadMovimientos>0){
                        dibujarMovimientosValidos(hdc,pDatos->movimientosValidos,pDatos->cantidadMovimientos,pDatos->tamano,margenX,margenY);
                    }
                    
                    dibujarAlfil(hdc,pDatos->alfilFila,pDatos->alfilCol,pDatos->tamano,margenX,margenY);
                }
            }
            EndPaint(hwnd,&ps);
            break;
        }
        case WM_SIZE:{
            InvalidateRect(hwnd,NULL,TRUE);
            break;
        }
        case WM_DESTROY:{
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd,message,wParam,lParam);
    }
    return 0;
}

HWND crearVentana(HINSTANCE hInstance,DatosVentana* pDatos,int ancho,int alto){
    WNDCLASS wc={};
    wc.lpfnWndProc=WindowProcedure;
    wc.hInstance=hInstance;
    wc.lpszClassName=TEXT("ClaseTablaTxt");
    wc.hbrBackground=(HBRUSH)(COLOR_BACKGROUND);
    if(!RegisterClass(&wc)){
        cout<<"Error al registrar la ventana."<<endl;
        return NULL;
    }
    HWND hwnd=CreateWindowEx(
        0,
        TEXT("ClaseTablaTxt"),
        TEXT("Tablero de Ajedrez - Alfil"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        ancho,alto,
        NULL,NULL,hInstance,
        pDatos
    );
    return hwnd;
}

int main(){
    int n = 8;
    cout<<"========================================="<<endl;
    cout<<"\t\tTABLERO DEL ALFIL (8x8)"<<endl;
    cout<<"========================================="<<endl;

    int** matrizTablero=crearMatriz(n);
    if(!crearArchivoTablero(n)){
        liberarMatriz(matrizTablero,n);
        return 1;
    }
    if(!leerArchivoTablero(matrizTablero,n)){
        liberarMatriz(matrizTablero,n);
        return 1;
    }
    HINSTANCE hInstance=GetModuleHandle(NULL);
    DatosVentana datos;
    datos.matriz=matrizTablero;
    datos.tamano=n;
    datos.alfilFila=-1;
    datos.alfilCol=-1;
    datos.movimientosValidos=nullptr;
    datos.cantidadMovimientos=0;
    int anchoVentana=(n*TAM_CASILLA)+120;
    int altoVentana=(n*TAM_CASILLA)+140;
    HWND hwnd=crearVentana(hInstance,&datos,anchoVentana,altoVentana);
    if(hwnd==NULL){
        liberarMatriz(matrizTablero,n);
        return 1;
    }
    ShowWindow(hwnd,SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    
    MSG msg={};
    bool ventanaAbierta=true;
    
    while(ventanaAbierta){
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            if(msg.message==WM_QUIT){
                ventanaAbierta=false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if(!ventanaAbierta) break;
        
        int fila,col;
        cout<<"\n========================================="<<endl;
        cout<<"Ingrese las coordenadas del alfil"<<endl;
        cout<<"Columna (1 a "<<n<<"): ";
        cin>>col;
        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout<<"Error: Debe ingresar un numero."<<endl;
            continue;
        }
        cout<<"Fila (1 a "<<n<<"): ";
        cin>>fila;
        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout<<"Error: Debe ingresar un numero."<<endl;
            continue;
        }
        
        if(col<1 || col>n || fila<1 || fila>n){
            cout<<"Coordenadas invalidas. Intente de nuevo."<<endl;
        }else{
            datos.alfilCol=col-1;
            datos.alfilFila=n-fila;
            
            calcularMovimientosAlfil(datos.alfilFila,datos.alfilCol,n,datos.movimientosValidos,datos.cantidadMovimientos);
            
            cout<<"[OK] Alfil colocado en columna "<<col<<", fila "<<fila<<endl;
            cout<<"Movimientos posibles: "<<datos.cantidadMovimientos<<endl;
            InvalidateRect(hwnd,NULL,TRUE);
        }
    }
    
    liberarMovimientos(datos.movimientosValidos,datos.cantidadMovimientos);
    liberarMatriz(matrizTablero,n);
    return 0;
}