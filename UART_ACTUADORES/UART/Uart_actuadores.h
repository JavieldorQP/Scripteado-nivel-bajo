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
//Parametros de posición:
//Generales
//#define BIT_PRIO buffer[0]
#define T_INSTRUCCION buffer[0] //Primer char, es el que identifica la instrucción
//Argumentos
#define BIT_C_DISTANCIA buffer[1]
#define BIT_D_DISTANCIA buffer[2]
#define BIT_U_DISTANCIA buffer[3]


extern char *ptr_rx;     // Puntero de recepción
extern char rx_completa; // Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
extern char *ptr_tx;     // Puntero de transmisión
extern char tx_completa; // Flag de transmision completa
extern char buffer[70];  // Buffer de recepción de caracteres

//Variables de MIguel
extern int distancia;
extern char Instruccion_Codigo;


extern void uart0_init(int baudrate);
extern void transmitir_cadenaUART0(char *cadena);
extern void Traduccion_Variables(void);

#endif /* UART_H_ */
