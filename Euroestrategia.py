
"""
    Estrategia Eurobot 2020:
        Se basa en tres clases que ceden el control la una a la otra, de tal manera que el planificador revisa la clase Juego, decide
        que pasa a ejecutor y ejecutor decide que instruciones mandar a la MDK2, cuando ejecutor acaba actualiza la clase juego.
        OHHH JEFE SUPREMO APIADATE DE MI

    Autor: Javir Ortiz Perez-Jaraiz (Lesmus Trompiz)
    Año: Eurobot 2020


"""
import time
# Constantes
VASO_1 = 'A'
X = 0
Y = 1


class robot:
    def __init__(self):
        self.pos = [-1500, -700]
        self.bandera = 0
        self.velocidad = [0, 0]
        self.velocidadangular = [0]


class vasos:
    def __init__(self, estado, posx, posy, color):
        self.estado = estado
        self.pos = [posx, posy]
        self.color = color


class campo:
    def __init__(self):
        self.vaso1 = vasos(True, 7, 5, 'r')
        self.vaso2 = vasos(True, 7, 5, 'r')
        self.vaso3 = vasos(True, 7, 5, 'r')


class game:
    def __init__(self):
        self.tiempo = 0
        self.puntos = 0
        self.robot = robot
        self.campo = campo

        #self.vaso1 = vaso(1, 15, 2, 'r')
        # self.campo


# class planificador:


def planificador(juego):
    if(juego.tiempo <= 100):
        print("Queda tiempo, vamos por la siguiente instruccion")
    else:
        print("Rapido pirate")
    if juego.campo.vaso1.estado:
        print("Vamos a por el vaso 1, hay que salir y girar")
        ejecutor(VASO_1, juego)

    juego.tiempo = juego.tiempo+1


def ejecutor(instruccion, juego):
    if(VASO_1):
        distancia = juego.campo.vaso1.pos-juego.robot.pos
        print(distancia)


def main():
    juego = game
    #vaso1 = vaso(1, 15, 2, 'r')
    while game.tiempo < 140:
        planificador(juego)
        # print(juego.campo.vaso1.color)
        # print(juego.campo)
        time.sleep(0.5)


main()
