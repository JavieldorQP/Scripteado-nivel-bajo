
"""
    Estrategia Eurobot 2020:
        Se basa en tres clases que ceden el control la una a la otra, de tal manera que el planificador revisa la clase Juego, decide
        que pasa a ejecutor y ejecutor decide que instruciones mandar a la MDK2, cuando ejecutor acaba actualiza la clase juego.
        OHHH JEFE SUPREMO APIADATE DE MI....

    Autor: Javir Ortiz Perez-Jaraiz (Lesmus Trompiz)
    Año: Eurobot 2020


"""
import time
import math
from funcionesTyM import calculo_giro_avanzo_giro
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
# Bahia
BAHIA_1X = -400
BAHIA_1Y = -650
BAHIA_2X = BAHIA_1X
BAHIA_2Y = -BAHIA_1Y
# Puertos:
PUERTO_SUR_GRISX = -1200
PUERTO_SUR_GRISY = -300
PUERTO_SUR_AMARILLOX = -PUERTO_SUR_GRISX
PUERTO_SUR_AMARILLOY = PUERTO_SUR_GRISY


def control(angulo, distancia, juego):
    """Función que simula el nivel bajo, de momento no hace más que generar delays 
    en función de las distancias recorridas, en el futuro sera capaz de simular 
    enemigos y errores"""
    vdefecto = 500  # 1 m/s
    wdefecto = 360  # 1Hz
    print("Tiempo que tarda el robot:")
    tiempo = abs((distancia/vdefecto))+abs((angulo/wdefecto))
    juego.tiempo = juego.tiempo+tiempo
    print(tiempo)
    time.sleep(tiempo)
    return 'A'


class robot:  # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot
    def __init__(self, posx, posy, vx, vy):
        self.pos = [PUERTO_SUR_GRISX, PUERTO_SUR_GRISY]
        self.bandera = 0
        self.velocidad = [0, 0]
        self.velocidadangular = [0]
        self.orientacion = math.radians(0)


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
        self.estanteria3 = estanteria4
        self.estanteria4 = estanteria4


class game:  # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, posx, posy, vx, vy):
        self.tiempo = 0
        self.puntos = 0
        self.robot = robot(posx, posy, vx, vy)
        self.vasos = vasos(True, True, True, True)
        self.estanterias = estanterias(True, True, True, True)
        self.experimento = True
        # self.vaso1 = vaso(1, 15, 2, 'r')
        # self.campo


# Revisa el estado de la clase juego y decide que hacer, hecho esto cede el contro a ejecutor
def planificador(juego):
    if(juego.tiempo <= 100):
        print("Queda tiempo, vamos por la siguiente instruccion")
        if(juego.experimento):
            print("Activo el experimento")
            ejecutor(ACTIVACION_EXPERIMENTOX,
                     ACTIVACION_EXPERIMENTOY, juego, math.radians(90))
            juego.experimento = 0

        elif (juego.vasos.vaso1):
            print("Vamos a por el vaso 1, hay que avanzar")
            ejecutor(VASO_1X, VASO_1Y, juego, 0)
            juego.vasos.vaso1
    else:
        print("Rapido pirate")
        ejecutor(PUERTO_SUR_GRISX, PUERTO_SUR_GRISY, juego, -180)
    print("He salido")


# Ejecutor recrea TyM parcialmente, es decir recibe que ha de hacer y lo traduce en  instrucciones
def ejecutor(Objx, Objy, juego, orientacion_final):
    # (Falta por implementar la creación de los waypoints, asi que solo hace gira->avanza)
    distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
        juego.robot.pos[0], juego.robot.pos[1], Objx, Objy, juego.robot.orientacion, orientacion_final)
    print(distancia)
    print(angulo_giro1)
    print(angulo_giro2)
    feedback = control(distancia, angulo_giro1+angulo_giro2, juego)
    if(feedback):
        juego.robot.pos[0] = Objx
        juego.robot.pos[1] = Objy
        juego.robot.orientacion = orientacion_final
        return 1
    else:
        print("Error")
        return 0
    # Simula el nivel bajo
    # def control(juego, instrucciones)


def main():
    juego = game(-1500, -700, 0, 0)
    # vaso1 = vaso(1, 15, 2, 'r')
    while juego.tiempo < 140:
        planificador(juego)
        print("Vamos por el segundo de partido:")
        print(juego.tiempo)
        # print(juego.campo.vaso1.color)
        # print(juego.campo)


main()
