""" 
    Funciones de la comunicación de la Raspy
"""
# Importo las bibliotecas:
import serial  # Necesaria

def envio_instrucciones_traccion(instruccion_1, instruccion_2, instruccion_3):
    # Puertos usados en la demo:
    # MDK2_Port_traccion = serial.Serial('/dev/ttyUSB0', 115200, timeout=1) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
    # Puerto usado en la demo para probar cosas
    MDK2_Port_traccion = serial.Serial('COM3', 115200, timeout=0.02)
    instruccion = (instruccion_1, instruccion_2, instruccion_3)
    acabado = 0
    i = 0
    c = 1
    while acabado != 1:
        mensaje_recibido = MDK2_Port_traccion.readline()  # Guardo lo que leo en un string

        if(c):
            # ENVIAMOS MENSAJE:
            mensaje = instruccion[i] + '\0'
            MDK2_Port_traccion.write(mensaje.encode())
            print("S")
            c = 0

        if mensaje_recibido == b'A':
            print("A")
            i = i+1
            if(i < len(instruccion)):
                mensaje = instruccion[i] + '\0'
                MDK2_Port_traccion.write(mensaje.encode())
            else:
                acabado = 1
    MDK2_Port_traccion.close()  # Cierro el puerto al finalizar el programa

# Funcion propia del protocolo

def envio_instrucciones_actuadores(instruccion):
    """
    envio_instrucciones_actuadores:
        Envia la instrucción a la MDK2 encargada de los actuadores
    """
    MDK2_Port_actuadores = serial.Serial('/dev/ttyUSB0', 115200, timeout=1) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
    c = 1
    acabado = 0
    while acabado != 1:
        mensaje_recibido = MDK2_Port_actuadores.readline()  # Guardo lo que leo en un string
        if(c):
            # ENVIAMOS MENSAJE:
            mensaje = instruccion + '\0'
            MDK2_Port_actuadores.write(mensaje.encode())
            c = 0
        if (mensaje_recibido == b'A'):
            acabado = 1
    MDK2_Port_actuadores.close() 

def pose_to_msg(Posicion):
    """
        pose_to_msg:: Num -> String
        Utiliza un entero de tres cifras y devuelve un string con el formato que se utiliza 
        en la comunicación para los datos tipos posición posiciones 
        >>> pose_to_msg(23)
        '+0023'
        >>> pose_to_msg(-24)
        '-0024'
    """
    if(Posicion >= 0):                                              # Si el numero es positivo o 0
        if (Posicion >= 1000):                                      # Si el numero es superior a 1000
            MSG = "+" + str(int(Posicion))
        elif (Posicion >= 100):
            MSG = "+0" + str(int(Posicion))
        elif (Posicion >= 10):
            MSG = "+00" + str(int(Posicion))
        else:  # lif (Posicion >= 0):
            MSG = "+000" + str(int(Posicion))
        # if Posicion < 0:
        #     MSG = "-" + MSG
        # else:
        #     MSG = '+' + MSG
    # if posicion < 0:
    #     signo = "-"
    # else:
    #     signo = "+"
    # return signo + msg

    if(Posicion < 0):
        if (abs(Posicion) >= 1000):                                 # Si el numero es superior a 1000
            MSG = "-" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 100):
            MSG = "-0" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 10):
            MSG = "-00" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 0):
            MSG = "-000" + str(abs(int(Posicion)))
    return MSG

def angle_to_msg(Grados):
    """
        angle_to_msg:: Num -> String
        Utiliza un entero de tres cifras y devuelve un string con el formato que se utiliza 
        en la comunicación para los datos tipos posición posiciones 
        >>> angle_to_msg(123)
        '+123'
        >>> angle_to_msg(-124)
        '-124'
        >>> angle_to_msg(9)
        '+009'
        >>> angle_to_msg(-14)
        '-014'
        >>> angle_to_msg(-174)
        '-174'
        >>> angle_to_msg(90)
        '+090'
    """
    if (Grados >= 100):
        MSG = "+" + str(int(Grados))
    elif (Grados >= 10):
        MSG = "+0" + str(int(Grados))
    elif (Grados >= 0):
        MSG = "+00" + str(int(Grados))
    elif (Grados > -10):
        MSG = "-00" + str(abs(int(Grados)))
    elif (Grados > -100):
        MSG = "-0" + str(abs(int(Grados)))
    elif (Grados > -1000):
        MSG = "-" + str(abs(int(Grados)))
    else:
        print("Error")
    return MSG
if __name__ == "__main__":
    import doctest
    doctest.testmod()

