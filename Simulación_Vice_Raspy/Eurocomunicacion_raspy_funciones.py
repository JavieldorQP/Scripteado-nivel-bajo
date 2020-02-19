""" 
    Funciones de la comunicación de la Raspy
"""
# Importo las bibliotecas:
import serial  # Necesaria
import time
def envio_instrucciones_traccion(instruccion_1, instruccion_2, instruccion_3):
    # Puertos usados en la demo:
    MDK2_Port_traccion = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.02) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
    # Puerto usado en la demo para probar cosas
    #MDK2_Port_traccion = serial.Serial('COM3', 115200, timeout=0.02)
    instruccion = (instruccion_1, instruccion_2, instruccion_3)
    acabado = 0
    i = 0
    primera_vez = 1
    while acabado != 1:
        if(primera_vez):
            # ENVIAMOS MENSAJE
            mensaje = instruccion[i] + '\0'
            MDK2_Port_traccion.write(mensaje.encode())
            primera_vez = 0
            print(f"Instrucción {mensaje}")
            time.sleep(2)
        mensaje_recibido = MDK2_Port_traccion.readline()  # Guardo lo que leo en un string
        odom,siguiente_orden = analizo_mensaje(mensaje_recibido) 
        print(odom)
        if (siguiente_orden):
            print("S")
            i = i+1
            if (i == len(instruccion)):
                print("Instrucciones transmitidas")
                acabado = 1
            else:
                mensaje = instruccion[i] + '\0'
                MDK2_Port_traccion.write(mensaje.encode())
                print(f"Instrucción {mensaje}")
        
    MDK2_Port_traccion.close()  # Cierro el puerto al finalizar el programa

def analizo_mensaje(mensaje):
    mensaje = str(mensaje, 'utf-8')   #Es este formato el correcto
    print(mensaje)
    msg_size = len(mensaje)
    if (msg_size == 0):
        odom = [0,0,0]
        return odom, 0
    else:
        if (mensaje[0] == "S"):
            bitx = mensaje.find("X")       
            bity = mensaje.find("Y")  
            bita = mensaje.find("A")     
            bitfinal = len(mensaje)
            odom_x = msg_to_odom(mensaje, bitx, bity)
            odom_y = msg_to_odom(mensaje, bity, bita)
            #odom_angle = msg_to_odom(mensaje, bita, bitfinal)
            #print(f"ODOM ANGLE {odom_angle}")
            odom_angle = 0
            if (odom_angle>180):
                odom_angle = -1 * (360 - odom_angle)
            odom = [odom_x, odom_y, odom_angle]
            return odom,1
        else: 
            odom = [0,0,0]
            return odom, 0
        
             

def msg_to_odom(mensaje,bitinicio, bitfinal):
    """
    msg_to_odom:: String -> Int
        Recibe un string con el formato de comunicación de este año
        y devuelve un int con la odometria resultante. 
    """
    odom = '0'
    if (mensaje[bitinicio + 1] == "-"):
        signo = -1
        for numero_cifra in  range(bitinicio+2, bitfinal-1): 
            odom = odom + mensaje[numero_cifra]    
        odom = signo * int(odom)
        return odom
    
    else:
        signo = 1 
        for numero_cifra in range(bitinicio+1, bitfinal-1): 
            odom = odom + mensaje[numero_cifra]
        odom = signo * int(odom)
        return odom

    



def envio_instrucciones_actuadores(instruccion):
    """
    envio_instrucciones_actuadores:
        Envia la instrucción a la MDK2 encargada de los actuadores
    """
    MDK2_Port_actuadores = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.2) #Si estamos en la Raspberry py y conectamos la MDK2 por el puerto de arriba a la derecha.
   
    print("hola")
    c = 1
    ON = True
    while (ON):
        if (c):
            mensaje = instruccion + '\0'
            MDK2_Port_actuadores.write(mensaje.encode())
            print(f"Mensaje {instruccion} enviada")
            c = 0
        mensaje_recibido = MDK2_Port_actuadores.readline()  # Guardo lo que leo en un string
        if (mensaje_recibido == b'S'):
            ON = False
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

