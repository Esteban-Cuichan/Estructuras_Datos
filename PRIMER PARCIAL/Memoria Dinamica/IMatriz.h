#ifndef IMATRIZ_H
#define IMATRIZ_H

template <typename T>
class IMatriz{
	public:
		virtual ~IMatriz(){}
		virtual void setElemento(int fil, int col, T valor)=0;
		virtual T getElemento(int fil, int col)const=0;
		virtual void setMatriz(T** nuevaMatriz)=0;
		virtual T** getMatriz()const=0;
		virtual int getFila() const = 0;
        virtual int getColumna() const = 0;
};

#endif
