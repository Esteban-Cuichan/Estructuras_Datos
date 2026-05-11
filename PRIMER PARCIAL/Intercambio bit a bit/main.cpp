#include <iostream>
#include <string>
using namespace std;

int main(void){
    int a=10, b=25;
    cout<<"Antes: a="<<a<<"\t b="<<b<<endl;
    cout<<"-------------------------"<<endl;
    a ^= b;
    b ^= a;
    a ^= b;
    cout << "DESPUES: a = " << a << ", b = " << b << endl;
    cout << "-----------------------------------" << endl;
    string nombre1="PEPITO1";
    string nombre2="ALBERTO";
    cout<<"Nombre originales: "<<nombre1<<"y"<<nombre2<<endl;
    char* p1=&nombre1.front();
    char* p2=&nombre2.front();
    for(int i=0;i<nombre1.length();i++){
        *(p1+i)^=*(p2+i);
        *(p2+i)^=*(p1+i);
        *(p1+i)^=*(p2+i);
    }
    cout<<"Nombres intercambiados: "<<nombre1<<"y"<<nombre2<<endl;
    return 0;
}