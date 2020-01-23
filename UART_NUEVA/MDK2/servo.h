#include <LPC17xx.H>

// Aqui se muestran todas las funciones que tienen relaci�n con el servo

//Definici�n constantes:
#define Fpclk 25e6 // Fcpu/4 (defecto despu�s del reset)
#define Tpwm 25e-3 // Periodo

//Funciones:
//Configuracio�n inicial

void servo_config(void);
void set_servo(float grados);
