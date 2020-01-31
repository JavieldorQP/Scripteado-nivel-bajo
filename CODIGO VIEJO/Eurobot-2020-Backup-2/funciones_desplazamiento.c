#include "lpc17xx.h"
#include "funciones_pwm.h"
#include <stdint.h>
#include "math.h"
#include "variables.h"

#define FPCLK 				25e6																//Frecuencia de la LPC de 25MHz

#define PI        	  3.14159
											
//NOTA IMPORTANTE: he definido el struct en un fichero '.h' aparte para que pueda acceder a los campos de esta estructura desde cualquier 
//fichero '.c' que haya creado. Adem�s para asegurarme que se cambian los valores de los campos del struct he usado punteros donde envio la
//direccion de memoria del struct


void configuracion_parametros_mecanicos (param_mecanicos *mecanica)
{
	mecanica->aceleracion = 3000;
	mecanica->reductora = 26;
	mecanica->diametro = 12.2;
	mecanica->vel_eje_max = 6380;
	mecanica->vel_max = mecanica->vel_eje_max * PI / 30 / mecanica->reductora;
}


void calculo_t_acel(cinematica *variable,param_mecanicos *mecanica)			//Esta funcion calcula el tiempo que tarda en cambiar de velocidad inicial a final
{
	variable->tiempo_acel = fabs ( ( variable->velocidad_final - variable->velocidad_inicial ) / ( mecanica->aceleracion * PI / 30 * ( 1 / mecanica->reductora ) ) );
		// la formula es t=|v_fin-v_ini/a|
}	

void calculo_d_acel(cinematica *variable,param_mecanicos *mecanica)			// Esta funcion calcula la distancia recorridad durante el periodo de aceleraci�n
{
	variable->distancia_acel = fabs ( ( pow(variable->velocidad_final,2) - pow(variable->velocidad_inicial,2) ) 
		/ ( 2 * mecanica->aceleracion * PI / 30 * ( 1 / mecanica->reductora ) ) );
		// la formula es d=|(v_fin^2-v_ini^2)/2a|
}

void calculo_d_frenada(cinematica *variable,param_mecanicos *mecanica)		// Esta funcion calcula la distancia recorridad durante el periodo de frenado
{
	variable->distancia_frenada = fabs ( pow(variable->velocidad_final,2) ) 
		/ ( 2 * mecanica->aceleracion * PI / 30 * ( 1 / mecanica->reductora ) );
		// la formula es d=|(v_fin^2-v_ini^2)/2a| donde v_fin vale 0 por ser MEGA FRENADA EXTREMA y la v_ini es la v_fin del anterior
}

void calculo_t_vel_cte (cinematica *variable)	// Esta funcion calcula el tiempo que debe estar a velocidad final antes de frenar
{
	variable->tiempo_vel_cte = fabs ( ( variable->distancia_total - variable->distancia_acel ) / variable->velocidad_final );
		//la formula es t=|(d_total-d_acel-d_decel)/v_fin|
}

void velocidad_derecha (double rad_por_s,param_mecanicos *mecanica)		//Esta funci�n pone en la rueda derecha la velocidad que se le indica en rad/s
{
	PWM2_SetDuty((float)rad_por_s*800/mecanica->vel_max);
}

void velocidad_izquierda (double rad_por_s,param_mecanicos *mecanica)	//Esta funci�n pone en la rueda izquierda la velocidad que se le indica en rad/s
{
	PWM3_SetDuty((float)rad_por_s*800/mecanica->vel_max);
}

void avanza_recto(cinematica *variable,param_mecanicos *mecanica)				//FUNCION GORDACA DE IR PA' LANTE
{
	double t_acel_vel_cte;										//variable que almacena el tiempo que la referencia del motor se mantiene en alto.
	
	calculo_d_acel(variable,mecanica);
	calculo_t_acel(variable,mecanica);
	//calculo_d_frenada(variable,mecanica);
	calculo_t_vel_cte(variable);
	
	t_acel_vel_cte = variable->tiempo_acel + variable->tiempo_vel_cte;			//tiempo que acelera y mantiene v cte
	
	LPC_TIM1->MR1=((int)FPCLK*t_acel_vel_cte-1);											//Modificar MR1 para que el timer interrumpa pasado ese tiempo
	LPC_TIM1->TCR|=(1<<0);																						//Activamos el timer
	
	velocidad_derecha(variable->velocidad_final,mecanica);
	velocidad_izquierda(variable->velocidad_final,mecanica);
	
}
