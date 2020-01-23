#ifndef UART_H_
#define UART_H_

// Accepted Error baud rate value (in percent unit)

//Parametros inamovibles comunicación:
#define UART_ACCEPTED_BAUDRATE_ERROR 3
#define CHAR_8_BIT (3 << 0)
#define STOP_1_BIT (0 << 2)
#define PARITY_NONE (0 << 3)
#define DLAB_ENABLE (1 << 7)
#define FIFO_ENABLE (1 << 0)
#define RBR_IRQ_ENABLE (1 << 0)
#define THRE_IRQ_ENABLE (1 << 1)
#define UART_LSR_THRE (1 << 5)
#define RDA_INTERRUPT (2 << 1)
#define CTI_INTERRUPT (6 << 1)

//Parametros de mensaje
#define T_INSTRUCCION buffer[0]       //Primer char, es el que identifica la instrucción
#define BIT_DGrados (buffer[1] - '0') //Valor del bit en el que se indican las decenas de los grados.
#define BIT_UGrados (buffer[2] - '0') //Valor del bit en el que se indican las unidades de los grados.
#define BIT_S_OX buffer[3]            //Posición del bit de signo del objetivo en x.
#define BIT_M_OX (buffer[4] - '0')    //Valor del bit en el que se indican las milesimas de la posición objetivo en x.
#define BIT_C_OX (buffer[5] - '0')    //Valor del bit en el que se indican las centenas de la posición objetivo en x.
#define BIT_D_OX (buffer[6] - '0')    //Valor del bit en el que se indican las decenas de la posición objetivo en x.
#define BIT_U_OX (buffer[7] - '0')    //Valor del bit en el que se indican las unidades de la posición objetivo en x.
#define BIT_S_OY buffer[8]            //Posición del bit de signo del objetivo en y.
#define BIT_M_OY (buffer[9] - '0')    //Valor del bit en el que se indican las milesimas de la posición objetivo en y.
#define BIT_C_OY (buffer[10] - '0')   //Valor del bit en el que se indican las centenas de la posición objetivo en y.
#define BIT_D_OY (buffer[11] - '0')   //Valor del bit en el que se indican las decenas de la posición objetivo en y.
#define BIT_U_OY (buffer[12] - '0')   //Valor del bit en el que se indican las unidades de la posición objetivo en y.

extern char *ptr_rx;       // Puntero de recepción
extern char rx_completa;   // Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
extern char *ptr_tx;       // Puntero de transmisión
extern char tx_completa;   // Flag de transmision completa
extern char buffer[70];    // Buffer de recepción de caracteres
extern int error_longitud; // Flag visa de que se ha encontrado con un mensaje más grande de lo espeardo
extern void uart0_init(int baudrate);
extern void transmitir_cadenaUART0(char *cadena);

#endif /* UART_H_ */
