#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int* celda(int* tablero, int N, int fila, int columna)
{
    return tablero+fila*N+columna;
}

void inicializar(int* tablero, int N)
{
    int* fin=tablero+N*N;
    while (tablero<fin)
    {
        *tablero=0;
        tablero++;
    }
}

bool esValido(int* tablero, int N, int subGrid, int fila, int columna, int numero)
{
    for(int c=0; c<N; c++)
        if(*celda(tablero, N, fila, c)==numero)
            return false;

    for(int f=0; f<N; f++)
        if(*celda(tablero, N, f, columna)==numero)
            return false;

    int inicioFila=(fila/subGrid)*subGrid;
    int inicioCol=(columna/subGrid)*subGrid;

    for(int f=0; f<subGrid; f++)
        for(int c=0; c<subGrid; c++)
            if(*celda(tablero, N, inicioFila+f, inicioCol+c)==numero)
                return false;

    return true;
}

bool resolver(int* tablero, int N, int subGrid, int posicion)
{
    if(posicion==N*N)
        return true;

    int fila=posicion/N;
    int columna=posicion%N;

    if(*celda(tablero, N, fila, columna)!=0)
        return resolver(tablero, N, subGrid, posicion+1);

    for(int numero=1; numero<=N; numero++)
    {
        if(esValido(tablero, N, subGrid, fila, columna, numero))
        {
            *celda(tablero, N, fila, columna)=numero;

            if(resolver(tablero, N, subGrid, posicion+1))
                return true;

            *celda(tablero, N, fila, columna)=0;
        }
    }
    return false;
}

void mezclarPrimerBloque(int* tablero, int N, int subGrid) 
{
    for (int f=0; f<subGrid; f++) {
        for (int c=0; c<subGrid; c++) {
            int num;
            do {
                num=(rand()%N)+1;
            } while (!esValido(tablero, N, subGrid, f, c, num));
            *celda(tablero, N, f, c)=num;
        }
    }
}

void copiarTablero(int* origen, int* destino, int N)
{
    int* fin=origen+N*N;
    while(origen<fin)
    {
        *destino=*origen;
        origen++;
        destino++;
    }
}

void vaciarCasillas(int* tablero, int N)
{
    int cantidad=(N*N)/2;
    for(int i=0; i<cantidad; i++)
    {
        int pos=rand()%(N*N);
        *(tablero+pos)=0;
    }
}

void imprimirLinea(int N, int subGrid)
{
    cout<<"+";
    for(int c=0; c<N; c++)
    {
        cout<<"---";
        if((c+1)%subGrid==0)
            cout<<"+";
    }
}

void imprimirTodos(int* memoria, int cantidadTableros, int N, int subGrid)
{
    for(int fila=0; fila<N; fila++)
    {
        if(fila%subGrid==0)
        {
            for(int t=0; t<cantidadTableros; t++)
            {
                imprimirLinea(N, subGrid);
                cout<<"     ";
            }
            cout<<"\n";
        }

        for(int t=0; t<cantidadTableros; t++)
        {
            int* tablero=memoria+t*N*N;

            for(int columna=0; columna<N; columna++)
            {
                if(columna%subGrid==0)
                    cout<<"|";

                int valor=*celda(tablero, N, fila, columna);

                if(valor==0)
                    cout<<" . ";
                else
                    cout<<" "<<valor<<" ";
            }
            cout<<"|";
            cout<<"     ";
        }
        cout<<"\n";
    }

    for(int t=0; t<cantidadTableros; t++)
    {
        imprimirLinea(N, subGrid);
        cout<<"     ";
    }
    cout<<"\n";
}

int main()
{
    srand((unsigned)time(NULL));

    int subGrid;
    cout<<"Dimension de la subcuadricula (Ej: 3 para 9x9): ";
    cin>>subGrid;

    int N=subGrid*subGrid;
    int cantidad;
    cout<<"Cantidad de tableros: ";
    cin>>cantidad;

    int* memoriaJuegos=new int[cantidad*N*N];
    int* memoriaSoluciones=new int[cantidad*N*N];

    for(int t=0; t<cantidad; t++)
    {
        int* tJuego=memoriaJuegos+t*N*N;
        int* tSolucion=memoriaSoluciones+t*N*N;

        inicializar(tJuego, N);
        mezclarPrimerBloque(tJuego, N, subGrid);
        resolver(tJuego, N, subGrid, 0);
        copiarTablero(tJuego, tSolucion, N);
        vaciarCasillas(tJuego, N);
    }

    cout<<"\n==========================================================\n";
    cout<<"                 TABLERO DE JUEGO (DESAFÍOS)              \n";
    cout<<"==========================================================\n";
    imprimirTodos(memoriaJuegos, cantidad, N, subGrid);

    cout<<"\n==========================================================\n";
    cout<<"                 TABLEROS DE SOLUCIÓN                      \n";
    cout<<"==========================================================\n";
    imprimirTodos(memoriaSoluciones, cantidad, N, subGrid);

    delete[] memoriaJuegos;
    delete[] memoriaSoluciones;

    system("pause");
    return 0;
}