#ifndef _FUNCIONES_PWM
#define _FUNCIONES_PWM
#include "lpc17xx.h"
#include "math.h"

void init_pwm(void);
void v_derecha (int V);
void v_izquierda (int V);
void PWM2_SetDuty (float duty);
void PWM3_SetDuty (float duty);
#endif