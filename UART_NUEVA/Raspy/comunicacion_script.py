""" 
    En esta demo se implementan las funciones necesarias para la comuncación desde la
    Raspberry con la MDK2. A falta de concretar la cantidad de mensajes que se necesitan 
    a nivel bajo y a nivel alto en esta demo se trabaja con los siguientes datos: Posición
    del objetivo, ángulo al que tiene que estar un pequeño actuador (servo) y la odometria del robot 
    que es enviada desde la MDK2.

    Si se carga este programa en la Raspberry y su homologo en la MDK2, seremos capaces de observar
    como un rectángulo pasa por la pantalla de la MDK2, un servo gira y leemos el valor 00 por la 
    pantalla simulando la odometria.

"""
# Importo las bibliotecas:
import serial  # Necesaria
import time  # Añadida para la demo
from serialpacking import POSE_TO_MSG


def main():
    # Puertos usados en la demo:
    # MDK2_Port = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
    # Puerto usado en la demo para probar cosas
    MDK2_Port = serial.Serial('COM3', 115200, timeout=0.02)

    # Modo de selección de instrucciones Manual/Predefinidas en Z:
    # Scaneo
    # Seleccion de instrucciones

    # Opciones predefinidas:
    instruccion = ("G180", "D050010002000", "C4500100020004000")
    acabado = 0
    i = 0
    while acabado != 1:
        mensaje_recibido = MDK2_Port.readline()  # Guardo lo que leo en un string

        if(mensaje_recibido == b'S'):
            # ENVIAMOS MENSAJE:
            mensaje = instruccion[i] + '\0'
            MDK2_Port.write(mensaje.encode())
            print("S")

        elif mensaje_recibido == b'A':
            print("A")
            i = i+1
            if(i < len(instruccion)):
                mensaje = instruccion[i] + '\0'
                MDK2_Port.write(mensaje.encode())
            else:
                acabado = 1
    print("Programa acabado")
    MDK2_Port.close()  # Cierro el puerto al finalizar el programa


main()
