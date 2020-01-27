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

	char Codigo;                //Letra de la instrucción
	char Prioridad;             //Prioridad 1 ==> URGENTE
                                //Prioridad 0 ==> Normal
} Mensaje;

/**** Variables de la comunicaci�n por UART (by Lesmus Trompiz) ****/

char buffer[70];  // Buffer de recepci�n de 70 caracteres
char *ptr_rx;     // puntero de recepci�n
char rx_completa; // Flag de recepci�n de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;     // puntero de transmisi�n
char tx_completa; // Flag de transmisi�n de cadena que se activa al transmitir el caracter null (fin de cadena)
Mensaje instru;   //Hay que guardar aquí las instrucciones recibidas por UART

//char estado = 'NULL';
int distancia = 0;
int velocidad = 0;
int velocidad_maxima = 0;
int radio = 0;
int grados_giro = 0;
int posx = -1500;
int posy = 500;

int Siguiente_Estado = ST_INICIAL; 	
int Estado = ST_INICIAL;				

int flag_fin = 1;                           //La inicializo a 1 para que actualice Estado desde ST_INICIAL

/**** Comandos del robot ****/
/*
#define		CMD_RECTA				0
#define		CMD_GIRO				1
#define		CMD_CURVA				2
*/
/**** Estados de la m�quina de estados ****/

#define 	ST_INICIAL				0
#define		ST_PARADO				1
#define		ST_RECTA				2
#define		ST_GIRO					3
#define		ST_CURVA				4
#define		ST_FRENO				5



/**** Funciones de la m�quina de estados ****/

int CMD_Inicial(void){
//En el bucle general se llama a esta función que solo lee mensajes de la UART para arrancar hacia otro estado
}

int CMD_Parado(void){
//Función de seguridad en el que llevamos todas las velocidades a 0 y miramos por la siguiente instrucción
//Seguramente estamos aquí por un mensaje de ABORTAR o EMERGENCIA
}

int CMD_Recta(void){
//Avanzamos una distancia en línea recta a velocidad máxima definida



//ESTRUCTURA GENERAL DE LOS COMANDOS

/*
static bool inicio = 1;

if(inicio){
    inicio = 0;
    flag_fin = 0;
    Cargo datos leídos por UART
    Pongo el contador de odometría a 0
    Habilito los motores y a movernos
}

Leo UART cada 250 ms
si no ha cambiado, la instrucción entonces Siguiente_Estado=Estado
Evalúo el error que tengo en cada ciclo hasta que "llego"
    flag_fin = 1;                            //LEVANTO FLAG LLEGADA PARA CARGAR EL SIGUIENTE ESTADO 
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

}

int Maquina_Estados (*Estado){
	//Siguiente_Estado = ST_Inicial(); 						
	while(1) {
		switch(instru.Codigo) {
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
	}

}

/**** Funci�n main ****/

int main(){

//Configuramos TODO lo configurable

		while(1){
															//Revisamos las nuevas instrucciones para actualizar el estado, de ser así lo actualizamos
			Maquina_Estados(Estado);
			//Estado = Siguiente_Estado;
            
            if(instru.Prioridad | flag fin){
                Estado = Siguiente_Estado;                  //Actualiza si no hay instrucción en curso o hay URGENCIA
            }

            switch(Estado){	
            
                /* En el estado inicial, esperamos la llegada de la primera instrucci�n	 
                basado en la estrategia. */
                case ST_INICIAL:
                                                            // nuevo_estado = estado_inicial(Estado)
                                                            //Hago una primera lectura de todos los sensores
                                                            //Espero por la UART la primera instrucción
                    CMD_Inicial();
                    traduccion_de_variables();                      //Leo la UART para saber si hay siguiente instrucción

                    break;
                
                /* Estado PARADO: nos quedamos a la espera de la siguiente instrucci�n.
                Se puede llegar aqu� por: ausencia de tareas, enemigo cercano. */
                case ST_PARADO:
                                                            //PAREN LAS ROTATIVAS
                                                            //Hago lo mismo que en INICIAL 
                    CMD_Parado();    
                    traduccion_de_variables();                      //Leo la UART para saber si hay siguiente instrucción
                            
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
                    Estado=ST_PARADO;
                    traduccion_de_variables();                      //Leo la UART para saber si hay siguiente instrucción

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
