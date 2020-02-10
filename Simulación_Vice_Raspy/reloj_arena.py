"""
    Demo Reloj de arena:
        El robot se moverá realizando un reloj de arena, de base
        1m y con un angulo de inclinación 45º
"""
# Bibliotecas
import serial   # Necesaria
import time     # Añadida para la demo
tupla = [0,1]
print (len(tupla))
def main():
    # Puertos usados en la demo:
    MDK2_Port = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.02)
    instruccion = ("0D+100005000000","0G-135", "0D+141405000000","0G+135", "0D+100005000000","0G+135","0D+141405000000","0G-135")
    acabado = 0
    i = 0
    c = 1
    while acabado != 1:
        mensaje_recibido = MDK2_Port.readline()      # Guardo lo que leo en un string
        if(c):                                       # Si es la primera vez no espero a que me manden nada  
            mensaje = instruccion[i] + '\0'
            MDK2_Port.write(mensaje.encode())
            c=0
        elif mensaje_recibido[0] == b'S':
            print("A")
            i = i+1
            if(i == len(instruccion)):
                acabado = 1
            else:
                mensaje = instruccion[i] + '\0'
                MDK2_Port.write(mensaje.encode())
    print("Programa acabado")
    MDK2_Port.close() 