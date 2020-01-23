#include "lpc17xx.h"
#include "math.h"
//defines PWM
#define CLK 					25e6							//Frecuencia del reloj
#define FPWM 					1000     					//Frecuencia de la PWM




//FUNCIÓN init_pwm: Inicia y configura todo lo relacionado con las PWM1.2 (que se usa para la rueda derecha) y PWM1.3 (que
//se usa para la rueda izquierda) y los pines de habilitación y sentido de las controladoras MAXON.
//Para poder usar estas PWM hay que poner esta función a ser posible al principio del codigo main().

void init_pwm(void)  
{
	LPC_PINCON->PINSEL3 |=	(2<<8)			|		(2<<10);					//Se configuran lso pines 1.20 y 1.21 como PWM1.2 y PWM1.3 respectivamente.
	
	LPC_SC->PCONP				|=	(1<<6);														//Se activa la alimentación de las PWM mediante su pin del registro PCONP.
	
	LPC_PWM1->MCR 			= 	(1<<1);														//Se configura para que no se genere interrupción pero si se resetee en MR0.
	
	LPC_PWM1->CTCR			|=	(0<<0)			|		(0<<1);	    			//Se configura para que la PWM funcione con su timer.
	
	LPC_PWM1->MR0				=		(CLK/FPWM)	-		1;       					//Se ajusta MR0 para la f deseada.	
																														//El -1 es porque la cuenta comienza en 0.
	
	LPC_PWM1->LER				|=	(1<<0)			| 	(1<<2)	| (1<<3);	//Se cargan los valores puestos en los registros de MR0, MR2 y MR3 
																														//siendo estos dos últimos 0 por el reset.
	
	LPC_PWM1->PCR				&=	(0<<2)			|		(0<<3);						//Se configuran las PWM1.2 y PWM1.3 como controladas por un único flanco 
	
	LPC_PWM1->PCR				|=	(1<<10)			|		(1<<11);					//Se habilitan los canales de salida de los pines de pwm1.2 y pwm1.3
	
	LPC_PWM1->TCR				|=	(1<<1);														//Se resetea el valor del timer
	
	LPC_PWM1->TCR				=		(1<<3)			|		(1<<0);						//Se habilita el contador y la PWM.

	LPC_GPIO2->FIODIR1	|=	(3<<3);														//Se configuran los GPIOS 2.11 y 2.12 como salida para los enable de
																														//la controladora. el 2.11 es para la izquierda y el 2.12 para la derecha.
																														
	LPC_GPIO2->FIOCLR1	|=	(3<<3);														//Se ponen los pines a 0 como predeterminado.
	
	LPC_GPIO3->FIODIR3	|=	(3<<1);														//Se confguran los pines 3.25 y 3.26 (pines de sentido de las ruedas)
																														//como salidas. el 3.25 corresponde a la derecha y el 3.26 a la izquierda.
																														//Los valores de sentido en Sebastian son 0 para el avance de ambas ruedas.
}


//FUNCION PWM2_SetDuty: Esta función cambia la velocidad de la rueda derecha y acepta valores entre 800 y -800 (las controladoras
//MAXON aceptan un rango de pwm de 10% a 90% con una resolución de 0.1% lo que hace 800 valores posibles en cada sentido) 
void PWM2_SetDuty (float velocidad)
{
	float duty;																										//Se define la variable duty, que sirve para calcular de forma
																																//sencilla el valor del MR2
	
	duty=(float)fabs((double)velocidad)	/	1000	+	0.1;						//El duty se calcula como el 10% de mínimo mas el valor absoluto de
																																//la velocidad entre mil. La función fabs recibe y devuelve un double
																																//y es por eso por lo que necesitamos las conversiones de tipo.
	
	LPC_PWM1->MR2				 = 	(uint32_t)(duty*LPC_PWM1->MR0);				//Se calcula el MR2 como el duty por el valor de MR0 convertido a uint32_t
	
	LPC_PWM1->LER 			|= 	(1<<2);																//Se actualiza el valor de MR2 desde el registro LER
	
	if(velocidad>=0)
		LPC_GPIO3->FIOCLR	|=	(3<<25);															//Si la velocidad es positiva ponemos el bit de sentido derecho a 0 (avance)
	
	else
		LPC_GPIO3->FIOSET	|=	(3<<25);															//En caso contrario se pone a 1(retroceso)
	
	LPC_GPIO2->FIODIR1	|=	(2<<3);																//Se pone a 1 el bit de enable derecho(2.12).
	
	return;
}
//FUNCION PWM3_SetDuty: Esta función cambia la velocidad de la rueda izquierda y acepta valores entre 800 y -800 (las controladoras
//MAXON aceptan un rango de pwm de 10% a 90% con una resolución de 0.1% lo que hace 800 valores posibles en cada sentido) 
void PWM3_SetDuty (float velocidad)
{
	float duty;																										//Se define la variable duty, que sirve para calcular de forma
																																//sencilla el valor del MR3
	
	duty	=		(float)fabs((double)velocidad)	/	1000	+	0.1;			//El duty se calcula como el 10% de mínimo mas el valor absoluto de
																																//la velocidad entre mil. La función fabs recibe y devuelve un double
																																//y es por eso por lo que necesitamos las conversiones de tipo.
	
	LPC_PWM1->MR3	 			 = 	(uint32_t)(duty*LPC_PWM1->MR0);				//Se calcula el MR3 como el duty por el valor de MR0 convertido a uint32_t
	
	LPC_PWM1->LER 			|=	(1<<3);																//Se actualiza el valor de MR3 desde el registro LER
	
	if(velocidad>=0)
		LPC_GPIO3->FIOCLR	|=	(3<<26);															//Si la velocidad es positiva ponemos el bit de sentido izquierdo a 0 (avance)
	
	else
		LPC_GPIO3->FIOSET	|=	(3<<26);															//En caso contrario se pone a 1(retroceso)
	
	LPC_GPIO2->FIODIR1	|=	(1<<3);																//Se pone a 1 el bit de enable izquierdo(2.11).
	
	return;
}