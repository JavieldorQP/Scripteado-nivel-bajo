
"""
    Estrategia Eurobot 2020:
        Se basa en tres clases que ceden el control la una a la otra, de tal manera que el planificador revisa la clase Juego, decide
        que pasa a ejecutor y ejecutor decide que instruciones mandar a la MDK2, cuando ejecutor acaba actualiza la clase juego.
        OHHH JEFE SUPREMO APIADATE DE MI....

    Autor: Javir Ortiz Perez-Jaraiz (Lesmus Trompiz)
    Año: Eurobot 2020


"""
# Biliotecas:
import time
import math
import random
from Simulo_robot import simula_movimiento, actuadores
# Constantes
# Definir vaso 1 como una tupla
# Indicar cuantos robots hay
VASO_1X = 0
VASO_1Y = 0
ACTIVACION_EXPERIMENTOX = -1200
ACTIVACION_EXPERIMENTOY = 800
# Estanterias:
ESTANTERIA_VASOS_1X = -3000
ESTANTERIA_VASOS_1Y = -1150
ESTANTERIA_VASOS_2X = -575
ESTANTERIA_VASOS_2Y = 900
ESTANTERIA_VASOS_3X = +575
ESTANTERIA_VASOS_3Y = 900
ESTANTERIA_VASOS_4X = +3000
ESTANTERIA_VASOS_4Y = 1150
ESTANTERIA_VASOS_3X = +575
ESTANTERIA_VASOS_3Y = 900
# Bahia:
BAHIA_CENTRAL_AMARILLOX = -400  # Tener en cuenta que es la del otro lado
BAHIA_CENTRAL_AMARILLOY = -650
BAHIA_CENTRAL_AZULX = -BAHIA_CENTRAL_AMARILLOX
BAHIA_CENTRAL_AZULY = BAHIA_CENTRAL_AMARILLOY
# Puertos:
PUERTO_SUR_X = -1200
PUERTO_SUR_Y = -300
PUERTO_SUR_AMARILLOX = -PUERTO_SUR_X
PUERTO_SUR_AMARILLOY = PUERTO_SUR_Y
PUERTO_NORTE_X = PUERTO_SUR_X
PUERTO_NORTE_Y = 550
PUERTO_NORTE_AMARILLOX = -PUERTO_NORTE_X
PUERTO_NORTE_AMARILLOY = PUERTO_NORTE_Y
# Posiciones iniciales:
PA_AX = PUERTO_SUR_AMARILLOX
PA_AY = PUERTO_SUR_AMARILLOY+20
POS_AX = PUERTO_SUR_AMARILLOX
POS_AY = PUERTO_SUR_AMARILLOY-20

PA_GX = PUERTO_SUR_X
PA_GY = PUERTO_SUR_Y+20
POS_GX = PUERTO_SUR_X
POS_GY = PUERTO_SUR_Y-20

#
BAHIA_AZULX = -1300
BAHIA_AZULY = 200
BAHIA_AMARILLOX = -BAHIA_AZULX
BAHIA_AMARILLOY = BAHIA_AZULY
# Posición para ver la camara:
CAMARA_X = 0
CAMARA_Y = 800
# Instrucciones
ACTIVAR_EXPERIMENTO = 0
BAHIA_SOLTAR = 1
ESTANTERIAS_NEUTRO_CERCA = 2
ESTANTERIAS_CERCA = 3
ESTANTERIAS_NEUTRO_ENEMIGO = 4
ESTANTERIAS_ENEMIGO = 5
BRUJULA = 6
CASA = 7
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1


def camara(juego):
    juego.brujula = random.choice(["N", "S"])
    time.sleep(4)
    juego.tiempo = juego.tiempo+4


class robot:  # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot
    def __init__(self, posx, posy, orientacion):
        self.disponible = True
        self.pos = [posx, posy]
        self.bandera = 0
        self.velocidad = [0, 0]
        self.velocidadangular = [0]
        self.orientacion = math.radians(orientacion)


class Posavasos:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  # Datos por concretar
        self.ventosas_ocupada = False  # No ha cogido ningún vaso


class Parejitas:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  # Datos por concretar
        self.compuertas = False  # Estan abiertas
        self.actuador_banda = False


class vasos():  #
    def __init__(self, vaso1, vaso2, vaso3, vaso4):
        self.vaso1 = vaso1
        self.vaso2 = vaso2
        self.vaso3 = vaso4
        self.vaso4 = vaso4


class estanterias():  #
    def __init__(self, estanteria1, estanteria2, estanteria3, estanteria4):
        self.estanteria1 = estanteria1
        self.estanteria2 = estanteria2
        self.estanteria3 = estanteria3
        self.estanteria4 = estanteria4


