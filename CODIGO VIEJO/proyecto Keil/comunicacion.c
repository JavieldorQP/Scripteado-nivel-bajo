#include "lpc17xx.h"
#include "uart.h"

#define baudrate 9600
#define N_caracteres 30

//Deben ser globales___________________________________________
char buffer[N_caracteres];//Variable que introduce los comandos (Maximo 30 caracteres)
char *ptr_rx;			// puntero de recepción
char rx_completa;		// Flag de recepción de cadena que se activa a "1" al recibir la tecla return CR(ASCII=13)
char *ptr_tx;			// puntero de transmisión
char tx_completa;		// Flag de transmisión de cadena que se activa al transmitir el caracter null (fin de cadena)
extern int flag_ok;
typedef struct{
	float pos_x;							//Posición en la coordenada X
	float pos_y;							//Posición en la coordenada Y
	float omega;}							//Ángulo respecto a X en grados
pos;
	
extern pos pos_final;
//______________________________________________________________


	
	
	
	void funcion_comunicacion(){
	//Parte de la comprobacion
	int i=0;
	
	while(buffer[i]!=13 && flag_ok==1){
		switch(i){
		case 0:if(buffer[i]=='x')										flag_ok=1;			else flag_ok=0;break;
		case 1:if(buffer[i]=='+' || buffer[i]=='-')	flag_ok=1;			else flag_ok=0;break;
		case 2:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 3:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 4:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 5:if(buffer[i]=='y')										flag_ok=1;			else flag_ok=0;break;
		case 6:if(buffer[i]=='+' || buffer[i]=='-')	flag_ok=1;			else flag_ok=0;break;
		case 7:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 8:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 9:if(buffer[i]>='0' && buffer[i]<='9')	flag_ok=1;			else flag_ok=0;break;
		case 10:if(buffer[i]=='a')									flag_ok=1;			else flag_ok=0;break;
		case 11:if(buffer[i]>='0' && buffer[i]<='9')flag_ok=1;			else flag_ok=0;break;
		case 12:if(buffer[i]>='0' && buffer[i]<='9')flag_ok=1;			else flag_ok=0;break;
		case 13:if(buffer[i]>='0' && buffer[i]<='9')flag_ok=1;			else flag_ok=0;break;
		default:flag_ok=0;
		}
		i++;
	}
	
	//Parte de la lectura
	if(flag_ok==1){
			pos_final.pos_x=100.0*(buffer[2]-'0')+10.0*(buffer[3]-'0')+(buffer[4]-'0');
			pos_final.pos_y=100.0*(buffer[7]-'0')+10.0*(buffer[8]-'0')+(buffer[9]-'0');
			pos_final.omega=100.0*(buffer[11]-'0')+10.0*(buffer[12]-'0')+(buffer[13]-'0');
			if(buffer[1]=='-')
				pos_final.pos_x=(-1)*(pos_final.pos_x);
			if(buffer[6]=='-')
				pos_final.pos_y=(-1)*(pos_final.pos_y);
			tx_cadena_UART0("Posicion actualizada");
		}
	if(flag_ok==0)
		tx_cadena_UART0("Comandos erroneos\n\r");
		
}
