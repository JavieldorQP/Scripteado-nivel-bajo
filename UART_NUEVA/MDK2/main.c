/*
    En esta demo se implementan las funciones necesarias para la comuncación desde la 
    MDK2 con la Raspberry. A falta de concretar la cantidad de mensajes que se necesitan 
    a nivel bajo y a nivel alto en esta demo se trabaja con los siguientes datos: Posición
    del objetivo, ángulo al que tiene que estar un pequeño actuador (servo) y la odometria del robot 
    que es enviada desde la MDK2. Ademas se enviara desde la MDK2 un mensaje a la Raspberry indicando 
    los siguientes errors:
        EG--------->Error en el mensaje de grados, se ha superado el máximo/el minimo
        ESOX------->Error en el signo de la posición X del objetivo 
        EPOX------->Error en la posción X, excede el máximo del campo
        ESOY------->Error en el signo de la posición Y del objetivo 
        EPOY------->Error en la posción Y, excede el máximo del campo
        EL--------->Erro mensaje más largo de lo esperado  

    Si se carga este programa en la Raspberry y su homologo en la MDK2, seremos capaces de observar
    como un rectángulo pasa por la pantalla de la MDK2, un servo gira y leemos el valor 00 por la 
    pantalla simulando la odometria. 
    
*/

//Incluimos bibliotecas:
#include <LPC17xx.H>
#include "uart.h"
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
char estado = 'NULL';
int distancia = 0;
int velocidad = 0;
int velocidad_maxima = 0;
int radio = 0;
int grados_giro = 0;
int posx = -1500;
int posy = 500;
uint8_t flag_grados = 0;
uint8_t flag_objetivo = 0;
uint8_t flag_velocidad = 0;
int error_longitud = 0;

//Declaración de funciones:
void traduccion_de_variables() //Se encarga de leer el mensaje recibido, actualizar las variables y levantar los flags.
{
    switch (T_INSTRUCCION)
    {
    case ('G'):
        estado = T_INSTRUCCION;
        grados_giro = (buffer[1] - '0') * 100 + (buffer[2] - '0') * 10 + (buffer[3] - '0');
        break;
    case ('D'):
        estado = T_INSTRUCCION;
        distancia = (buffer[1] - '0') * 1000 + (buffer[2] - '0') * 100 + (buffer[3] - '0') * 10 + (buffer[4] - '0');
        velocidad = (buffer[5] - '0') * 1000 + (buffer[6] - '0') * 100 + (buffer[7] - '0') * 10 + (buffer[8] - '0');
        velocidad_maxima = (buffer[9] - '0') * 1000 + (buffer[10] - '0') * 100 + (buffer[11] - '0') * 10 + (buffer[12] - '0');
        break;
    case ('C'):
        estado = T_INSTRUCCION;
        distancia = (buffer[1] - '0') * 1000 + (buffer[2] - '0') * 100 + (buffer[3] - '0') * 10 + (buffer[4] - '0');
        velocidad = (buffer[5] - '0') * 1000 + (buffer[6] - '0') * 100 + (buffer[7] - '0') * 10 + (buffer[8] - '0');
        velocidad_maxima = (buffer[9] - '0') * 1000 + (buffer[10] - '0') * 100 + (buffer[7] - '0') * 10 + (buffer[11] - '0');
        radio = (buffer[12] - '0') * 1000 + (buffer[13] - '0') * 100 + (buffer[14] - '0') * 10 + (buffer[15] - '0');
        break;
    }
}

int main() //Función principal con maquina de estados sencillita para la demo
{
    char mensaje_enviado[30];
    int i = 0;
    uart0_init(115200);
    LCD_Initializtion();
    LCD_Clear(White);
    transmitir_cadenaUART0("S");

    while (1)
    {
        if (rx_completa)
        {
            rx_completa = 0;
            traduccion_de_variables();
        }
        else
        {
            switch (estado)
            {
            case 'G':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Grados %d", grados_giro);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    estado = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'D':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Distancia %d", distancia);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Velocidad %d", velocidad);
                GUI_Text(130, 80, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "V_max %d", velocidad_maxima);
                GUI_Text(130, 60, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    estado = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'C':
                LCD_Clear(White);
                sprintf(mensaje_enviado, "Distancia %d", distancia);
                GUI_Text(130, 100, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Velocidad %d", velocidad);
                GUI_Text(130, 80, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "V_max %d", velocidad_maxima);
                GUI_Text(130, 60, (uint8_t *)mensaje_enviado, Cyan, Black);
                sprintf(mensaje_enviado, "Radop %d", radio);
                GUI_Text(130, 40, (uint8_t *)mensaje_enviado, Cyan, Black);
                if (i == 100)
                {
                    estado = 'A';
                    i = 0;
                }
                else
                    i++;
                break;
            case 'A':
                transmitir_cadenaUART0("A");
                estado = 'NULL';
                break;
            }
        }
    }
}
