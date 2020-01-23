#include "lpc17xx.h" 

#define Ts 0.005                                 		//Tiempo de muestreo

void config_Timers(void){
	
		LPC_SC->PCONP|=(1<<22)|(1<<23)|(1<<2); 					//Activo los TIMER1, TIMER2 y TIMER3
	
		LPC_PINCON->PINSEL0|=(3<<8); 										//Pongo el P0.4 como CAP2.0 (Motor DERECHO)
		LPC_PINCON->PINSEL1|=(3<<14); 									//Pongo el P0.23 como CAP3.0 (Motor IZQUIERDO)
	
		LPC_TIM1->MR1=Ts*25e6-1;												//Valor del registro MR1 para que interrumpa cada Ts
	
		
		LPC_TIM2->CTCR|=(1<<0); 												//Pongo TIMER2 en modo "counter" y utilizo el CAP2.0 (Aumenta cada flanco subida)
		LPC_TIM3->CTCR|=(1<<0); 												//Pongo TIMER3 en modo "counter" y utilizo el CAP3.0 (Aumenta cada flanco de subida)
	
		LPC_TIM2->CCR&=~(7<<0); 												//Me aseguro que el CCR esta a "000"
		LPC_TIM3->CCR&=~(7<<0); 												//Me aseguro que el CCR esta a "000"
		
		LPC_TIM1->TCR=1;																//El TIMER1 siempre esta contando
		LPC_TIM2->TCR|=(1<<0); 													//Habilito el TC del TIMER2
		LPC_TIM3->TCR|=(1<<0); 													//Habilito el TC del TIMER3
		LPC_TIM2->TC&=(0);															//Reseteo el TC del TIMER2
		LPC_TIM3->TC&=(0);															//Reseteo el TC del TIMER3
		LPC_TIM1->MCR|=(3<<3);													//Interrumpe y Reseta TC cuando se produce un Match (Valor de MR1)
		NVIC_EnableIRQ(TIMER1_IRQn);										//Habilito la interrupción del TIMER1
		
}
