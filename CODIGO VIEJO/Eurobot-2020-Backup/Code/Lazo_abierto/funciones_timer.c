#include "lpc17xx.h"
#include <funciones_pwm.h>
#include <variables.h>
#define Fpclk 25e6

uint8_t fl_fin_timer;

void config_TIMER1(void)
{
	LPC_SC->PCONP|=(1<<2); //Alimento el periferico del TIM1
	LPC_TIM1->MCR|=(3<<3); //Cuando se produzca un match en el MR1 hago un reset y que interrumpa
	NVIC_EnableIRQ(TIMER1_IRQn); //Habilito la interrupcion del TIM1
	NVIC_SetPriority(TIMER1_IRQn,1); //Prioridad 1 al TIMER
}
void TIMER1_IRQHandler(void)
{
	
	LPC_TIM1->IR|=(1<<1); // borro el flag de la interrupcion
	
	fl_fin_timer=1;
	
	LPC_TIM1->TCR=0;			// Desactivamos el contador del timer
	LPC_TIM1->TC=0;				// Ponemos el registro del timer (TC) a 0
}