class game:  # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, lado, pos_inicio_parejitasx, pos_inicio_parejitasy, pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial):
        self.tiempo = 0
        self.puntos = 0
        self.lado = lado
        self.posavasos = Posavasos(
            pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial)
        self.parejitas = Parejitas(
            pos_inicio_parejitasx, pos_inicio_parejitasy, orientacion_inicial)
        self.vasos = vasos(True, True, True, True)
        self.estanterias = estanterias(True, True, True, True)
        self.experimento = True
        self.brujula = "D"


# Revisa el estado de la clase juego y decide que hacer, hecho esto cede el contro a ejecutor


def planificador(juego):
    if(juego.tiempo <= 90):  # Hay tiempo
        if(juego.posavasos.estado):  # Posavasos no esta ocupado
            if(juego.experimento):
                print("Activo el experimento")
                ejecutor(ACTIVAR_EXPERIMENTO, POSAVASOS, juego)
            elif(juego.robot.ventosas_ocupada):
                print("Vamos a la bahia a descargar los vasos")
                ejecutor(BAHIA_SOLTAR, POSAVASOS, juego)
            elif (juego.estanterias.estanteria2):
                print("Voy a la estanteria del medio")
                ejecutor(ESTANTERIA_VASOS_1X, ESTANTERIA_VASOS_1Y,
                         juego, math.radians(90))
                juego.estanterias.estanteria2 = 0
                actuadores("R", juego)
            elif (juego.estanterias.estanteria1):
                print("Voy a la estanteria de mi lado")
                ejecutor(ESTANTERIA_VASOS_1X, ESTANTERIA_VASOS_1Y,
                         juego, math.radians(180))
                juego.estanterias.estanteria1 = 0
                actuadores("R", juego)
            elif (juego.brujula == "D"):
                print("Voy a mirar la brujula")
                ejecutor(CAMARA_X, CAMARA_Y,
                         juego, math.radians(90))
                camara(juego)
            else:
                print("No tengo que hacer nada")
                if(juego.brujula == "N"):
                    ejecutor(PUERTO_SUR_X, PUERTO_SUR_Y, juego, 180)
                else:
                    ejecutor(PUERTO_NORTE_X,
                             PUERTO_NORTE_Y, juego, 180)
                juego.puntos = juego.puntos+15
    elif(juego.parejitas.estado):
        print("Parejitas es tu turno")
    else:
        print("Rapido pirate")
        if(juego.brujula == "N"):
            ejecutor(PUERTO_SUR_X, PUERTO_SUR_Y, juego, 180)
        else:
            ejecutor(PUERTO_NORTE_X, PUERTO_NORTE_Y, juego, 180)
        juego.puntos = juego.puntos+15


# Ejecutor recrea TyM parcialmente, es decir recibe que ha de hacer y lo traduce en  instrucciones
def ejecutor(orden, robot, juego):
    # (Falta por implementar la creación de los waypoints, asi que solo hace gira->avanza)
    if(orden == ACTIVAR_EXPERIMENTO):
        Objx = ACTIVACION_EXPERIMENTOX
        Objy = ACTIVACION_EXPERIMENTOY
        Orientacion_final = math.radians(90)
        # Publicaria en un topic
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
    elif(orden == BAHIA_SOLTAR):
        Objx = BA
        Objy = ACTIVACION_EXPERIMENTOY
        Orientacion_final = math.radians(90)
        # Publicaria en un topic
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        actuadores("S", juego)
    elif(orden == ESTANTERIAS_CERCA):

    elif(orden == ESTANTERIAS_NEUTRO_CERCA):

    elif(orden == BRUJULA):

        ESTANTERIA_VASOS_1X, ESTANTERIA_VASOS_1Y,
        juego, math.radians(90)
    # Simula el nivel bajo
    # def control(juego, instrucciones)


def main():
    print("Selecciona el lado (A para amarillo y G para ):")
    lado = input()
    if(lado == "A"):
        juego = game(lado, PA_AX, PA_AY, POS_AX, POS_AY, 0)
    else:
        juego = game(lado, PA_GX, PA_GY, POS_GX, POS_GY, 0)
    while juego.tiempo < 140:
        print("Situación inicial:")
        print(juego.parejitas.robot.pos[0])
        print(juego.parejitas.robot.pos[1])
        planificador(juego)
        print("Vamos por el segundo de partido:")
        print(ju1ego.tiempo)
        print("Tenemos los siguientes puntos")
        print(juego.puntos)
        # print(juego.campo.vaso1.color)
        # print(juego.campo)


main()
