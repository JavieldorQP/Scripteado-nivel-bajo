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
#include "servo.h"
#include "uart.h"
#include "./GLCD/GLCD.h"
#include <stdio.h>
//Definiciones utiles:

//Parametros posicionales:
#define MAXCAMPO_X 1500
#define MAXCAMPO_Y 1000
#define MINCAMPO_X -1500
#define MINCAMPO_Y -1000

//Parametros demo:
#define MAXPANTALLA_X 310
#define MAXPANTALLA_Y 230
#define MINPANTALLA_X 10
#define MINPANTALLA_Y 10
#define R_CP 10
#define MAX_GRADOS 180
#define MIN_GRADOS 0

//Variables globales:
//Variables propias del protocolo:
char buffer[70];  // Buffer de recepción de 50 caracteres
char *ptr_rx;     // puntero de recepción
char rx_completa; // Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;     // puntero de transmisión
char tx_completa; // Flag de transmisión de cadena que se activa al transmitir el caracter null (fin de cadena)

//Variables de la demo que se esperan recibir
uint32_t grados = 0; // Variable que indica los grados
int Objetivo_x = 0;
int Objetivo_y = 0;
int posx = 0;
int posy = 0;
int odomx = 0;
int odomy = 0;
uint8_t flag_grados = 0;
uint8_t flag_objetivo = 0;
uint8_t flag_velocidad = 0;
int error_longitud = 0;

//Declaración de funciones:
void traduccion_de_variables() //Se encarga de leer el mensaje recibido, actualizar las variables y levantar los flags.
{
    //Declaración de variables
    static int grados_ant;
    static int Objetivo_x_ant = 0;
    static int Objetivo_y_ant = 0;
    int8_t signo = 0;
    uint8_t flag_posicion = 0;
    uint8_t flag_errosigno = 1;

    //Calculo de los grados:
    grados = BIT_CGrados * 100 + BIT_DGrados * 10 + BIT_UGrados;
    if (grados_ant != grados)
    {
        if ((grados > MAX_GRADOS) || (grados < MIN_GRADOS))
        {
            transmitir_cadenaUART0("EG");
            grados = grados_ant;
        }
        else
        {
            flag_grados = 1;
            grados_ant = grados;
        }
    }

    //Calculo de la posición X objetivo:
    if (BIT_S_OX == '+') //Si recibo un mas es positivo
        signo = 1;
    else if (BIT_S_OX == '-') //Si recibo un menos es negativo
        signo = -1;
    else
    {
        flag_errosigno = 0;
        transmitir_cadenaUART0("ESOX");
    }
    Objetivo_x = signo * (BIT_M_OX * 1000 + BIT_C_OX * 100 + BIT_D_OX * 10 + BIT_U_OX);
    if (Objetivo_x_ant != Objetivo_x)
    {
        if ((Objetivo_x > MAXCAMPO_X) || (Objetivo_x < MINCAMPO_X))
        {
            transmitir_cadenaUART0("EPOX");
            Objetivo_x = Objetivo_x_ant;
        }
        else
        {
            flag_posicion = 1;
            Objetivo_x_ant = Objetivo_x;
        }
    }

    //Calculo de la posición Y objetivo:
    if (BIT_S_OY == '+') //Si recibo un mas es positivo
        signo = 1;
    else if (BIT_S_OY == '-') //Si recibo un menos es negativo
        signo = -1;
    else
    {
        flag_errosigno = 0;
        transmitir_cadenaUART0("ESOY");
    }
    Objetivo_y = signo * (BIT_M_OY * 1000 + BIT_C_OY * 100 + BIT_D_OY * 10 + BIT_U_OY);
    if (Objetivo_y_ant != Objetivo_y)
    {
        if ((Objetivo_y > MAXCAMPO_Y) || (Objetivo_y < MINCAMPO_Y))
        {
            transmitir_cadenaUART0("EPOY");
            Objetivo_y = Objetivo_y_ant;
        }
        else
        {
            flag_posicion = 1;
            Objetivo_y_ant = Objetivo_y;
        }
    }
    //Activamos el flag
    if (flag_posicion & flag_errosigno)
        flag_objetivo = 1;
}

void dibujar_cuadrado(CX, CY) //Dibuja un cuadrado por pantalla para la demo:
{
    int i, j;
    for (i = CX - 10; i <= CX + 10; i++)
    {
        for (j = CY - 10; j <= CY + 10; j++)
        {
            LCD_SetPoint(i, j, 40);
        }
    }
}

int main() //Función principal con maquina de estados sencillita para la demo
{
    char mensaje_enviado[30];
    int j = 0;
    servo_config();
    set_servo(0);
    uart0_init(115200);
    LCD_Initializtion();
    LCD_Clear(White);
    transmitir_cadenaUART0("START");
    dibujar_cuadrado(155, 230);
    while (1)
    {
        if (rx_completa)
        {
            rx_completa = 0;
            if (error_longitud)
            {
                error_longitud = 0;
                transmitir_cadenaUART0("EL");
            }
            else
                traduccion_de_variables();
        }
        if (flag_grados)
        {
            set_servo(grados);
            flag_grados = 0;
        }
        if (flag_objetivo)
        {
            flag_objetivo = 0;
            LCD_Clear(White);
            dibujar_cuadrado((Objetivo_x - MINCAMPO_X) / 10, (Objetivo_y - MINCAMPO_Y) / R_CP);
        }
        if (tx_completa & (j == 1000000))
        {
            //Nota a la hora de enviar mensajes se puede hacer una estructura
            sprintf(mensaje_enviado, "%d%d", odomx, odomy);
            transmitir_cadenaUART0(mensaje_enviado);
            j = 0;
        }
        j++;
    }
}
