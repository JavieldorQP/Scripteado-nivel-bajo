#ifndef _TECLADO_NUM
#define _TECLADO_NUM
#include "lpc17xx.h"

#define columna_1			LPC_GPIO1->FIOPIN   				//VALOR PUERTO DE LA COLUMNA 1
#define columna_2 			LPC_GPIO1->FIOPIN 					//VALOR PUERTO DE LA COLUMNA 2
#define columna_3 			LPC_GPIO1->FIOPIN					//VALOR PUERTO DE LA COLUMNA 3

#define fila_1				LPC_GPIO1->FIOPIN					//VALOR PUERTO DE LA FILA 1
#define fila_2				LPC_GPIO1->FIOPIN					//VALOR PUERTO DE LA FILA 1
#define fila_3				LPC_GPIO1->FIOPIN					//VALOR PUERTO DE LA FILA 1
#define fila_4				LPC_GPIO1->FIOPIN					//VALOR PUERTO DE LA FILA 1

#define columna_1_ON 		(LPC_GPIO1->FIOPIN | (1<<19))			//ENCENDER PIN DE LA COLUMNA 1
#define columna_2_ON 		(LPC_GPIO1->FIOPIN | (1<<21))			//ENCENDER PIN DE LA COLUMNA 2
#define columna_3_ON 		(LPC_GPIO1->FIOPIN | (1<<23))			//ENCENDER PIN DE LA COLUMNA 3

#define columna_1_OFF 		(LPC_GPIO1->FIOPIN & !(1<<19)) 			//APAGAR PIN DE LA COLUMNA 1
#define columna_2_OFF 		(LPC_GPIO1->FIOPIN & !(1<<21))			//APAGAR PIN DE LA COLUMNA 2
#define columna_3_OFF 		(LPC_GPIO1->FIOPIN & !(1<<23))			//APAGAR PIN DE LA COLUMNA 3

#define fila_1_ON			(LPC_GPIO1->FIOPIN | (1<<18))			//PIN DE LA FILA 1 ENCENDIDO
#define fila_2_ON			(LPC_GPIO1->FIOPIN | (1<<20))			//PIN DE LA FILA 2 ENCENDIDO
#define fila_3_ON			(LPC_GPIO1->FIOPIN | (1<<22))			//PIN DE LA FILA 3 ENCENDIDO
#define fila_4_ON			(LPC_GPIO1->FIOPIN | (1<<24))			//PIN DE LA FILA 4 ENCENDIDO

#define fila_1_OFF			(LPC_GPIO1->FIOPIN & !(1<<18))			//APAGAR PIN DE LA FILA 1
#define fila_2_OFF			(LPC_GPIO1->FIOPIN & !(1<<20))			//APAGAR PIN DE LA FILA 2
#define fila_3_OFF			(LPC_GPIO1->FIOPIN & !(1<<22))			//APAGAR PIN DE LA FILA 3
#define fila_4_OFF			(LPC_GPIO1->FIOPIN & !(1<<24))			//APAGAR PIN DE LA FILA 4

char leer_teclado(void);
#endif