/**** Inclusión de bibliotecas ****/

#include <LPC17xx.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Eurouart.h"
#include "GLCD.h"

/**** Caracterización del robot ****/

typedef struct {

	int X;																				// Coordenada X del robot en mm
	int Y;																				// Coordenada Y del robot en mm

} Posicion;

typedef struct {
	
	Posicion Pos;																	// Coordenadas actuales del robot
	int Orientacion;															// Ángulo con respecto a la orientación del campo
	int VelActual;																// Velocidad actual del robot en mm/s
	
} Caracterizacion;

Caracterizacion Robot;

/**** Caracterización de los mensajes ****/

char Instruccion_Codigo = ST_INICIAL;
char Instruccion_Prioridad = NORMAL;

/**** Variables de la comunicación por UART (by Lesmus Trompiz) ****/

char buffer[70];  // Buffer de recepci�n de 70 caracteres
char *ptr_rx;     // puntero de recepci�n
char rx_completa; // Flag de recepci�n de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;     // puntero de transmisi�n
char tx_completa; // Flag de transmisi�n de cadena que se activa al transmitir el caracter null (fin de cadena)

//char estado = 'NULL';
int distancia = 0;
int velocidad = 0;
int velocidad_maxima = 0;
int radio = 0;
int grados_giro = 0;
//int posx = -1500;
//int posy = 500;


/**** CONDICIONES INICIALES ****/

#define 	X_I						-1500
#define 	Y_I						500
#define 	X_D						1500
#define 	Y_D						500

/**** Estados de la máquina de estados ****/

#define 	ST_INICIAL			0
#define		ST_PARADO				1
#define		ST_RECTA				2
#define		ST_GIRO					3
#define		ST_CURVA				4
#define		ST_FRENO				5

/**** Prioridades de la comunicación ****/

#define 	NORMAL 					0					//No asignar a ninguna instrucción que nos haga movernos
#define		URGENTE					1					//Freno

/**** Prioridades de la comunicación ****/

#define 	IZQUIERDA					0
#define		DERECHA						1
#define 	LADO						1>>17			//Para leer el 0.17 como bit de lado del campo

/**** Funciones de la máquina de estados ****/

int Flag_EstadoFinalizado = 1;                           //La inicializo a 1 para que actualice Estado desde ST_INICIAL
int Siguiente_Estado = ST_INICIAL; 	
int Estado = ST_INICIAL;

int CMD_Inicial(void){
//En el bucle general se llama a esta función que inicializa la estructura Robot y espera instrucciones para arrancar hacia otro estado

//strcpy(buffer, "D100");			//Prueba del correcto funcionamiento de la máquina de estados

static bool inicio = 1;
	if(inicio)
	{
		inicio=0
		//Flag_EstadoFinalizado=0			//En ST_INICIAL no hace falta ya que está deseoso de órdenes
		
		Robot.VelActual = 0;
		Robot.Orientacion = Seleccion_Lado();
		
		if(Robot.Orientacion == -1)
		{
			transmitir_cadenaUART0('Error orientacion\n');						//Cazamos errores de lectura de lado
		}
		else if (Robot.Orientacion == IZQUIERDA)
		{
			Robot.Pos.X = X_I;
			Robot.Pos.Y = Y_I;
		}
		else if (Robot.Orientacion == DERECHA)
		{
			Robot.Pos.X = X_D;
			Robot.Pos.Y = Y_D;
		}
		transmitir_cadenaUART0(ST_INICIAL);
	}

	//ESTE ESTADO NO HACE NADA APARTE DE INICIALIZAR

	//Un timer por debajo se encarga de transmitir la estructura a los de arriba cada 500 ms
	//cuando terminamos un comando tb se transmite la estructura del robot, esta vez de manera asíncrona
}

int CMD_Parado(void){
//Función de seguridad en el que llevamos todas las velocidades a 0 y miramos por la siguiente instrucción
//Seguramente estamos aquí por un mensaje de FRENO

static bool inicio = 1;
	if(inicio)
	{
		inicio=0
		Flag_EstadoFinalizado=0					//En ST_PARADO SÍ hay que hacerlo inicialmente ya que solemos llegar por ENEMIGO
		Motores(0); 								//Repite la instrucción de parar por si ha llegado aquí por mensaje aleatorio, no mediante una frenada anterior
		transmitir_cadenaUART0(ST_PARADO);
	}

	//Analiza todo lo que necesite mientras esta en reposo

	if(sensor.laser)								//hay moros en la costa
	{	
		//analizo que cara es la que detecta
		//informo arriba de que hay algo ahí
		transmitir_sensor();
	}
	else
	{
		Flag_EstadoFinalizado = 1;
	}
	
}

int CMD_Recta(void){
//Avanzamos una distancia en línea recta a velocidad máxima definida



//ESTRUCTURA GENERAL DE LOS COMANDOS

/*
static bool inicio = 1;

if(inicio){
    inicio = 0;
    Flag_EstadoFinalizado = 0;
	INFORMO A LESMUS QUE ESTOY EN ESTE ESTADO 
    Cargo datos leídos por UART
    Pongo el contador de odometría a 0
    Habilito los motores y a movernos
}


Evalúo el error que tengo en cada ciclo hasta que "llego"
    Flag_EstadoFinalizado = 1;                            //LEVANTO FLAG LLEGADA PARA CARGAR EL SIGUIENTE ESTADO 
    inicio = 1;

*/
}

