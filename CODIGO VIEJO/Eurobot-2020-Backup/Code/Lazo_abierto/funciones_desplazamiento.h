#ifndef _FUNCIONES_DESPLAZAMIENTO
#define _FUNCIONES_DESPLAZAMIENTO
#include "lpc17xx.h"
#include <funciones_pwm.h>
#include <variables.h>
#include <stdint.h>
#include "math.h"

#define FPCLK 				25e6																//Frecuencia de la LPC de 25MHz


#define PI        	  3.14159

void configuracion_parametros_mecanicos (param_mecanicos *mecanica);			//Parametros mecanicos de controladora+motor
void calculo_t_acel(cinematica *variable,param_mecanicos *mecanica);			//Esta funcion calcula el tiempo que tarda en cambiar de velocidad inicial a final
void calculo_d_acel(cinematica *variable,param_mecanicos *mecanica);			// Esta funcion calcula la distancia recorridad durante el periodo de aceleraci�n
void calculo_d_frenada(cinematica *variable,param_mecanicos *mecanica);		// Esta funcion calcula la distancia recorridad durante el periodo de frenado
void calculo_t_vel_cte (cinematica *variable,param_mecanicos *mecanica);	// Esta funcion calcula el tiempo que debe estar a velocidad final antes de frenar
void velocidad_derecha (double rad_por_s,param_mecanicos *mecanica);			//Esta funci�n pone en la rueda derecha la velocidad que se le indica en rad/s
void velocidad_izquierda (double rad_por_s,param_mecanicos *mecanica);		//Esta funci�n pone en la rueda izquierda la velocidad que se le indica en rad/s
void avanza_recto(cinematica *variable,param_mecanicos *mecanica);				//FUNCION GORDACA DE IR PA' LANTE

#endif