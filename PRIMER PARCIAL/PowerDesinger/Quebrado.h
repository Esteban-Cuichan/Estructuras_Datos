/***********************************************************************
 * Module:  Quebrado.h
 * Author:  Esteban
 * Modified: jueves, 9 de abril de 2026 20:45:02
 * Purpose: Declaration of the class Quebrado
 ***********************************************************************/

#if !defined(__ObjectOrientedModel_1_Quebrado_h)
#define __ObjectOrientedModel_1_Quebrado_h

class Quebrado
{
public:
   Quebrado(float num, float den);
   float getNumerador(void);
   float getDenominador(void);
   void setNumerador(float num);
   void setDenominador(float den);

protected:
private:
   float numerador;
   float denominador;


};

#endif