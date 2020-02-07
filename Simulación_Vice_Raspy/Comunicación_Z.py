""" 
    En esta demo se implementan las funciones necesarias para la comuncación desde la
    Raspberry con la MDK2. 
"""
# Importo las bibliotecas:
import serial  # Necesaria
import time  # Añadida para la demo
#from serialpacking import POSE_TO_MSG


def main():
    # Puertos usados en la demo:
    MDK2_Port = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.02) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
    # Puerto usado en la demo para probar cosas
    # MDK2_Port = serial.Serial('COM3', 115200, timeout=0.02)

    # Modo de selección de instrucciones Manual/Predefinidas en Z:
    # Scaneo
    # Seleccion de instrucciones

    # Opciones predefinidas:
    #instruccion = ("0G+180", "0D-050010002000", "0C4500100020004000") #Instruccion de ejemplo 
    #instruccion = ("0D+130005000000","0G-135", "0D+164005000000","0G+135", "0D+130005000000")   #Instrucción de la z
    instruccion = ("0D+050005000000","0G-135", "0D+030005000000","0G+135", "0D+050005000000")
    acabado = 0
    i = 0
    c = 1
    while acabado != 1:
        mensaje_recibido = MDK2_Port.readline()  # Guardo lo que leo en un string

        if(c):
            # ENVIAMOS MENSAJE:
            mensaje = instruccion[i] + '\0'
            MDK2_Port.write(mensaje.encode())
            c=0

        elif mensaje_recibido == b'S':
            print("A")
            i = i+1
            if(i <len(instruccion)-1):
                mensaje = instruccion[i] + '\0'
                MDK2_Port.write(mensaje.encode())
            else:
                acabado = 1
    print("Programa acabado")
    MDK2_Port.close()  # Cierro el puerto al finalizar el programa


main()