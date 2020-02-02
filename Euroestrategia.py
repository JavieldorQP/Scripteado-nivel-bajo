
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
from Simulo_robot import simula_movimiento, actuadores, camara

# Constantes:
# Vasos:
VASO_1X = 0
VASO_1Y = 0
# Experimento
ACTIVACION_EXPERIMENTOX_AZUL = -1200
ACTIVACION_EXPERIMENTOY_AZUL = 800
ACTIVACION_EXPERIMENTOX_AMARILLO = -ACTIVACION_EXPERIMENTOX_AZUL
ACTIVACION_EXPERIMENTOY_AMARILLO = ACTIVACION_EXPERIMENTOY_AZUL
# Estanterias:
ESTANTERIA_VASOS_1X = -1500
ESTANTERIA_VASOS_1Y = -800
ESTANTERIA_VASOS_2X = -575
ESTANTERIA_VASOS_2Y = 900
ESTANTERIA_VASOS_3X = -ESTANTERIA_VASOS_2X
ESTANTERIA_VASOS_3Y = ESTANTERIA_VASOS_2Y
ESTANTERIA_VASOS_4X = -ESTANTERIA_VASOS_1X
ESTANTERIA_VASOS_4Y = ESTANTERIA_VASOS_1Y
# Bahia:
BAHIA_CENTRAL_AMARILLOX = -400  # Tener en cuenta que es la del otro lado
BAHIA_CENTRAL_AMARILLOY = -650
BAHIA_CENTRAL_AZULX = -BAHIA_CENTRAL_AMARILLOX
BAHIA_CENTRAL_AZULY = BAHIA_CENTRAL_AMARILLOY
BAHIA_AZULX = -1300
BAHIA_AZULY = 200
BAHIA_AMARILLOX = -BAHIA_AZULX
BAHIA_AMARILLOY = BAHIA_AZULY
# Puertos Azules:
PUERTO_SUR_AZULX = -1200
PUERTO_SUR_AZULY = -300
PUERTO_NORTE_AZULX = PUERTO_SUR_AZULX
PUERTO_NORTE_AZULY = 550
# Puertos Amarillos:
PUERTO_SUR_AMARILLOX = -PUERTO_SUR_AZULX
PUERTO_SUR_AMARILLOY = PUERTO_SUR_AZULY
PUERTO_NORTE_AMARILLOX = -PUERTO_NORTE_AZULX
PUERTO_NORTE_AMARILLOY = PUERTO_NORTE_AZULY
# Posición para ver con la camara:
CAMARA_X = 0
CAMARA_Y = 800
# Instrucciones
ACTIVAR_EXPERIMENTO = 0
BAHIA_SOLTAR = 1
ESTANTERIAS_NEUTRO_CERCA = 2
ESTANTERIAS_CERCA = 3
ESTANTERIAS_NEUTRO_ENEMIGO = 4
ESTANTERIAS_ENEMIGO = 5
ACTUALIZAR_BRUJULA = 6
CASA = 7
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1
# Lados
AMARILLO = 1
AZUL = 2


class robot:         # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot:
    def __init__(self, posx, posy, orientacion):
        self.disponible = True
        self.pos = [posx, posy]
        self.velocidad = [0, 0]
        self.velocidadangular = [0]
        self.orientacion = orientacion


class Posavasos:     # Clase que representa toda la información de posavasos:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)                 
        self.ventosas_ocupada = False                               # No ha cogido ningún vaso


class Parejitas:  # Clase que representa toda la información de parejitas
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  
        self.compuertas = False                                     # Estan abiertas
        self.actuador_manga = False                                 # Actuador para tirar las mangas de viento
        self.bandera = 0                                            # Ha sacado ya la bandera


class vasos:         # Almacena la información de la disponibilidad de los vasos
    def __init__(self, vaso1, vaso2, vaso3, vaso4):
        self.vaso1 = vaso1                                          
        self.vaso2 = vaso2
        self.vaso3 = vaso4
        self.vaso4 = vaso4


class estanterias:   # Almacena la información de las estanterias y si esta disponible
    def __init__(self, estanteria_casa, estanteria_neutro_cerca, estanteria_neutro_lejos, estanteria_enemigo):
        self.estanteria_casa = estanteria_casa                      # Disponibilidad de mi lado del mapa
        self.estanteria_neutro_cerca = estanteria_neutro_cerca      # Disponibilidad de la estanteria más cercana a casa del neutro
        self.estanteria_neutro_lejos = estanteria_neutro_lejos      # Disponibilidad de la estanteria más cercana a casa del neutro
        self.estanteria_enemigo = estanteria_enemigo                # Disponibilidad de la estanteria del enemigo


