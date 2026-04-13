#ifndef ICONTENEDOR_H
#define ICONTENEDOR_H

template <typename T>
class IContenedor{
	public:
		virtual ~IContenedor() {}
		virtual T recuperar()=0;
		virtual void limpiar()=0;
		virtual bool estVacio()=0;
};

#endif
