#ifndef _VARIABLES
#define _VARIABLES

#include "lpc17xx.h"


#define 	PI 							3.141592

/****  Variables odometria  ****/
#define 	AVANZA					1
#define 	RETROCEDE				0


extern uint8_t fl_fin_timer;
extern int contador;
typedef struct								//Estructura para los parametros de la cinematica
{
	double 	distancia_total_rad,			//distancia total a recorrer que se nos pasa desde alto nivel convertida a radianes
					distancia_frenada,  			//distancia recorrida durante el tiempo de deceleración convertida a radianes
					distancia_acel_vel_cte,		//distancia que debe recorrer cuando acelera y va a velocidad constante/distancia recorrida cuando la velocidad es constante convertida a radianes
	
					error_posicion_actual_derecha,
					error_posicion_actual_izquierda,
	
					error_posicion_actual_derecha_total,
					error_posicion_actual_izquierda_total,
	
					distancia,
					angulo,
					ajustar_distancia,
	
					velocidad_inicial,	//velocidad inicial en rad/s
					velocidad_final;		//velocidad final en rad/s
	
	float		offset_lineal,
					offset_angular;
	
}cinematica;

typedef struct								//Estructura para los parametros mecanicos (motor + controladora)
{
	double aceleracion,					//Aceleraci�n de la controladora de 3000 rpm/s
				 deceleracion,				//Valor de deceleracion de los motores
				 reductora,
				 vel_eje_max,					//Velocidad m�xima del eje del motor segun datasheet
				 vel_max,							//Velocidad m�xima de la rueda
				 pulsos_por_rev,			//Pulsos por revolucion del encoder
				 L,
				 diametro;						//Diametro de la rueda
	
}param_mecanicos;

/**** Caracterizaci�n del robot ****/

typedef struct {

	int X;																				// Coordenada X del robot en mm
	int Y;																				// Coordenada Y del robot en mm

}Posicion;

typedef struct {
	
	Posicion Pos;																	// Coordenadas actuales del robot
	int Orientacion;															// �ngulo con respecto a la orientaci�n del campo
	int VelActual;																// Velocidad actual del robot en mm/s
	
}Caracterizacion;


#endif
