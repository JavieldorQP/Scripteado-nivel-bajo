// librerias
#include "lpc17xx.h"
#include "math.h"
#include "uart.h"
#include "funciones_pwm.h"
#include "timer.h"
#include "comunicacion.h"
 #define PI 								3.1416
 #define diametro						12.2 									//cm
 #define pulsos							256
 #define reduct							26
 #define p_a_cm							diametro*PI/(pulsos*reduct)			//Numero de cm recorrido por pulso  
 #define LONG_EJE 					34										//Longitud del eje en cm
 #define error_max_avance   1											//Error máximo en el avance en cm
 #define error_max_angulo		0.1										//Error máximo en el ángulo en grados
 #define KG          				1.2 	 	  					  //constante proporcional del giro
 #define KA									1.5									  //constante proporcional del avance
 #define CG									1											//constante de corrección del giro
 #define CA									1											//constante de corrección del avance
 #define OG									0											//offset de corrección del giro
 #define OA									0											//offset de corrección del avance
 #define baudrate 					9600
// variables globales

	
pos pos_actual, pos_final;
	
float ang_encare, avance_total;
char f_uart;
int flag_ok;
int timer, DIR_I, DIR_D;    						//flag del timer para realizar el control y de sentidos de los motores
typedef struct{
	int timer_derecho,timer_izquierdo;
}timer_counters;
timer_counters TC;
	void TIMER1_IRQHandler(){
			LPC_TIM1->IR|=(1<<1);
		timer=1;
	}
	

	
// funciones

void act_odom() 
{
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
	deltaX						=		avance*cosf(pos_actual.omega*PI/180);   			//calculo del avance en X del robot  (funcion cos en radianes)
	deltaY						=		avance*sinf(pos_actual.omega*PI/180);				//calculo del avance en Y del robot  (funcion sin en radianes)
	pos_actual.pos_x	+=	deltaX;
	pos_actual.pos_y	+=	deltaY;						//incrementar las variables globales con los datos calculados
	pos_actual.omega	+=	deltaO;

	while (pos_actual.omega>=360)							// si se pasa de 360º convierte a rango 0
		pos_actual.omega-=360;								// no se contempla que de mas de una vuelta en un tiempo de muestreo
	while (pos_actual.omega<0)									// si se pasa de 360º convierte a rango 0
		pos_actual.omega+=360;	

}



