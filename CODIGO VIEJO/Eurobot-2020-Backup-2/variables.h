#ifndef _VARIABLES
#define _VARIABLES

#include "lpc17xx.h"
#include "stdbool.h"

extern bool vel_fin_cambiar;
extern bool continua;


 
/**** Caracterización del robot ****/

typedef struct {

	int X;																				// Coordenada X del robot en mm
	int Y;																				// Coordenada Y del robot en mm

}Posicion;

typedef struct {
	
	Posicion Pos;																	// Coordenadas actuales del robot
	int Orientacion;															// Ángulo con respecto a la orientación del campo
	int VelActual;																// Velocidad actual del robot en mm/s
	
}Caracterizacion;


typedef struct{
	int timer_derecho,timer_izquierdo;
}timer_counters;


typedef struct											//Estructura para los parametros de la cinematica
{
	double 	distancia_total_rad,			//distancia total a recorrer que se nos pasa desde alto nivel convertida a radianes
					distancia_frenada,  			//distancia recorrida durante el tiempo de deceleración convertida a radianes
					distancia_acel_vel_cte,		//distancia que debe recorrer cuando acelera y va a velocidad constante
	
					error_pos_act_derecha,		//error de posición actual de la rueda derecha
					error_pos_act_izquierda,	//error de posicion actual de la rueda izquierda
	
					velocidad_inicial,				//velocidad inicial en rad/s
					velocidad_final;					//velocidad final en rad/s
	
}cinematica;

typedef struct								//Estructura para los parametros mecanicos (motor + controladora)
{
	double aceleracion,					//Aceleración de la controladora de 3000 rpm/s
				 deceleracion,				//Valor de deceleracion de los motores
				 reductora,						//Reductora que tienen los motores maxon
				 vel_eje_max,					//Velocidad máxima del eje del motor segun datasheet
				 vel_max,							//Velocidad máxima de la rueda
				 pulsos_por_rev,			//Pulsos por revolucion del encoder
				 diametro;						//Diametro de la rueda
	
}param_mecanicos;

typedef struct								//Estructura para las instrucciones que le puedan llegar por el puerto UART
{
	char orden;									//Orden de la instruccion que le llega (A: avanzar ; G: girar)
	bool sentido_giro;					//Sentido de giro (1: Horario ; 0: Antihorario)
	
}instruccion;

typedef struct
{
	
	uint8_t num_estado;
	
}maquina_estados;

#endif			
