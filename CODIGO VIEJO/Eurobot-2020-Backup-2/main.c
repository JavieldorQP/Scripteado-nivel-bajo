/**** Inclusión de bibliotecas ****/

#include <LPC17xx.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Eurouart.h"
#include "funciones_pwm.h"
#include "funciones_timer.h"
#include "funciones_control.h"
#include "maquina_estados.h"
#include "configuraciones.h"
#include "variables.h"
#include "GLCD.h"

#define FPCLK 25e6

/**** Variables de la comunicación por UART (by Lesmus Trompiz) ****/

#define BAUDRATE 115200
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

char aux[100]={0};	//Buffer de transmisión de 100 caracteres
/**** CINEMÁTICA ****/

cinematica lazo_abierto;
param_mecanicos maxon;

//cinematica variable; 									// 31/01/2020: La declaro para eliminar errores

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

/**** Caracterización del robot ****/

Caracterizacion Robot;

/**** Caracterización de los mensajes ****/

char Instruccion_Codigo = ST_INICIAL;
char Instruccion_Prioridad = NORMAL;

/****  Variables maquina estados  ****/

int Flag_EstadoFinalizado = 1;                           //La inicializo a 1 para que actualice Estado desde ST_INICIAL
int Siguiente_Estado = ST_INICIAL; 	
int Estado = ST_INICIAL;

/****  Variables odometria  ****/
#define 	AVANZA					1
#define 	RETROCEDE				0

#define 	LONG_EJE 				34										//Longitud del eje en cm
#define 	DIAMETRO				12.2 									//Diametro en cm
#define 	pulsos					256
#define 	reduct					26
#define 	p_a_cm					DIAMETRO*PI/(pulsos*reduct)				//Numero de cm recorrido por pulso  

/**** Funciones odometría ****/

void act_odom(int DIR_I, int DIR_D) 
{
	timer_counters TC;
	float deltaO;
	float avance;
	float deltaX;							//definición de avance en recto del robot, incremento del ángulo, incremento en X e incremento en Y 
	float deltaY;	

	TC.timer_derecho=LPC_TIM2->TC;
	TC.timer_izquierdo=LPC_TIM3->TC;
	
	avance=(DIR_D*TC.timer_derecho*p_a_cm	+	DIR_I*TC.timer_izquierdo*p_a_cm)	/	2;          					//cálculo del avance en recto del robot
	deltaO=((DIR_D*TC.timer_derecho*p_a_cm	-	DIR_I*TC.timer_izquierdo*p_a_cm)	/	LONG_EJE)*180/PI;		//cálculo del incremento del ángulo en grados
	
	LPC_TIM2->TC = 0; // Manually Reset Timer 0 (forced)
	LPC_TIM3->TC = 0; // Manually Reset Timer 0 (forced)
	deltaX						=		avance*cosf(Robot.Orientacion*PI/180);   			//calculo del avance en X del robot  (funcion cos en radianes)
	deltaY						=		avance*sinf(Robot.Orientacion*PI/180);				//calculo del avance en Y del robot  (funcion sin en radianes)
	Robot.Pos.X	+=	deltaX;
	Robot.Pos.Y	+=	deltaY;						//incrementar las variables globales con los datos calculados
	Robot.Orientacion	+=	deltaO;

	while (Robot.Orientacion >= 360)							// si se pasa de 360º convierte a rango 0
		Robot.Orientacion -= 360;								// no se contempla que de mas de una vuelta en un tiempo de muestreo
	while (Robot.Orientacion <0)									// si se pasa de 360º convierte a rango 0
		Robot.Orientacion += 360;	

}

void reset_odometria(void){
	LPC_TIM2->TC=0;
	LPC_TIM3->TC=0;
}

void transmitir_estado(void){

sprintf(aux,"%d\n",Estado),
transmitir_cadenaUART0(aux);
	
}
/**** Funciones de la máquina de estados ****/

//ESTRUCTURA GENERAL DE LOS COMANDOS

/*
char Inicio = 1;

if(Inicio){
    Inicio = 0;
    Flag_EstadoFinalizado = 0;
	INFORMO A LESMUS QUE ESTOY EN ESTE ESTADO 
    Cargo datos leídos por UART
    Pongo el contador de odometría a 0
    Habilito los motores y a movernos
}


Evalúo el error que tengo en cada ciclo hasta que "llego"
    Flag_EstadoFinalizado = 1;                            //LEVANTO FLAG LLEGADA PARA CARGAR EL SIGUIENTE ESTADO 
    Inicio = 1;

*/

int CMD_Inicial(void){
//En el bucle general se llama a esta función que inicializa la estructura Robot y espera instrucciones para arrancar hacia otro estado

//strcpy(buffer, "D100");			//Prueba del correcto funcionamiento de la máquina de estados

static char Inicio = 1;
	if(Inicio)
	{
		Inicio=0;
		//Flag_EstadoFinalizado=0			//En ST_INICIAL no hace falta ya que está deseoso de órdenes
		
		Robot.VelActual = 0;
		Robot.Orientacion = 0;
		Robot.Pos.X = 0;
		Robot.Pos.Y = 0;
		transmitir_estado();		//TRANSMITIR A LESMUS UNA S
	}

	//ESTE ESTADO NO HACE NADA APARTE DE INICIALIZAR

	//Un timer por debajo se encarga de transmitir la estructura a los de arriba cada 500 ms
	//cuando terminamos un comando tb se transmite la estructura del robot, esta vez de manera asíncrona
	
	return 0;
}

