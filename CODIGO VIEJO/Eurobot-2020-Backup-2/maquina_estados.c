#include "lpc17xx.h"
#include "variables.h"
#include "funciones_control.h"
#include "stdbool.h"
#include "math.h"

#define PI 3.141592


void avanza_recto (cinematica *variable, param_mecanicos *mecanica)
{
		//Calculamos los parametros cinematicos
		calcula_parametros_cinematica (variable, mecanica);
	
		//Ponemos las velocidades en las ruedas
		velocidad_derecha(variable->velocidad_final, mecanica);
		velocidad_izquierda(variable->velocidad_final, mecanica);

}
