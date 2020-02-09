typedef struct posicion				// definimos la estructura posicion, que contiene los valores de cordenada x e y y angulo del robot,
											// adem�s de los valores de los contadores de los encoders
{
	double pos_x, pos_y, theta;
	int contador_derecho, contador_izquierdo;
} posicion;

posicion pose;								//Variable global que contiene los valores de posicion que se pasar�n a alto nivel

void init_odom(void){					//Esta funci�n inicializa las interrupciones externas y las GPIO que llevan los encoders en cuadratura.
	
	LPC_PINCON->PINSEL4 |= 	(1<<24) | (1<<26) ; //Pongo los pines 2.12 y 2.13 como EINT2 y EINT3 (derecha e izquierda respectivamente).
	LPC_PINCON->PINSEL0 &= !( (3<<20) | (3<<22) ) ; //Pongo los pines 0.10 y 0.11 como GPIOS.
	LPC_GPIO0->FIODIR 	&= !( (1<<10) | (1<<11) ) ;	//Defino los pines 0.10 y 0.11 como INPUT.
	
	LPC_SC->EXTMODE			|=	(1<<2)	| (1<<3) ;	//Pongo las interrupciones 2 y 3 para ser sensibles a flanco
	LPC_SC->EXTPOLAR		|=	(1<<2)	| (1<<3) ;  //Defino el flanco de activavi�n como el flanco de subida
	
	NVIC_EnableIRQ(EINT2_IRQn);									//Activo las interrupciones para EINT 2 y 3			
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT2_IRQn, 4);						//Ajusto la prioridad de las interruciones a un valor alto
	NVIC_SetPriority(EINT3_IRQn, 5);

}
/***********************************************************************************************************************************
																				IMPORTANTE COMPROBAR EL SENTIDO DE LAS RUEDAS
																					PARA LAS INTERRUPCIONES DE LOS ENCODERS
***********************************************************************************************************************************/
void EINT2_IRQHandler (void){			//C�digo de la interrupci�n del encoder de la rueda derecha 

	if ((LPC_GPIO0->FIOPIN & (1<<11)) >> 11)					//Valor del pin 0.11
		pose.contador_derecha++;												//Si al cambiar a alto el canal A el canal B esta en alto vamos hacia alante
	else
		pose.contador_derecha--;												//Si al cambiar a alto el canal A el canal B esta en bajo vamos hacia atras

}

void EINT3_IRQHandler (void){			//C�digo de la interrupci�n del encoder de la rueda izquierda 
	
	if ((LPC_GPIO0->FIOPIN & (1<<10)) >> 10)					//Valor del pin 0.11
		pose.contador_izquierda++;											//Si al cambiar a alto el canal A el canal B esta en alto vamos hacia alante
	else
		pose.contador_izquierda--;											//Si al cambiar a alto el canal A el canal B esta en bajo vamos hacia atras

}

void act_odom(posicion pos_actual) 
{
	double deltaO, avance, deltaX, deltaY;		//definici�n de avance en recto del robot, incremento del �ngulo, incremento en X e incremento en Y 

	
	avance=(pose.contador_derecho*p_a_cm	+	pose.contador_izquierdo*p_a_cm)	/	2;          						//c�lculo del avance en recto del robot
	deltaO=((pose.contador_derecho*p_a_cm	-	pose.contador_izquierdo*p_a_cm)	/	LONG_EJE)*180/PI;		//c�lculo del incremento del �ngulo en grados
	

	deltaX						=		avance*cos(pos_actual.theta*PI/180);   			//calculo del avance en X del robot  (funcion cos en radianes)
	deltaY						=		avance*sin(pos_actual.theta*PI/180);				//calculo del avance en Y del robot  (funcion sin en radianes)
	pos_actual.pos_x	+=	deltaX;
	pos_actual.pos_y	+=	deltaY;						//incrementar las variables globales con los datos calculados
	pos_actual.theta	+=	deltaO;

	while (pos_actual.theta>=360)							// si se pasa de 360� convierte a rango 0
		pos_actual.theta-=360;								// no se contempla que de mas de una vuelta en un tiempo de muestreo
	while (pos_actual.theta<0)									// si se pasa de 360� convierte a rango 0
		pos_actual.theta+=360;	

}