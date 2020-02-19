import rospy
import serial
import time
from eurobot_ws.msg import OperadorStatus, Instruccion 
from geometry_msgs.msg import Pose2D

# mantiene un estado entre ["Moviendose", "Parado", "Error"]

class Operador:
    # Datos actuales de la MiniDK2
    odom = Pose2D()
    status = OperadorStatus()
    _ultimo_buffer_actual = "--"

    # Esto por visibilidad
    instruccion_pasada = Instruccion()
    instruccion_actual = Instruccion()
    instruccion_proxima = Instruccion()

    # Guardamos las ultimas instrucciones enviadas para traducir de ID a Instruccion
    _ultimas_instrucciones = {}

    # Tenemos un diccionario para traducir a las acortaciones
    _nombres_instrucciones = {"Recta": 'D', "Giro": 'G', "Curva": 'C', "Stop": 'F', "Estado": 'S'}
    

    def actualizar_estado(self) -> None:
        '''
        Envia una peticion de estatus a la miniDK2, espera a su respuesta y 
         actualiza el estado
        '''
        # TODO Decidir si la MiniDK2 envia actualizaciones periodicas o si enviamos peticiones de estado
        # Leer buffer por si hay alguna actualizacion
        msg_mdk2 = self.serial.Read()
        # Si no lo hay y ha pasado mas de un tiempo definido
        # enviar status request
        if(len(msg_mdk2) != 15):
            inst = Instruccion()
            inst.id = 'EE'
            inst.tipo = "Estado"
            self.enviar_instruccion(inst)
            time.sleep(0.01)
            msg_mdk2 = self.serial.Read()
            # esperar respuesta
        # si no hay respuesta (timeout) poner nuestro estado en modo "Error"
        if(len(msg_mdk2) != 15):
            self.status.Estado = "Error"
        # si hay respuesta procesarla
        else:
            self.__procesar_mensaje_estado(msg_mdk2)


    def enviar_instruccion(self, Inst) -> bool:
        '''
        Envia una instruccion a la MiniDK2
        '''

        """
        NO QUIERO TUS FORMATOS DE MOMENTO
        # Añade la prioridad y el ID
        instruccion = '1' if Inst.prioridad else '0'
        instruccion += format(str(Inst.id), '02')[-2:] # Hace que sea 2 char solo

        # Añade el tipo de instruccion
        instruccion += self._nombres_instrucciones[Inst.tipo]
        
        # Añade los argumentos PIIC111122223333
        instruccion += format(Inst.arg1, '04')[-4:]
        instruccion += format(Inst.arg2, '04')[-4:]
        instruccion += format(Inst.arg3, '04')[-4:]
        """        
        # Escribe la instruccion al serial
        self.serial.write(instruccion)

        # Guarda la instruccion en el diccionario
        self._ultimas_instrucciones[Inst.id] = Inst 

    def __procesar_mensaje_estado(self, msg: str) -> None:
        '''
        Toma el mensaje de la miniDK2 y lo convierte en Pose2D (Para la odometria) y
         rellena los campos del estado
        El mensaje de estado tiene la siguiente estructura  B1 B2 XXXX YYYY AAA
        ''' 
        # Primero parseamos la cola de instrucciones
        buffer_actual = msg[:2]
        buffer_proximo = msg[2:4]

        # Si la instruccion ejecutandose ha cambiado, entonces ponemos la anterior como ultima instruccion
        if(self._ultimo_buffer_actual != buffer_actual and
        self._ultimo_buffer_actual != "--"):
            self.status.instruccion_pasada = self._ultimas_instrucciones[int(self._ultimo_buffer_actual)]
            self._ultimo_buffer_actual = buffer_actual

        self.status.instruccion_actual = self._ultimas_instrucciones[int(buffer_actual)]
        self.status.instruccion_proxima = self._ultimas_instrucciones[int(buffer_proximo)]

        # Despues parseamos la odometria
        odom_x = msg[4:8]
        odom_y = msg[8:12]
        odom_yaw = msg[12:15]

        self.odom.x = float(odom_x) / 100 # TODO Comprobar que la conversion esta bien
        self.odom.y = float(odom_y) / 100 # TODO Comprobar que la conversion esta bien
        self.odom.theta = float(odom_yaw) / 100 # TODO Comprobar que la conversion esta bien

    def __init__(self, puerto: str, bitrate: int, timeout: float):
        self.serial = serial.Serial(puerto, bitrate, timeout)
        