int CMD_Parado(void){
//Función de seguridad en el que llevamos todas las velocidades a 0 y miramos por la siguiente instrucción
//Seguramente estamos aquí por un mensaje de FRENO

static char Inicio = 1;
	if(Inicio)
	{
		Inicio=0;
		Flag_EstadoFinalizado=0;				//En ST_PARADO SÍ hay que hacerlo inicialmente ya que solemos llegar por ENEMIGO
		velocidad_derecha(0,&maxon);
		velocidad_izquierda(0,&maxon); 								//Repite la instrucción de parar por si ha llegado aquí por mensaje aleatorio, no mediante una frenada anterior
		transmitir_estado();
	}

	//Analiza todo lo que necesite mientras esta en reposo
/*
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
	*/
	
	
	if(Siguiente_Estado != Estado) 
		Flag_EstadoFinalizado = 1;
	return 0;
}

int CMD_Recta(void){
//Avanzamos una distancia en línea recta a velocidad máxima definida

static char Inicio = 1;

if(Inicio){
	Inicio = 0;
	Flag_EstadoFinalizado = 0;
	reset_odometria();
	transmitir_estado();
	lazo_abierto.distancia_total_rad = (distancia/2)/maxon.diametro;
	lazo_abierto.velocidad_final = velocidad_maxima;
	lazo_abierto.velocidad_inicial = 0;			//Cuando sea capaz de empalmar comandos sería Robot.VelActual

	avanza_recto(&lazo_abierto,&maxon);
	
}

	//Evalúo el error que tengo en cada ciclo hasta que "llego" con el TIMER1
	

	if(lazo_abierto.error_pos_act_derecha < 0.1 || lazo_abierto.error_pos_act_izquierda < 0.1){
		//Actualmente esto frena al final, no encadena velocidades aún
		//Modificar funciones_desplazamiento
				velocidad_derecha(lazo_abierto.velocidad_inicial,&maxon);
				velocidad_izquierda(lazo_abierto.velocidad_inicial,&maxon);
		
				Flag_EstadoFinalizado = 1;                            //LEVANTO FLAG LLEGADA PARA CARGAR EL SIGUIENTE ESTADO 
   	 		Inicio = 1;
			}
	return 0;
}

int CMD_Giro(void){
//Giramos sobre el eje del robot un ángulo definido a velocidad máxima definida


/*
if(grados_giro>0)
act_odom(RETROCEDE,AVANZA);
*/
	
	
return 0;
}

int CMD_Curva(void){
//Tomamos una curva de radio determinado y angulo de arco determinado a velocidad máxima definida

	
	
	return 0;
}

int CMD_Freno(void){
//FRENA POR TU VIDA

static int Inicio = 1;

	if (Inicio)
	{
		Inicio = 0;
		Flag_EstadoFinalizado = 0;
		
		lazo_abierto.error_pos_act_derecha = 1;											//Le asigno un valor mayor que la comprobacion del if del main
		lazo_abierto.error_pos_act_izquierda = 1;										//Le asigno un valor mayor que la comprobacion del if del main
		calculo_d_frenada(&lazo_abierto,&maxon);
		lazo_abierto.distancia_acel_vel_cte = lazo_abierto.distancia_frenada;
		reset_odometria();
		velocidad_derecha(0,&maxon);
		velocidad_izquierda(0,&maxon); 					//Paramos en cuanto recibimos el comando
		
		transmitir_estado();
		
	}

	
	//Evalúo el error que tengo en cada ciclo hasta que "llego" con el TIMER1
	
	
	if(lazo_abierto.error_pos_act_derecha < 0.1 || lazo_abierto.error_pos_act_izquierda < 0.1){
			Flag_EstadoFinalizado = 1;                            //LEVANTO FLAG LLEGADA PARA CARGAR EL SIGUIENTE ESTADO 
			Inicio = 1;
			Instruccion_Codigo = ST_PARADO;		//Al no estar entre las opciones, se irá al default que es ST_PARADO
		
	//transmitir_odometria();				//Transmitimos la distancia que nos hemos movido. Esta funcion debería ser bloqueante
										//En este estado incluye la que se movió con el estado anterior, ya que no hubo flag_final al ser URGENTE
	//reset_odometria();
		}
	
	
	return 0;
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
			//Limpia el mensaje para que en caso de no recibir nada se pare
			Instruccion_Codigo = ST_PARADO;
			Instruccion_Prioridad = 0;
		}
		
		if(rx_completa)
		{
		Traduccion_Variables();
		rx_completa=0;
		}
		return 0;
}



/**** Funci�n main ****/

int main(){

//Configuramos TODO lo configurable

init_pwm();
config_TIMER1();
configuracion_parametros_mecanicos(&maxon,&lazo_abierto);
uart0_init(BAUDRATE);

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
