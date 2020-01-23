#include "pantalla.h" //Enlazo el .c con la pantavo_config(void)
#include <stdio.h>
#include <stdlib.h>
void LCD_config(int estado, float temperatura_adc, float temperatura_i2c, float distancia_medida, float frecuencia, float grados_servo)
{
	char Mensaje[8] = {0};
	LCD_Initializtion();
	switch (estado)
	{
	case (1):
		sprintf(Mensaje, "Estado: Proyecto incializando");
		GUI_Text(5, ARRIBA_Y, (uint8_t *)Mensaje, Cyan, Black);
		break;

	case (2):
		sprintf(Mensaje, "Estado: offline                ");
		GUI_Text(5, ARRIBA_Y, (uint8_t *)Mensaje, Cyan, Black);
		break;
	}

	//LCD_Initializtion();
	//Distancia en y sube si le restas
	LCD_mostrar_variable(temperatura_i2c, 'g');
	//LCD_mostrar_variable(temperatura_adc,MEDIO_Y-30,'g');
	LCD_mostrar_variable(temperatura_adc, 'g');
	LCD_mostrar_variable(distancia_medida, 'd');
	LCD_mostrar_variable(frecuencia, 'f');
	LCD_mostrar_variable(grados_servo, 's');
	//			printf(Mensaje2, "Temperatura 1 %d",temperatura_adc);
	//			GUI_Text(30,MEDIO_Y,(uint8_t *)Mensaje2,Cyan,Black);
	//			LCD_Initializtion();
	//			printf(Mensaje3, "Temperatura 2 %d",temperatura_i2c);
	//			GUI_Text(30,MEDIO_Y-15,(uint8_t *)Mensaje3,Cyan,Black);

	//LCD_mostrar_variable(8);
}

void LCD_mostrar_variable(float numero, char tipo)
{
	char Mensaje2[8] = {0};
	switch (tipo)
	{
	case ('g'):
		sprintf(Mensaje2, "Temperatura %.1f grados", numero);
		GUI_Text(30, YADC, (uint8_t *)Mensaje2, Cyan, Black);
		break;
	case ('d'):
		sprintf(Mensaje2, "Distancia %.1f cm", numero);
		GUI_Text(30, YDISTANCIA, (uint8_t *)Mensaje2, Cyan, Black);
		break;
	case ('f'):
		sprintf(Mensaje2, "Frecuencia %.1f Hz", numero);
		GUI_Text(30, YFRECUENCIA, (uint8_t *)Mensaje2, Cyan, Black);
		break;
	case ('s'):
		sprintf(Mensaje2, "SERVO: %.1f grados", numero);
		GUI_Text(30, YSERVO, (uint8_t *)Mensaje2, Cyan, Black);
		break;
	case ('q'):
		switch ((int)numero)
		{
		case (0):
			sprintf(Mensaje2, "Proyecto incializando");
			GUI_Text(5, YQ, (uint8_t *)Mensaje2, Cyan, Black);
			sprintf(Mensaje2, "Key 1 online Key 2 off");
			GUI_Text(5, MEDIO_Y, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		case (1):
			sprintf(Mensaje2, "Modo offline:         ");
			GUI_Text(5, YQ, (uint8_t *)Mensaje2, Cyan, Black);
			sprintf(Mensaje2, "Key 1 auto Key 2 manual");
			GUI_Text(5, MEDIO_Y, (uint8_t *)Mensaje2, Cyan, Black);
			break;

		case (2):
			sprintf(Mensaje2, "Modo online:          ");
			GUI_Text(5, YQ, (uint8_t *)Mensaje2, Cyan, Black);
			sprintf(Mensaje2, "Lea el terminal        ");
			GUI_Text(5, MEDIO_Y, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		}
		break;
	case ('o'):
		switch ((int)numero)
		{
		case (1):
			sprintf(Mensaje2, "Barrido automatico    "); //Cuidado al escribir muchos caracteres
			GUI_Text(5, YO, (uint8_t *)Mensaje2, Cyan, Black);
			break;

		case (2):
			sprintf(Mensaje2, "Barrido manual        ");
			GUI_Text(5, YO, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		case (3):
			sprintf(Mensaje2, "Deteccion de obstaculos");
			GUI_Text(5, YO, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		case (0):
			sprintf(Mensaje2, "Seleccion de operacion ");
			GUI_Text(5, YO, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		}
		break;
	case ('u'):
		switch ((int)numero)
		{
		case (1):
			sprintf(Mensaje2, "Mensaje recibido");
			GUI_Text(5, YUART, (uint8_t *)Mensaje2, Cyan, Black);
			break;

		case (2):
			sprintf(Mensaje2, "Estado: offline               ");
			GUI_Text(5, YUART, (uint8_t *)Mensaje2, Cyan, Black);
			break;
		}
		break;
	}
}