// main
int main(){
char estado, f_g1, f_a, f_g2;
	float e_avance,e_angulo,avance_act;
	ptr_rx=buffer;
	estado=0;      														//inicializar la maquina de estados y los flags
	f_uart=0;
	f_g1=0;
	f_a=0;
	f_g2=0;
	flag_ok=1;
	
	init_pwm();																//inicializar las pwm de las ruedas
	uart0_init(baudrate);											//inicializar la UART 0
	config_Timers ();													//inicializar los Timers CAP y el de Tiempo de muestreo
	
/*	
	PWM2_SetDuty (0.4);												//solo pruebas
	PWM3_SetDuty (0.4);
*/	
	tx_cadena_UART0("Introduce los comandos \n");
	
while(1){	
	
	if ((estado==0) &&  (f_uart==1)){
		if (rx_completa){
			funcion_comunicacion();
			rx_completa=0;
			if (flag_ok==1){
				ang_encare=(float)((180/PI)*CG*atan((double)((pos_final.pos_y-pos_actual.pos_y)/(pos_final.pos_x-pos_actual.pos_x)))+OG);
				if(pos_final.pos_x-pos_actual.pos_x<0) {
					ang_encare+=180;
				}
				avance_total=CA*sqrt((pos_final.pos_y-pos_actual.pos_y)*(pos_final.pos_y-pos_actual.pos_y)
												+(pos_final.pos_x-pos_actual.pos_x)*(pos_final.pos_x-pos_actual.pos_x))+OA;
				estado=1; f_uart=0;
				LPC_GPIO2->FIOSET=3<<11;
			} else {
				f_uart=0;
			}
			flag_ok=1;
		}	
	}
	
	else if ((estado==1) &&  (f_g1==1)){
		estado=2; f_g1=0;}											//MÁQUINA DE ESTADOS:
																						// estado=  0-> REPOSO		1-> PRIMER GIRO		2-> AVANCE		3-> SEGUNDO GIRO   
	else if ((estado==2) &&  (f_a==1)){
		estado=3; f_g2=0;}
	
	else if ((estado==3) &&  (f_g2==1)){
		
		estado=0;}
	
		
	if (timer==1){
timer=0;
		
		
		if (estado==2)
		avance_act+=(LPC_TIM2->TC*p_a_cm+LPC_TIM3->TC*p_a_cm)/2;				//
		else
		avance_act=0;
		
		
		act_odom ();
		switch (estado){
			case 1:
			{	
				e_angulo=ang_encare-pos_actual.omega;				//calculo del error de angulo con rectificación para coger el mejor giro
				while (e_angulo>180) e_angulo-=360;					//con estas dos sentencias se elige el sentido de giro de tal forma que
				while (e_angulo<-180) e_angulo+=360;				//transforma giros de mas de 180º en negativos y viceversa

				if (fabs(e_angulo)<error_max_angulo)        //comprobar si esta dentro del error permitido y en caso afirmativo activar el flag de fin de instrucción
				{
					f_g1=1;
					PWM2_SetDuty(0);
					PWM3_SetDuty(0);
				}
				else {
					if(fabs(e_angulo*KG)<400){
						PWM2_SetDuty(-1*(e_angulo*KG));						//actualizar la velocidad de las ruedas y sus flags y crear la limitacion de velocidad	
						PWM3_SetDuty((e_angulo*KG));
						if (e_angulo>0){
							DIR_D=1; DIR_I=-1;
							LPC_GPIO3->FIOCLR|=(3<<25);
							LPC_GPIO3->FIOPIN3&=~(1<<1);
							LPC_GPIO3->FIOPIN3|=(1<<2);
						}
						else{
							DIR_D=-1; DIR_I=1;
							LPC_GPIO3->FIOSET|=(3<<25);
							LPC_GPIO3->FIOPIN3|=(1<<1);
							LPC_GPIO3->FIOPIN3&=~(1<<2);						
						}
					}
					else if(e_angulo*KG>400)
					{
						PWM2_SetDuty(-400);
						DIR_D=1;
						PWM3_SetDuty(400);
						DIR_I=-1;
						LPC_GPIO3->FIOCLR|=(3<<25);
						LPC_GPIO3->FIOPIN3&=~(1<<1);
						LPC_GPIO3->FIOPIN3|=(1<<2);
					}
					else
					{
						PWM2_SetDuty(400);
						DIR_D=-1;
						PWM3_SetDuty(-400);
						DIR_I=1;
						LPC_GPIO3->FIOSET|=(3<<25);
						LPC_GPIO3->FIOPIN3|=(1<<1);
						LPC_GPIO3->FIOPIN3&=~(1<<2);	
					}
					
				}
				break;
			}
			case 2:
			{
				e_avance=avance_total-avance_act;				//calculo del error de angulo con rectificación para coger el mejor giro
			
				
				if (fabs(e_avance)<error_max_avance)        //comprobar si esta dentro del error permitido y en caso afirmativo activar el flag de fin de instrucción
				{
					f_a=1;
					PWM2_SetDuty(0);
					PWM3_SetDuty(0);
				}
				else {
					if(fabs(e_avance*KA)<400){
						PWM2_SetDuty((int)(e_avance*KA));						//actualizar la velocidad de las ruedas y crear la limitacion de velocidad
						PWM3_SetDuty((int)(e_avance*KA));
						DIR_D=1;
						DIR_I=1;
						LPC_GPIO3->FIOCLR|=(3<<25);
						LPC_GPIO3->FIOPIN3&=~(1<<1);
						LPC_GPIO3->FIOPIN3&=~(1<<2);
					}
					else
					{
						PWM2_SetDuty(400);
						PWM3_SetDuty(400);
						DIR_D=1;
						DIR_I=1;
						LPC_GPIO3->FIOCLR|=(3<<25);
						LPC_GPIO3->FIOPIN3&=~(1<<1);
						LPC_GPIO3->FIOPIN3&=~(1<<2);
					}
				}
				break;
			}
			case 3:
			{	
				e_angulo=pos_final.omega-pos_actual.omega;				//calculo del error de angulo con rectificación para coger el mejor giro
				if (e_angulo>180)
					e_angulo-=360;
				

				if (fabs(e_angulo)<error_max_angulo)        //comprobar si esta dentro del error permitido y en caso afirmativo activar el flag de fin de instrucción
				{	
					f_g2=1;
					PWM2_SetDuty(0);
					PWM3_SetDuty(0);
				}
				else {
					if(fabs(e_angulo*KG)<400){
						PWM2_SetDuty(-1*(e_angulo*KG));						//actualizar la velocidad de las ruedas y sus flags y crear la limitacion de velocidad	
						PWM3_SetDuty((e_angulo*KG));
						if (e_angulo>0){
							DIR_D=1; DIR_I=-1;
							LPC_GPIO3->FIOCLR|=(3<<25);
							LPC_GPIO3->FIOPIN3&=~(1<<1);
							LPC_GPIO3->FIOPIN3|=(1<<2);
						}
						else{
							DIR_D=-1; DIR_I=1;
							LPC_GPIO3->FIOSET|=(3<<25);
							LPC_GPIO3->FIOPIN3|=(1<<1);
							LPC_GPIO3->FIOPIN3&=~(1<<2);						
						}
					}
					else if(e_angulo*KG>400)
					{
						PWM2_SetDuty(-400);
						DIR_D=1;
						PWM3_SetDuty(400);
						DIR_I=-1;
						LPC_GPIO3->FIOCLR|=(3<<25);
						LPC_GPIO3->FIOPIN3&=~(1<<1);
						LPC_GPIO3->FIOPIN3|=(1<<2);
					}
					else
					{
						PWM2_SetDuty(400);
						DIR_D=-1;
						PWM3_SetDuty(-400);
						DIR_I=1;
						LPC_GPIO3->FIOSET|=(3<<25);
						LPC_GPIO3->FIOPIN3|=(1<<1);
						LPC_GPIO3->FIOPIN3&=~(1<<2);	
					}
					
				}
				LPC_GPIO2->FIOCLR=3<<11;
			break;
		}
	
	 }
	}
}
}
