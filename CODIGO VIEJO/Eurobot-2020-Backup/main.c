/**** Inclusi�n de bibliotecas ****/

#include <LPC17xx.h>
#include <stdio.h>
#include <math.h>
#include "Eurouart.h"
#include "GLCD.h"

/**** Caracterizaci�n del robot ****/

typedef struct {

	int X;																				// Coordenada X del robot en mm
	int Y;																				// Coordenada Y del robot en mm

} Posicion;

typedef struct {
	
	Posicion Pos_Inicial;
	Posicion Pos_Final;
	int Vel_Actual;																// Velocidad actual del robot en mm/s
	
} Robot;

/**** Caracterizaci�n de los mensajes ****/

typedef struct {

	char Codigo;
	char Prioridad;

} Mensaje;

/**** Variables de la comunicaci�n por UART (by Lesmus Trompiz) ****/

char buffer[70];  // Buffer de recepci�n de 50 caracteres
char *ptr_rx;     // puntero de recepci�n
char rx_completa; // Flag de recepci�n de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;     // puntero de transmisi�n
char tx_completa; // Flag de transmisi�n de cadena que se activa al transmitir el caracter null (fin de cadena)

char estado = 'NULL';
int distancia = 0;
int velocidad = 0;
int velocidad_maxima = 0;
int radio = 0;
int grados_giro = 0;
int posx = -1500;
int posy = 500;

/**** Comandos del robot ****/

#define		CMD_RECTA				0
#define		CMD_GIRO				1
#define		CMD_CURVA				2

/**** Estados de la m�quina de estados ****/

#define 	ST_INICIAL			0
#define		ST_PARADO				1
#define		ST_RECTA				2
#define		ST_GIRO					3
#define		ST_CURVA				4
#define		ST_FRENO				5
int Estado = 0;

/**** Funciones de la m�quina de estados ****/

int ST_Inicial(void){

}

int ST_Parado(void){

}

int ST_Recta(void){

}

int ST_Giro(void){

}

int ST_Curva(void){

}

int ST_Freno(void){

}

void Maquina_Estados (*Estado){
	
	int Siguiente_Estado;

	Siguiente_Estado = ST_Inicial();
	while(1) {
		switch(Siguiente_Estado) {
			case ST_INICIAL:
				Siguiente_Estado = ST_Inicial();
				break;
			default:
				break;
		}
	}

}

/**** Funci�n main ****/

int main(){

		while(1){
			
			Maquina_Estados(Estado);
			Estado = Siguiente_Estado;
		
		switch(Estado){
			
			/* En el estado inicial, esperamos la llegada de la primera instrucci�n	 
			basado en la estrategia. */
			case ST_INICIAL:
				// nuevo_estado = estado_inicial(Estado)
				break;
			
			/* Estado PARADO: nos quedamos a la espera de la siguiente instrucci�n.
			Se puede llegar aqu� por: ausencia de tareas, enemigo cercano. */
			case ST_PARADO:
				break;
			
			/* Estado RECTA: instrucci�n de movimiento en l�nea recta. Se evalua:
			la vel. actual, la vel. m�xima, la vel. final y la distancia */
			case ST_RECTA:
				break;
			
			/* Estado GIRO: instrucci�n de giro sobre el propio eje. Se evalua:
			la vel. actual, la vel. final y �ngulo de giro */
			case ST_GIRO:
				break;
			
			/* Estado CURVA: instrucci�n de movimiento tomando una curva. Se evalua:
			�ngulo, radio de curvatura, vel. actual y vel. final */
			case ST_CURVA:
				break;
			
			/* Estado FRENO: instrucci�n de frenada de emergencia. No se evalua nada.
			Caso particular de RECTA */
			case ST_FRENO:
				break;
			
			default:
				break;
				
		}
		// estado = nuevo_estado
	}
}

/*
int estado_inicial(*Estado) {
 while(1){
	 siguiente_estado = ST_INIT
	 // mensaje = leer_comando_uart(args)
	 switch(mensaje.codigo) {
	   ENCENDER_LED:
		 break;
		 APAGAR_LED:
		 break;
		 AVANZAR:
		 // Estado.adonde_ir_x = mensaje.x
		 // Estado.adonde_ir_y = mensaje.x
		 siguiente_estado = ST_AVANZAR
		 break;
	 }
	 return siguiente_estado
 }
*/
