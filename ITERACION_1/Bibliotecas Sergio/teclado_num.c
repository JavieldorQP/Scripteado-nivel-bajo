/************************************************************************************************************************
Esta libreria contiene funciones de lectura de teclado matricial numerico del tipo de la figura.
El modelo exacto es XYHDZ1688.

	Para usarla debes elegir 7 pines GPIO de la LPC a los que conectar el teclado y definirlos 								1	2	3
	en el .h los tres de las columnas como entrada, los 4 de las filas como salida y todos sin resistencias		4	5	6
	Despues completa los defines con el registro y el numero del pin si hace falta y a correr con							7	8	9
	tu teclado :D																																															*	0	#
	
PD: No pulses dos teclas a la vez que no lo coje bien. Si lo haces coge la de mas arriba a la izquierda.
************************************************************************************************************************/




#include "teclado_num.h"


char leer_teclado(void) //esta funcion lee el teclado en los pines especificados arriba y devuelve el char que corresponde
{
	fila_1=fila_1_ON; //enciende la primera fila
	if (columna_1==columna_1_ON)
		{
			fila_1=fila_1_OFF;
			return('1');
		}
	else if (columna_2==columna_2_ON)
		{
			fila_1=fila_1_OFF;
			return('2');
		}
	else if (columna_3==columna_3_ON)
		{
			fila_1=fila_1_OFF;
			return('3');
		}
	else
		{
			fila_1=fila_1_OFF;
			fila_2=fila_2_ON;
			if (columna_1==columna_1_ON)
				{
					fila_2=fila_2_OFF;
					return('4');
				}
			else if (columna_2==columna_2_ON)
				{
					fila_2=fila_2_OFF;
					return('5');
				}
			else if (columna_3==columna_3_ON)
				{
					fila_2=fila_2_OFF;
					return('6');
				}
			else
				{
					fila_2=fila_2_OFF;
					fila_3=fila_3_ON;
					if (columna_1==columna_1_ON)
						{
							fila_3=fila_3_OFF;
							return('7');
						}
					else if (columna_2==columna_2_ON)
						{
							fila_3=fila_3_OFF;
							return('8');
						}
					else if (columna_3==columna_3_ON)
						{
							fila_3=fila_3_OFF;
							return('9');
						}
					else
						{
							fila_3=fila_3_OFF;
							fila_4=fila_4_ON;
							if (columna_1==columna_1_ON)
								{
									fila_4=fila_4_OFF;
									return('*');
								}
							else if (columna_2==columna_2_ON)
								{
									fila_4=fila_4_OFF;
									return('0');
								}
							else if (columna_3==columna_3_ON)
								{
									fila_4=fila_4_OFF;
									return('#');
								}
							else
							{
								fila_4=fila_4_OFF;
								return(0);
							}
							
						}
						

				}
		
		
		}
	
	
	
		
}
