
//Biblioteca:
#include "./GLCD/GLCD.h"
#define ARRIBA_Y 10
#define MEDIO_Y 200
#define ABAJO_Y 300
#define YQ ARRIBA_Y
#define YI2C MEDIO_Y - 30
#define YADC MEDIO_Y
#define YDISTANCIA MEDIO_Y - 60
#define YFRECUENCIA MEDIO_Y - 90
#define YSERVO MEDIO_Y - 120
#define YUART MEDIO_Y + 30
#define MEDIO_X 30
#define YO ARRIBA_Y + 30
//Funciones:

void LCD_config(int estado, float temperatura_adc, float temperatura_i2c, float distancia_medida, float frecuencia, float grados_servo);
void LCD_mostrar_variable(float numero, char tipo);