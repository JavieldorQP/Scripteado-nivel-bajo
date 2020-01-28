/**** Estados de la máquina de estados ****/

#define 	INICIAL			0
#define		PARADO			1
#define		RECTA				2
#define		GIRO				3
#define		CURVA				4
#define		FRENO				5
int Estado = 0;

/**** Declaración de variables ****/

int velActual = 0;								// Velocidad actual del robot en mm/s


/**** Función main ****/

int main(){

	
	while(1){
		
		switch(Estado){
			
			/* En el estado inicial, esperamos la llegada de la primera instrucción	 
			basado en la estrategia. */
			case INICIAL:
				nuevo_estado = estado_inicial(Estado)
				break;
			
			/* Estado PARADO: nos quedamos a la espera de la siguiente instrucción.
			Se puede llegar aquí por: ausencia de tareas, enemigo cercano. */
			case PARADO:
				break;
			
			/* Estado RECTA: instrucción de movimiento en línea recta. Se evalua:
			la vel. actual, la vel. máxima, la vel. final y la distancia */
			case RECTA:
				break;
			
			/* Estado GIRO: instrucción de giro sobre el propio eje. Se evalua:
			la vel. actual, la vel. final y ángulo de giro */
			case GIRO:
				break;
			
			/* Estado CURVA: instrucción de movimiento tomando una curva. Se evalua:
			ángulo, radio de curvatura, vel. actual y vel. final */
			case CURVA:
				break;
			
			/* Estado FRENO: instrucción de frenada de emergencia. No se evalua nada.
			Caso particular de RECTA */
			case FRENO:
				break;
			default:
				
		}
		estado = nuevo_estado
	}
}

int estado_inicial(*Estado) {
 while(1){
	 siguiente_estado = ST_INIT
	 // mensaje = leer_comando_uart(args)
	 switch(mensaje.codigo) {
	   ENCENDER_LED:
		 break;
		 APAGAR_LED:
		 break;
		 AVANZAR:
		 // Estado.adonde_ir_x = mensaje.x
		 // Estado.adonde_ir_y = mensaje.x
		 siguiente_estado = ST_AVANZAR
		 break;
	 }
	 return siguiente_estado
 }