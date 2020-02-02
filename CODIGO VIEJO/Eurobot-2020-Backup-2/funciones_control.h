#ifndef _FUNCIONES_CONTROL
#define _FUNCIONES_CONTROL

#include "lpc17xx.h"
#include "math.h"
#include "variables.h"
#include "funciones_pwm.h"


#define PI 3.141592

void calculo_d_frenada(cinematica *variable,param_mecanicos *mecanica);
void velocidad_derecha (double rad_por_s,param_mecanicos *mecanica);
void velocidad_izquierda (double rad_por_s,param_mecanicos *mecanica);
void calcula_parametros_cinematica (cinematica *variable, param_mecanicos *mecanica);
void calcula_error_rueda_derecha (cinematica *variable, param_mecanicos *mecanica);
void calcula_error_rueda_izquierda (cinematica *variable, param_mecanicos *mecanica);

#endif
