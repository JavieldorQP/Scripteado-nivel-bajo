#ifndef _VARIABLES
#define _VARIABLES

#include "lpc17xx.h"

extern uint8_t Flag_FinTimer;



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


typedef struct								//Estructura para los parametros de la cinematica
{
	double 	distancia_total,		//distancia total a recorrer que se nos pasa desde alto nivel convertida a radianes
					distancia_acel,			//distancia recorrida durante el tiempo de aceleraci�n convertida a radianes
					distancia_frenada,  //distancia recorrida durante el tiempo de deceleraci�n convertida a radianes
					distancia_vel_cte,	//distancia recorrida cuando la velocidad es constante convertida a radianes
	
					velocidad_inicial,	//velocidad inicial en rad/s
					velocidad_final,		//velocidad final en rad/s
	
					tiempo_total,				// tiempo total de viaje
					tiempo_acel,				// tiempo acelerando
					tiempo_frenada,		  // tiempo decelerando
					tiempo_vel_cte;			// tiempo a velocidad constante
}cinematica;

typedef struct								//Estructura para los parametros mecanicos (motor + controladora)
{
	double aceleracion,					//Aceleraci�n de la controladora de 3000 rpm/s
				 reductora,
				 diametro,						//Diametro de la rueda
				 vel_eje_max,					//Velocidad m�xima del eje del motor segun datasheet
				 vel_max;							//Velocidad m�xima de la rueda
				 
	
}param_mecanicos;


	


#endif
