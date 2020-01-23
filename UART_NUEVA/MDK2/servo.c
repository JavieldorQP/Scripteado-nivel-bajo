#include <LPC17xx.H>
#include "servo.h"
// Aqui se muestran todas las funciones que tienen relaci�n con el servo




    void servo_config(void) //Configuración de los registros para un correct
    {	
		LPC_PINCON->PINSEL3|=(2<<8);    //Configura la funci�n del pin a PWM-P1.20
		LPC_SC->PCONP|=(1<<6);			//Transmite energia a la PWM. Es inecesario puesto que el valor de reset es 1.
		LPC_PWM1->MR0=Fpclk*Tpwm-1;	    //Configura el registro de Match 1.
		LPC_PWM1->PCR|=(1<<10); 		//Configurado el ENA2 (1.2), es decir la salida de la PWM1.2 es activada. Y es de un solo filo.
		LPC_PWM1->MCR|=(1<<1);			//Lo configuras para que cuando el time counter llegue al MR0, resetee el time counter.
		LPC_PWM1->TCR|=(1<<0)|(1<<3);   //Activa el time counter de la pwm y el preescaler. Ademas activa el modo pwm, importante este
		//registro tiene que ser siempre configurado despues del MRn.
	}
	
	void set_servo(float grados) //Movmiento del servo
	{
        LPC_PWM1->MR2=(Fpclk*0.4e-3 + Fpclk*2e-3*grados/180); // TH		//ESTO SE HA CAMBIADO POR ENESIMA VEZ
	    LPC_PWM1->LER|=(1<<2);	//Despues del reinicio del time counter se restablecen al valor actual tanto el valor del MR0 como del MR2.
	}
   