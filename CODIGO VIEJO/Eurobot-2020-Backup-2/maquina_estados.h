#ifndef _MAQUINA_ESTADOS
#define _MAQUINA_ESTADOS

#include "lpc17xx.h"
#include "variables.h"
#include "funciones_control.h"
#include "stdbool.h"
#include "math.h"

#define PI 3.141592

void avanza_recto (cinematica *variable, param_mecanicos *mecanica);

#endif
