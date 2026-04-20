#ifndef IMATRIZ_H
#define IMATRIZ_H

template <typename T>
class IMatriz{
    public:
        virtual ~IMatriz(){}
        virtual void setElemento(int,int,T)=0;
        virtual T getElemento(int,int)const=0;
		virtual void setMatriz(T**)=0;
		virtual T** getMatriz()const=0;
		virtual int getFila() const = 0;
        virtual void setFila(int)=0;
        virtual int getColumna() const = 0;
        virtual void setColumna(int)=0;
        virtual void encerar(T**,int,int)=0;
};

#endif