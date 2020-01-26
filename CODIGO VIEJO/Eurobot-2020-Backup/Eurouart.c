/*
    En esta biblioteca incluyo las funciones clásicas de la UART y las propias del protocolo de este año(Eurobot 2020).
    Las funciones de este año se encuentran al final del documento

    Autor: Javier Ortiz Perez-Jaraiz    (Trompiz, el que no gestiona)
    Año: 2020


*/

#include "Eurouart.h" //Enlazo con la biblioteca
#include <LPC17xx.h>

//Configuraci�n de la Baudrate
static int uart0_set_baudrate(unsigned int baudrate) //Esta funci�n nos permite ajustar los registros de la MiniDK2 para ajustar al BAUDRATE con una gran precision. Funcion extraida de un ejemplo de la bb.
{
    int errorStatus = -1; //< Failure

    // UART clock (FCCO / PCLK_UART0)
    // unsigned int uClk = SystemCoreClock / 4;
    unsigned int uClk = SystemCoreClock / 4;
    unsigned int calcBaudrate = 0;
    unsigned int temp = 0;

    unsigned int mulFracDiv, dividerAddFracDiv;
    unsigned int divider = 0;
    unsigned int mulFracDivOptimal = 1;
    unsigned int dividerAddOptimal = 0;
    unsigned int dividerOptimal = 0;

    unsigned int relativeError = 0;
    unsigned int relativeOptimalError = 100000;

    uClk = uClk >> 4; /* div by 16 */

    /*
     *  The formula is :
     * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)
     *
     * The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
     * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15
     */
    for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++)
    {
        for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++)
        {
            temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv);

            divider = temp / baudrate;
            if ((temp % baudrate) > (baudrate / 2))
                divider++;

            if (divider > 2 && divider < 65536)
            {
                calcBaudrate = temp / divider;

                if (calcBaudrate <= baudrate)
                {
                    relativeError = baudrate - calcBaudrate;
                }
                else
                {
                    relativeError = calcBaudrate - baudrate;
                }

                if (relativeError < relativeOptimalError)
                {
                    mulFracDivOptimal = mulFracDiv;
                    dividerAddOptimal = dividerAddFracDiv;
                    dividerOptimal = divider;
                    relativeOptimalError = relativeError;
                    if (relativeError == 0)
                        break;
                }
            }
        }

        if (relativeError == 0)
            break;
    }

    if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR) / 100))
    {

        LPC_UART0->LCR |= DLAB_ENABLE; // importante poner a 1
        LPC_UART0->DLM = (unsigned char)((dividerOptimal >> 8) & 0xFF);
        LPC_UART0->DLL = (unsigned char)dividerOptimal;
        LPC_UART0->LCR &= ~DLAB_ENABLE; // importante poner a 0

        LPC_UART0->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);

        errorStatus = 0; //< Success
    }

    return errorStatus;
}

void uart0_init(int baudrate)
{
    //Comunicaci�n por USB, asi que no hace falta ningun USB
    LPC_PINCON->PINSEL0 |= (1 << 4) | (1 << 6); // Change P0.2 and P0.3 mode to TXD0 and RXD0

    LPC_UART0->LCR &= ~STOP_1_BIT & ~PARITY_NONE; // Set 8N1 mode (8 bits/dato, sin pariad, y 1 bit de stop)
    LPC_UART0->LCR |= CHAR_8_BIT;

    uart0_set_baudrate(baudrate); // Set the baud rate

    LPC_UART0->IER = (1 << 1) | (1 << 0); // Interrupciones habilitadas de transmisi�n y recepci�n, no se activa ninguna de fallos.
    NVIC_EnableIRQ(UART0_IRQn);           // Enable the UART interrupt (for Cortex-CM3 NVIC)
    NVIC_SetPriority(UART0_IRQn, 1);      //Asigno prioridad
}

void UART0_IRQHandler(void)
{
    static int i = 0;

    switch (LPC_UART0->IIR & 0x0E)
    {

    case 0x04: /* Dato recibido */
        //transmitir_cadenaUART0("X");
        //*ptr_rx=LPC_UART0->RBR;
        buffer[i] = LPC_UART0->RBR; /* lee el dato recibido y lo almacena */

        if (buffer[i] < 31) // Caracter return --> Cadena completa

        {
            transmitir_cadenaUART0("R");
            /*if (i != bits_esperados)
                error_longitud = 1; */
            i = 0;
            rx_completa = 1; /* rx completa */
        }
        else
            i++;
        break;

    case 0x02: /*Interrupciónn activada para enviar*/
        if (*ptr_tx != 0)
            LPC_UART0->THR = *ptr_tx++; /* carga un nuevo dato para ser transmitido */
        else
            tx_completa = 1;
        break;
    }
}
void transmitir_cadenaUART0(char *cadena)
{
    ptr_tx = cadena;
    tx_completa = 0;
    LPC_UART0->THR = *ptr_tx++; // IMPORTANTE: Introducir un car�cter al comienzo para iniciar TX o
} // activar flag interrupci�n por registro transmisor vacio

//Funciones nuevas:
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
