
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
from funcionesTyM import calculo_giro_avanzo
# Constantes
# Definir vaso 1 como una tupla
VASO_1X = 0
VASO_1Y = 0
X = 0
Y = 1


def control(angulo, distancia):
    """Función que simula el nivel bajo, de momento no hace más que generar delays 
    en función de las distancias recorridas, en el futuro sera capaz de simular 
    enemigos y errores"""
    vdefecto = 500  # 1 m/s
    wdefecto = 360  # 1Hz
    print("Tiempo que tarda el robot:")
    tiempo = abs((distancia/vdefecto))+abs((angulo/wdefecto))
    print(tiempo)
    time.sleep(tiempo)
    return 'A'


class robot:  # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot
    def __init__(self, posx, posy, vx, vy):
        self.pos = [-1500, -700]
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


class game:  # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, posx, posy, vx, vy):
        self.tiempo = 0
        self.puntos = 0
        self.robot = robot(posx, posy, vx, vy)
        self.vasos = vasos(True, True, True, True)

        # self.vaso1 = vaso(1, 15, 2, 'r')
        # self.campo


# Revisa el estado de la clase juego y decide que hacer, hecho esto cede el contro a ejecutor
def planificador(juego):
    if(juego.tiempo <= 100):
        print("Queda tiempo, vamos por la siguiente instruccion")
    else:
        print("Rapido pirate")
        # ejecutor(Aparcar, juego)
    if juego.vasos.vaso1:
        print("Vamos a por el vaso 1, hay que avanzar")
        ejecutor(VASO_1X, VASO_1Y, juego)
    juego.tiempo = juego.tiempo+1


# Ejecutor recrea TyM parcialmente, es decir recibe que ha de hacer y lo traduce en  instrucciones
def ejecutor(Objx, Objy, juego):
    # (Falta por implementar la creación de los waypoints, asi que solo hace gira->avanza)
    v, b = calculo_giro_avanzo(
        juego.robot.pos[0], juego.robot.pos[1], Objx, Objy, juego.robot.orientacion)
    print(v)
    print(b)
    feeback = control(v, b)
    if(feeback):
        juego.robot.pos[0] = Objx
        juego.robot.pos[1] = Objy
    else:
        print("Error")

    # Simula el nivel bajo
    # def control(juego, instrucciones)


def main():
    juego = game(-1500, -700, 0, 0)
    # vaso1 = vaso(1, 15, 2, 'r')
    while juego.tiempo < 140:
        planificador(juego)
        # print(juego.campo.vaso1.color)
        # print(juego.campo)


main()
