#include "lpc17xx.h"
#include "math.h"
//defines PWM
#define CLK 25e6
#define FPWM 1000     // 25000 ciclos de reloj



void init_pwm(void)
{
	/*LPC_PINCON->PINSEL3|=(10<<8);    // Activamos los pines p1.20 y p1.21 como PWM 1.2 (DERECHA) y PWM 1.3 (IZQUIERDA) A DIGITALES 1
	LPC_SC->PCONP|=(1<<6);			// Ponemos un 1 en el bit 6 de Pconp para activar los canales pwm, van de 1.0-1.6
	LPC_PWM1->MCR = (1<<1);						//no interrumpe y resetea en MR0
	LPC_PWM1->CTCR|=(0<<0)|(0<<1);
	LPC_PWM1->MR0=(CLK/FPWM)-1;        //Ajuste de MR0 para la f deseada
	LPC_PWM1->LER|=(1<<0);							// Al modificar MR0 tenemos que validarlo con el registro LER
	LPC_PWM1->PCR|=(3<<10);						//Habilita la PWM a traves de los ENABLE
	LPC_PWM1->TCR|=(1<<1);								//RESET COUNTER
	LPC_PWM1->TCR=(1<<3)|(1<<0);					//COUNTER ENABLE, PWM ENABLE
	LPC_GPIO2->FIODIR1|=(3<<3);																	// 2.11 enable izdo //////PIN DIGITAL 2 en controladora
	LPC_GPIO2->FIOSET1|=(3<<3);																	// 2.12 enable dcho
	LPC_GPIO3->FIODIR3|=(3<<1);																	//3.25	sentido dcho
	*/
LPC_PINCON->PINSEL3 |=(2<<8) | (2<<10);		// Ponemos pines 1.20(dcha) y 1.21(izq) como PWM1.2 y PWM1.3		
	LPC_SC->PCONP|=(1<<6);			// Ponemos un 1 en el bit 6 de Pconp para activar los canales pwm, van de 1.0-1.6
	LPC_PWM1->MCR = (1<<1);								//NO INTERRUMPE Y SE RESETEA EN PWMR0
	LPC_PWM1->CTCR|=(0<<0)|(0<<1);	    	//PWM FUNCIONA COMO TIMER
	LPC_PWM1->MR0=(CLK/FPWM)-1;        //Ajuste de MR0 para la f deseada							

	LPC_PWM1->LER|=(1<<0) | (1<<2) | (1<<3);
	LPC_PWM1->PCR&=(0<<2)|(0<<3);					//PWM CONTROLADAS POR UN UNICO FLANCO 
	LPC_PWM1->PCR|=(1<<10)|(1<<11);				//HABILITAMOS LOS CANALES DE SALIDA DE los pwm1.2 y pwm1.3
	LPC_PWM1->TCR|=(1<<1);								//RESET COUNTER
	LPC_PWM1->TCR=(1<<3)|(1<<0);					//COUNTER ENABLE, PWM ENABLE 		//3.26	sentido izda

	LPC_GPIO2->FIODIR1|=(3<<3);																	// 2.11 enable izdo //////PIN DIGITAL 2 en controladora
	LPC_GPIO2->FIOPIN1&=~(3<<3);																	// 2.12 enable dcho
	LPC_GPIO3->FIODIR3|=(3<<1);																	//pines de direccion como salida
}

void v_derecha (int V)
{
	if (0<V<=400){
		LPC_PWM1->MR2=2500+25*V-1;
	LPC_PWM1->LER|=(1<<2);
	LPC_GPIO3->FIOCLR3|=(1<<1);}
	else if (V==0){
		LPC_PWM1->MR2=2500;
	LPC_PWM1->LER|=(1<<2);
	LPC_GPIO3->FIOSET3|=(1<<1);}
	else if (-400<=V<0){
		LPC_PWM1->MR2=2500+25*(-V)-1;
	LPC_PWM1->LER|=(1<<2);
	LPC_GPIO3->FIOSET3|=(1<<1);}
		
}


void v_izquierda (int V)
{
	if (0<V<=400){
		LPC_PWM1->MR3=2500+25*V-1;
	LPC_PWM1->LER|=(1<<3);
	LPC_GPIO3->FIOSET3|=(1<<2);}
	else if (V==0){
		LPC_PWM1->MR3=2500;
	LPC_PWM1->LER|=(1<<3);
	LPC_GPIO3->FIOCLR3|=(1<<2);}
	else if (-400<=V<0){
		LPC_PWM1->MR3=2500+25*(-V)-1;
	LPC_PWM1->LER|=(1<<3);
	LPC_GPIO3->FIOCLR3|=(1<<2);}
		
}
//CAMBIAR EL CICLO DE TRABAJO PWM_1.2 P1.20 -> MOTOR DERECHA
void PWM2_SetDuty (float duty1)
{
	float duty;
	duty=(float)fabs((double)duty1)/1000+0.1;
	LPC_PWM1->MR2 = (uint32_t)(duty*LPC_PWM1->MR0);
	LPC_PWM1->LER |= (1<<2);
	if (duty<0){
		LPC_GPIO3->FIOPIN3|=(1<<1);				//ponemos el bit de sentido a 1
		LPC_GPIO2->FIOSET1|=(1<<3);					//ponemos el enable a 1
	}
	else if (duty1==0)
		LPC_GPIO2->FIOCLR1|=(1<<3);					//ponemos el enable a 0
		
	else
		{
		LPC_GPIO3->FIOPIN3&=~(1<<1);					//ponemos el bit de sentido a 0
		LPC_GPIO2->FIOSET1|=(1<<3);					//ponemos el enable a 1
		}
	return;
}
//	PWM_1.3 P1.21 -> MOTOR IZQUIERDA
void PWM3_SetDuty (float duty1)
{	
	float duty;
	duty=(float)fabs((double)duty1)/1000+0.1;
	LPC_PWM1->MR3 = (uint32_t)(duty*LPC_PWM1->MR0);
	LPC_PWM1->LER |= (1<<3);
	if (duty<0){
		LPC_GPIO3->FIOPIN3&=~(1<<2);					//ponemos el bit de sentido a 1
		LPC_GPIO2->FIOSET1|=(1<<4);}				//ponemos el enable a 1

	else if (duty1==0)
		LPC_GPIO2->FIOCLR1|=(1<<4);					//ponemos el enable a 0
		
	else
		{
		LPC_GPIO3->FIOPIN3&=~(1<<2);					//ponemos el bit de sentido a 0
		LPC_GPIO2->FIOSET1|=(1<<4);					//ponemos el enable a 1
		}
	return;
}
