#ifndef IVALIDAR_H
#define IVALIDAR_H

template <typename T>
class IValidar{
	public:
		virtual ~IValidar() {}
		virtual bool validar(T documento) = 0;
};

#endif
