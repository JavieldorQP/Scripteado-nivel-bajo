#ifndef _FUNCIONES_TIMER
#define _FUNCIONES_TIMER
#include "lpc17xx.h"
#include "funciones_pwm.h"
#include "variables.h"
#define Fpclk 25e6

extern cinematica lazo_abierto;
extern param_mecanicos maxon;
extern Caracterizacion Robot;
extern T_Counter cuadratura;

extern int contador;


void config_TIMER1(void);
void config_TIMER2(void);
void config_TIMER3(void);

void EINT2_IRQHandler (void);
void EINT3_IRQHandler (void);
void act_odom(Caracterizacion *situacion, T_Counter *contadores, param_mecanicos *mecanica);
#endif