class game:          # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, lado, pos_inicio_parejitasx, pos_inicio_parejitasy, pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial):
        self.Activo = True                                          # El partido esta activo
        self.tiempo = 0                                             # El segundo de partido por el que vamos
        self.puntos = 0                                             # Los puntos que tenemos
        self.lado = lado                                            # Nuestro lado, puede ser el azul o el amarillo
        self.posavasos = Posavasos(                                 # La clase posavasos
            pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial)
        self.parejitas = Parejitas(                                 # La clase parejtas
            pos_inicio_parejitasx, pos_inicio_parejitasy, orientacion_inicial)
        self.vasos = vasos(True, True, True, True)                  # Disponibilidad de los vasos
        self.estanterias = estanterias(True, True, True, True)      # Disponibilidad de las estanterias
        self.experimento = True                                     # Se puede activar el experimento
        self.brujula = "D"                                          # Color de la brujula desconocida



def planificador(juego):
    """
    Planificador:
        Esta función se encarga de mirar el estado de juego y en función de eso 
        decide que debe de hacer el robot, una vez lo decide le cede el control
        a ejecutor.
    """
    if(juego.tiempo <= 90):                                         # Si hay tiempo pensamos en que se puede hacer
        if(juego.posavasos.robot.disponible):                       # Si posavasos no esta ocupado busca que orden mandarle
            if(juego.experimento):                                  # Activar el experimento es prioritario y coincide con la ruta de posavasos
                print("Activo el experimento")
                ejecutor(ACTIVAR_EXPERIMENTO, POSAVASOS, juego)           
            elif(juego.posavasos.ventosas_ocupada):                 # Si tienes el acutador ocupado lo prioritario es soltar los vasos en nuestra bahia
                print("Vamos a la bahia a descargar los vasos")
                ejecutor(BAHIA_SOLTAR, POSAVASOS, juego)
            elif (juego.estanterias.estanteria_neutro_cerca):       # Si aun estan disponibles los vasos en el neutro lo recomendable es ir a por ello
                print("Voy a la estanteria del medio")
                ejecutor(ESTANTERIAS_NEUTRO_CERCA, POSAVASOS, juego)
            elif (juego.estanterias.estanteria_casa):
                print("Voy a la estanteria de mi lado")             # Una vez has recogido el resto de estanterias, hay que ir a la de nuestra casa
                ejecutor(ESTANTERIAS_CERCA, POSAVASOS, juego)
            elif (juego.brujula == 'N'):                            # Si no es conocida la posición de la brújula y no tienes nada más que hacer ve a revisarla
                print("Voy a mirar la brujula")
                ejecutor(ACTUALIZAR_BRUJULA, POSAVASOS, juego)
            else:                                                   # Si ya no tiene nada más que hacer que vuelva a casa para asegurar los puntos de anclaje
                print("No tengo que hacer nada")
                ejecutor(CASA, POSAVASOS, juego)
    # if(juego.parejitas.disponible):
    #    print("Parejitas es tu turno")
    else:                                                           # No queda tiempo para hacer nada más ambos robots se tienen que ir a casa para puntuar los puntos de anclaje
        print("Rapido pirate")
        ejecutor(CASA, POSAVASOS, juego)
        ejecutor(CASA, PAREJITAS, juego)


