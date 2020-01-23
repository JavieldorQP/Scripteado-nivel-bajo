#ifndef _COMPROBACION
#define _COMPROBACION
#include "lpc17xx.h"
#include "uart.h"
extern int flag_ok;
typedef struct{
	float pos_x;							//Posición en la coordenada X
	float pos_y;							//Posición en la coordenada Y
	float omega;}							//Ángulo respecto a X en grados
pos;
void funcion_comunicacion();
#endif 	
	