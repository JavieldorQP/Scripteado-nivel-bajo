#include "lpc17xx.h"
#include "variables.h"
#include "funciones_control.h"

cinematica variable;
param_mecanicos mecanica;

void TIMER1_IRQHandler(void)
{
	
	LPC_TIM1->IR|=(1<<1); // borro el flag de la interrupcion
	
	calcula_error_rueda_izquierda (&variable, &mecanica);
	calcula_error_rueda_derecha (&variable, &mecanica);
	
}
