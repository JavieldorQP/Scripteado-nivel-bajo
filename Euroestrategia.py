
"""
    Estrategia Eurobot 2020:
        Se basa en tres clases que ceden el control la una a la otra, de tal manera que el planificador revisa la clase Juego, decide
        que pasa a ejecutor y ejecutor decide que instruciones mandar a la MDK2, cuando ejecutor acaba actualiza la clase juego.
        OHHH JEFE SUPREMO APIADATE DE MI

    Autor: Javir Ortiz Perez-Jaraiz (Lesmus Trompiz)
    Año: Eurobot 2020


"""


class robot:
    def __init__(self, posx, posy, vx, vy):
        self.pos = [posx, posy]
        self.bandera = 0
        self.velocidad = [vx, vy]


class game:
    def __init__(self, t, posx, posy, vx, vy):
        self.tiempo = [t]
        self.robot = [posx, posy, vx, vy]
        # self.campo


def ejecutor(juego):
    if(juego.tiempo <= 130):
        print("Queda tiempo")


def main():
    juego = game(0, -1500, -700, 0, 0)
    ejecutor(juego)


main()