int CMD_Giro(void){
//Giramos sobre el eje del robot un ángulo definido a velocidad máxima definida
}

int CMD_Curva(void){
//Tomamos una curva de radio determinado y angulo de arco determinado a velocidad máxima definida
}

int CMD_Freno(void){
//FRENA POR TU VIDA

static bool inicio = 1;

	if (inicio)
	{
		inicio = 0;
		Flag_EstadoFinalizado = 0;
		Motores(0); 					//Paramos en cuanto recibimos el comando
		transmitir_cadenaUART0(ST_FRENO);
		Instruccion_Codigo = ST_PARADO;		//Al no estar entre las opciones, se irá al default que es ST_PARADO
											//Se hace aquí para que no esté frenando siempre y ya sepa que después viene un PARADO, aunque lo cargará cuando suban el flag_final
	}

	//Dependiendo del tiempo de frenada, para asegurarnos que hemos frenado habría que esperar más que dicho tiempo
	timer_freno(Robot.VelActual)
	//Al pasar ese tiempo de seguridad
	if(flag_timer_freno)
	{
		inicio = 1;
		Flag_EstadoFinalizado = 1;
		transmitir_odometria();				//Transmitimos la distancia que nos hemos movido. Esta funcion debería ser bloqueante
											//En este estado incluye la que se movió con el estado anterior, ya que no hubo flag_final al ser URGENTE
		reset_odometria();
		
	}
}
	
	

int Maquina_Estados(void){
	
		// Selección del próximo estado
		switch(Instruccion_Codigo){
			case ST_INICIAL:
				Siguiente_Estado = ST_INICIAL;
				break;
			case 'D':
				Siguiente_Estado = ST_RECTA;				
				break;
			case 'C':
				Siguiente_Estado = ST_CURVA;				
				break;
			case 'G':
				Siguiente_Estado = ST_GIRO;			
				break;
			case 'F':
				Siguiente_Estado = ST_FRENO;				
				break;
			default:										
				Siguiente_Estado = ST_PARADO;			
				break;
		}
		
		// Si el estado ha finalizado o hay un mensaje de prioridad urgente -> cambio de estado
			if(Instruccion_Prioridad || Flag_EstadoFinalizado){
				Estado = Siguiente_Estado;
			}
		
		Traduccion_Variables();
}


/**** Funciones auxiliares ****/

int Seleccion_Lado(void) {
	/*
	Evalúa en que lado empieza el partido y asigna un ángulo de inicio
	En la primera iteración el robot empieza perpendicular a la pared
	*/
	if( ( ( (LPC_GPIO0->FIOPIN) LADO) & 0x1) == IZQUIERDA)
	{
		Robot.Orientacion = 0;
	}		
	else if ( ( ( (LPC_GPIO0->FIOPIN) LADO) & 0x1) == DERECHA)
	{
		Robot.Orientacion = 180;
	}
	else
	{
		Robot.Orientacion = -1;
	}

return Robot.Orientacion;
}

/**** Funci�n main ****/

int main(){

//Configuramos TODO lo configurable

		while(1){
			
			// Ejecutamos la máquina de estados
			Maquina_Estados();
			
			// Super bucle para cambiar de estado
			switch(Estado){	
			
					/* En el estado inicial, esperamos la llegada de la primera instrucción	 
					basado en la estrategia. */
					case ST_INICIAL:
																											// nuevo_estado = estado_inicial(Estado)
																											//Hago una primera lectura de todos los sensores
																											//Espero por la UART la primera instrucción
							CMD_Inicial();
							break;
					
					/* Estado PARADO: nos quedamos a la espera de la siguiente instrucci�n.
					Se puede llegar aqu� por: ausencia de tareas, enemigo cercano. */
					case ST_PARADO:
																											//PAREN LAS ROTATIVAS
																											//Hago lo mismo que en INICIAL 
							CMD_Parado();    
											
							break;
					
					/* Estado RECTA: instrucci�n de movimiento en l�nea recta. Se evalua:
					la vel. actual, la vel. m�xima, la vel. final y la distancia */
					case ST_RECTA:
							CMD_Recta();
																											//SOLO EN EL PRIMER CICLO DEL ESTADO
																													//Cargamos los valores de distancia y velocidades
																													//Comenzamos la medida de odometría
																													//Habilitamos motores
																											//TODOS LOS CICLOS
																													//Controlamos lo que falta para llegar
							break;
					
					/* Estado GIRO: instrucci�n de giro sobre el propio eje. Se evalua:
					la vel. actual, la vel. final y �ngulo de giro */
					case ST_GIRO:
							CMD_Giro();
							break;
					
					/* Estado CURVA: instrucci�n de movimiento tomando una curva. Se evalua:
					�ngulo, radio de curvatura, vel. actual y vel. final */
					case ST_CURVA:
							CMD_Curva();
							break;
					
					/* Estado FRENO: instrucci�n de frenada de emergencia. No se evalua nada.
					Caso particular de RECTA */
					case ST_FRENO:
							CMD_Freno();
							break;
					
					default:
																											//PAREN LAS ROTATIVAS
																											//Hago lo mismo que en INICIAL 
							CMD_Parado();
							Estado = ST_PARADO;
							break;
            }
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
