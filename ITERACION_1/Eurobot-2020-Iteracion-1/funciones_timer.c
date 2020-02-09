#include "lpc17xx.h"
#include "funciones_pwm.h"
#include "variables.h"
#include "funciones_desplazamiento.h"
#include "funciones_control.h"
#define Fpclk 25e6
#define T_muestreo 0.02 

extern cinematica lazo_abierto;
extern param_mecanicos maxon;

extern int contador;

void TIMER1_IRQHandler(void)
{
	
	LPC_TIM1->IR|=(1<<1); // borro el flag de la interrupcion
	
	calcula_error_rueda_izquierda (&lazo_abierto, &maxon);
	calcula_error_rueda_derecha (&lazo_abierto, &maxon);

	calcula_error_rueda_derecha_final (&lazo_abierto, &maxon);
	calcula_error_rueda_izquierda_final (&lazo_abierto, &maxon);
	
	contador++;
}