def ejecutor(orden, robot, juego):
    """
    Ejecutor:
        Esta función recibe una orden de planificador y se la manda a la MDK2, una vez la 
        MDK2 ha realizado la orden, ejecutor se encarga de actualizar el estado de juego
        y espera a que planificador le mande otra orden.
        Para simular la actuación de la MDK2 se crean distintas funciones que estan en
        la biblioteca Simulo_robot.

    """

    if(orden == ACTIVAR_EXPERIMENTO):
        if(juego.lado == AMARILLO):
            print("Activo faro amarillo")
            Objx = ACTIVACION_EXPERIMENTOX_AMARILLO
            Objy = ACTIVACION_EXPERIMENTOY_AMARILLO

        elif(juego.lado == AZUL):
            print("Activo faro azul")
            Objx = ACTIVACION_EXPERIMENTOX_AZUL
            Objy = ACTIVACION_EXPERIMENTOY_AZUL

        Orientacion_final = 90
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.experimento = False
        juego.puntos = juego.puntos+15
    elif(orden == BAHIA_SOLTAR):
        if(juego.lado == AMARILLO):
            Objx = BAHIA_AMARILLOX
            Objy = BAHIA_AMARILLOY
            Orientacion_final = 0
        elif(juego.lado == AZUL):
            Objx = BAHIA_AZULX
            Objy = BAHIA_AZULY
            Orientacion_final = 180
        # Publicaria en un topic
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        actuadores("S", juego)

        juego.puntos = juego.puntos+14
    elif(orden == ESTANTERIAS_CERCA):
        if(juego.lado == AMARILLO):
            print("Holi holi amarillo")
            print(juego.posavasos.robot.pos[0])
            print(juego.posavasos.robot.pos[1])
            print(juego.posavasos.robot.orientacion)
            Objx = ESTANTERIA_VASOS_4X
            Objy = ESTANTERIA_VASOS_4Y
            Orientacion_final = 0
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_1X
            Objy = ESTANTERIA_VASOS_1Y
            print("Holi holi azul")
            print(juego.posavasos.robot.pos[0])
            print(juego.posavasos.robot.pos[1])
            print(juego.posavasos.robot.orientacion)
            Orientacion_final = 180
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        actuadores("R", juego)
        juego.estanterias.estanteria_casa = False
    elif(orden == ESTANTERIAS_NEUTRO_CERCA):
        if(juego.lado == AMARILLO):
            Objx = ESTANTERIA_VASOS_3X
            Objy = ESTANTERIA_VASOS_3Y
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_2X
            Objy = ESTANTERIA_VASOS_2Y
        Orientacion_final = 90
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        actuadores("R", juego)
        juego.estanterias.estanteria_neutro_cerca = False
    elif(orden == ACTUALIZAR_BRUJULA):
        Objx = CAMARA_X
        Objy = CAMARA_Y
        Orientacion_final = 90
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        camara(juego)
        juego.brujula
    elif(orden == CASA):
        if(juego.lado == AMARILLO):
            if(juego.brujula == 'N'):
                Objx = PUERTO_NORTE_AMARILLOX
                Objy = PUERTO_NORTE_AMARILLOY
            else:
                Objx = PUERTO_SUR_AMARILLOX
                Objy = PUERTO_SUR_AMARILLOY
            Orientacion_final = 0
        elif(juego.lado == AZUL):
            if(juego.brujula == 'N'):
                Objx = PUERTO_NORTE_AZULX
                Objy = PUERTO_NORTE_AZULY
            else:
                Objx = PUERTO_SUR_AZULX
                Objy = PUERTO_SUR_AZULY
            Orientacion_final = 180
        simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.puntos = juego.puntos+10
        juego.Activo = False


def main():
    #Selección del lado y configuración en función del valor escaneado.
    print("Selecciona el lado (1 para amarillo y 2 para el Azul ):")             
    lado = int(input())
    if(lado == AMARILLO):
        print("AMARILLO")
        juego = game(AMARILLO, PUERTO_SUR_AMARILLOX, PUERTO_SUR_AMARILLOY +
                     20, PUERTO_SUR_AMARILLOX, PUERTO_SUR_AMARILLOY-20, 180)

    else:
        print("AZUL")
        juego = game(AZUL, PUERTO_SUR_AZULX, PUERTO_SUR_AZULY+20,
                     PUERTO_SUR_AZULX, PUERTO_SUR_AZULY-20, 0)
    print("Situación inicial:")
    print("Posicion de parejitas")
    print(juego.parejitas.robot.pos[0])
    print(juego.parejitas.robot.pos[1])
    print("Posicion de posavasos")
    print(juego.posavasos.robot.pos[0])
    print(juego.posavasos.robot.pos[1])

    while juego.Activo:                                             # Mientras  el juego este en su trancurso
        planificador(juego)                                         
        print("Vamos por el segundo de partido:")       
        print(juego.tiempo)
        
    #Una vez acabado se resume
    print("Partido acabado")
    print("Resumen:")
    print("Ha durando tantos segundos:")
    print(juego.tiempo)
    print("Hemos conseguido tantos puntos:")
    print(juego.puntos)


main()
