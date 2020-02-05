//Segunda demo

//Incluimos bibliotecas:
#include <LPC17xx.H>
#include "Eurouart.h"
#include "./GLCD/GLCD.h"
#include <stdio.h>
//Definiciones utiles:

//Parametros demo:
#define MAXPANTALLA_X 310
#define MAXPANTALLA_Y 230
#define MINPANTALLA_X 10
#define MINPANTALLA_Y 10
#define R_CP 10
//Variables globales:
//Variables propias del protocolo:
char buffer[70];  // Buffer de recepción de 50 caracteres
char *ptr_rx;     // puntero de recepción
char rx_completa; // Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;     // puntero de transmisión
char tx_completa; // Flag de transmisión de cadena que se activa al transmitir el caracter null (fin de cadena)

//Variables de la demo que se esperan recibir
char Instruccion_Codigo = 'NULL';
int distancia = 0;
int velocidad_final= 0;
int velocidad_maxima = 0;
int radio = 0;
int grados_giro = 0;
int posx = -1500;
int posy = 500;
int Instruccion_Prioridad = 0;

//Declaración de funciones:

int main() //Función principal con maquina de Instruccion_Codigos sencillita para la demo
{
    char mensaje_enviado[30];
    int i = 0;
    uart0_init(115200);
    //LCD_Initializtion();
    LCD_Clear(White);
    transmitir_cadenaUART0("S");

    while (1)
    {
        if (rx_completa)
        {
            rx_completa = 0;
            Traduccion_Variables();
        }
        else
        {
            switch (Instruccion_Codigo)
            {
            case 'G':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Grados %d", grados_giro);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    Instruccion_Codigo = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'D':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Distancia %d", distancia);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Velocidad %d", velocidad_final);
                GUI_Text(130, 80, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "V_max %d", velocidad_maxima);
                GUI_Text(130, 60, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    Instruccion_Codigo = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'C':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Distancia %d", distancia);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Velocidad %d", velocidad_final);
                GUI_Text(130, 80, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "V_max %d", velocidad_maxima);
                GUI_Text(130, 60, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Radop %d", radio);
                GUI_Text(130, 40, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    Instruccion_Codigo = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'A':
                transmitir_cadenaUART0("A");
                Instruccion_Codigo = 'NULL';
                break;
            }
        }
    }
}
