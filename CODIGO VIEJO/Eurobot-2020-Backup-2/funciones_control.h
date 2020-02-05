#ifndef _FUNCIONES_CONTROL
#define _FUNCIONES_CONTROL

#include "lpc17xx.h"
#include "math.h"
#include "variables.h"
#include "funciones_pwm.h"

void calculo_de_frenada(cinematica *variable,param_mecanicos *mecanica);
void calcula_parametros_recta (cinematica *variable, param_mecanicos *mecanica);
void calcula_parametros_giro (cinematica *variable, param_mecanicos *mecanica);
void calcula_error_rueda_derecha (cinematica *variable, param_mecanicos *mecanica);
void calcula_error_rueda_izquierda (cinematica *variable, param_mecanicos *mecanica);


#endif
