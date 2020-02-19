"""
    Estrategia Eurobot 2020:
        Se basa en tres clases que ceden el control la una a la otra, de tal manera que el planificador revisa la clase Juego, decide
        que pasa a ejecutor y ejecutor decide que instruciones mandar a la MDK2, cuando ejecutor acaba actualiza la clase juego.
        OHHH JEFE SUPREMO APIADATE DE MI....
    Autor: Javir Ortiz Perez-Jaraiz (Lesmus Trompiz)
    Año: Eurobot 2020
"""
# Biliotecas:
from Eurocomunicacion_raspy_funciones import envio_instrucciones_actuadores, envio_instrucciones_traccion
from funcionesTyM import instrucciones_giro_avanzo_giro
import time
import random
import math
# Constantes
# Definir vaso 1 como una tupla
#El centro del campo es el centro del código aruco (1500,1250)
#PUNTOS DE PARTIDA:
Partida_Azul_X = -1350  #Como el centro del robot esta aprox de unos 150
Partida_Azul_Y = 500
Partida_Amarilla_X = -Partida_Azul_X
Partida_Amarilla_Y = Partida_Azul_Y

#Activación de experimentos
ACTIVACION_EXPERIMENTOX_AZUL = Partida_Azul_X 
ACTIVACION_EXPERIMENTOY_AZUL = Partida_Azul_Y + 950                     #Teniendo en cuenta el diametro 
ACTIVACION_EXPERIMENTOX_AMARILLO = -ACTIVACION_EXPERIMENTOX_AZUL
ACTIVACION_EXPERIMENTOY_AMARILLO = ACTIVACION_EXPERIMENTOY_AZUL
# Estanterias:
posiciones_estanterias = [
        (Partida_Azul_X, Partida_Azul_Y -250 ),         # Estanteria 1, cerca de casa
        (Partida_Azul_X + 850 , Partida_Azul_Y + 950)   # Estanteria 2, cerca del neutro
        ]
estanterias_lado_azul = [0]
estanterias_lado_amarillo = [0]

# Bahias (ACTUALIZAR CON EL DIAMETRO DEL ROBOT):
posiciones_bahias = [
    (Partida_Azul_X, Partida_Azul_Y + 500),             # Bahia 1, cerca de casa
    (Partida_Azul_X + 1650, Partida_Azul_Y - 450)       # Bahia 2, cerca del neutro
    ]
bahias_lado_azul = [0]
bahias_lado_amarillo = [0]

# Puertos Azules:
PUERTO_SUR_AZULX = Partida_Azul_X 
PUERTO_SUR_AZULY = Partida_Azul_Y - 230
PUERTO_NORTE_AZULX = Partida_Azul_X
PUERTO_NORTE_AZULY = Partida_Azul_Y + 770
# Puertos Amarillos:
PUERTO_SUR_AMARILLOX = -PUERTO_SUR_AZULX
PUERTO_SUR_AMARILLOY = PUERTO_SUR_AZULY
PUERTO_NORTE_AMARILLOX = -PUERTO_NORTE_AZULX
PUERTO_NORTE_AMARILLOY = PUERTO_NORTE_AZULY
#Mangas de viento
# Posición para ver la camara:      #Completamente orientativo
CAMARA_X = 0
CAMARA_Y = 1000
# Instrucciones, dentro de poco lo pasare a un diccionario
ACTIVAR_EXPERIMENTO = 0
BAHIA_SOLTAR = 1
ESTANTERIAS_NEUTRO_CERCA = 2
ESTANTERIAS_CERCA = 3
ESTANTERIAS_NEUTRO_ENEMIGO = 4
ESTANTERIAS_ENEMIGO = 5
ACTUALIZAR_BRUJULA = 6
CASA = 7
VASO_4 = 8
VASO_5 = 9
VASO_6 = 10
VASO_7 = 11
VASO_8 = 12
DESCARGAR_NEUTRO = 13
VASO_10 = 14
VASO_11 = 15
VASO_12 = 16
VASO_3 = 17
MANGA_1 = 18 
MANGA_2 = 19
# ROBOTS
PAREJITAS = 1
POSAVASOS = 2
# Lados
AMARILLO = 1
AZUL = 2
# Vasos:
VASO_1X = -1200
VASO_1Y = +50
posiciones_vasos = [
        (VASO_1X,VASO_1Y),                  # Vaso 1
        (VASO_1X + 150, VASO_1Y + 120 ),    # Vaso 2
        (VASO_1X,VASO_1Y + 800 ),           # Vaso 3
        (VASO_1X + 150, VASO_1Y + 490),     # Vaso 4 
        (VASO_1X + 530, VASO_1Y + 1100 ),   # Vaso 5
        (VASO_1X +650,VASO_1Y + 800),       # Vaso 6
        (VASO_1X + 800, VASO_1Y + 400),     # Vaso 7
        (VASO_1X + 970,VASO_1Y),            # Vaso 8
        (VASO_1X + 705,VASO_1Y - 755),      # Vaso 9
        (VASO_1X + 765, VASO_1Y - 400),     # Vaso 10
        (VASO_1X + 1035 ,VASO_1Y - 400),    # Vaso 11
        (VASO_1X + 1095,VASO_1Y - 755)      # Vaso 12
        ]
vasos_lado_azul = [0]
vasos_lado_amarillo = [0]

#Mangas:
MANGA_1X = 230
MANGA_1Y = 600
posiciones_mangas = [
    (MANGA_1X, MANGA_1Y),           # Manga 1, cerca de la casa
    (MANGA_1X + 405, MANGA_1Y)      # Manga 1, cerca del neutro
    ]
mangas_lado_azul = [0]
mangas_lado_amarillo = [0]



#Clases:
class robot:  # Clase tipo robot donde se almacenan todos los valores insteresantes del propio disponible del robot
    def __init__(self, posx, posy, orientacion):
        self.disponible = True
        self.pos = [posx, posy]
        self.bandera = 0
        self.velocidad = [0, 0]
        self.velocidadangular = [0]
        self.orientacion = orientacion

class Posavasos:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  # Datos por concretar
        self.ventosas_ocupada = False  # No ha cogido ningún vaso

class Parejitas:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  # Datos por concretar
        self.compuertas = False  # Estan abiertas
        self.actuador_manga = False
class vaso:
    def __init__(self,posx ,posy):
        self.pos = (posx, posy)
        self.disponible = True
class manga:
    def __init__(self, posx, posy): 
        self.pos = (posx, posy)
        self.disponible = True

class estanteria():  #
    def __init__(self, posx, posy):
        self.pos = (posx, posy)
        self.disponible = True

class bahia:
    def __init__(self, posx, posy):
        self.pos = (posx, posy)
        self.disponible = True

for numero_vaso in range (1 , 12):
    vasos_lado_azul += [vaso(posiciones_vasos[numero_vaso-1][0],
        posiciones_vasos [numero_vaso-1][1])]
    vasos_lado_amarillo += [vaso(-1*posiciones_vasos[numero_vaso-1][0],
        posiciones_vasos [numero_vaso-1][1])]

for numero_manga in range (1,2):
    mangas_lado_azul += [manga(posiciones_mangas[numero_manga-1][0],
        posiciones_mangas [numero_manga-1][1])]
    mangas_lado_amarillo += [-1*manga(posiciones_mangas [numero_manga-1][0],
        posiciones_mangas [numero_manga-1][1])]

for numero_estanterias in range (1,2):
    estanterias_lado_azul += [estanteria(posiciones_estanterias[numero_estanterias-1][0],
        posiciones_estanterias [numero_estanterias-1][1])]
    estanterias_lado_amarillo += [-1*estanteria(posiciones_estanterias[numero_estanterias-1][0],
        posiciones_estanterias [numero_estanterias-1][1])]

for numero_bahias in range (1,2):
    bahias_lado_azul += [bahia(posiciones_bahias[numero_bahias-1][0],
        posiciones_bahias [numero_bahias-1][1])]
    bahias_lado_amarillo += [-1*bahia(posiciones_bahias[numero_bahias-1][0],
        posiciones_bahias [numero_bahias-1][1])]
class objetivo:
    def __init__(self,posx,posy,orientacion):
        self.pos = [posx,posy]
        self.orientacion = orientacion    

class campo:
    def __init__(self):
        self.vasos_lado_azul = vasos_lado_azul
        self.vasos_lado_amarillo = vasos_lado_amarillo
        self.mangas_lado_azul = mangas_lado_azul
        self.mangas_lado_amarillo = mangas_lado_amarillo
        self.estanterias_lado_azul = estanterias_lado_azul
        self.estanterias_lado_amarillo = estanterias_lado_amarillo
        self.bahias_lado_azul = bahias_lado_azul
        self.bahia_lado_amarillo = bahias_lado_amarillo
        self.experimento = True
        self.brujula = "D"

class game:  # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, lado, pos_inicio_parejitasx, pos_inicio_parejitasy, pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial):
        self.activo = True
        self.tiempo = 0
        self.puntos = 0
        self.lado = lado
        self.posavasos = Posavasos(
            pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial)
        self.parejitas = Parejitas(
            pos_inicio_parejitasx, pos_inicio_parejitasy, orientacion_inicial)
        self.campo = campo
def moverse_giro_recta(robot, objetivo):
    """
        Acción de moverse del robot hacia unas coordenadas objetivo.
    """
    instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(
            robot.pos[0], 
            robot.pos[1], 
            objetivo.pos[0], 
            objetivo.pos[1],
            robot.orientacion,
            objetivo.orientacion)
    envio_instrucciones_traccion (instruccion_giro1,instruccion_distancia,"0G+000")

def moverse_giro_recta_giro(robot, objetivo):
    """
        Acción de moverse del robot hacia unas coordenadas objetivo y que acabe con un un ángulo final
    """
    instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(
            robot.pos[0], 
            robot.pos[1], 
            objetivo.pos[0], 
            objetivo.pos[1],
            robot.orientacion,
            objetivo.orientacion)
    pub_instruccion.publish(instruccion_giro1)
    pub_instruccion.publish(instruccion_distancia)
    pub_instruccion.publish(instruccion_giro2)
def coger_vasos_estanteria(game):
    """
        Baja el brazo y activa las ventosas
    """
    print("Cojo los vasos")
    print("No hace nada de momento")
    game.posavasos.ventosas_ocupada = True

def dejar_vasos_estanteria(game):
    """
        Dejar el brazo y desactiva las ventosas
    """
    print("Dejo los vasos")
    print("No hace nada de momento")
    game.posavasos.ventosas_ocupada = False

def golpear_experimento():
    """
        Saca el brazo y activa el experimento
    """
    print("Golpeo el experimento")
    print("No hace nada de momento")

def camara_brujula(game):
    game.campo.brujula = random.choice(["N", "S"])

def golpear_manga(Parjitas,manga):
    """
        Saca el brazo y tira la manga
    """
    print("Saco el brazo")
    #envio_instrucciones_actuadores()
    print("No hace nada de momento")
    Parejitas.actuador_manga = True
    manga.disponible = False
    envio_instrucciones_traccion(0,100,0)
    print(("Recojo el brazo"))

acciones = {
    0 : moverse_giro_recta,
    1 : moverse_giro_recta_giro,
    2 : coger_vasos_estanteria,
    3 : dejar_vasos_estanteria,
    4 : golpear_experimento,
    5 : camara_brujula,
    6 : golpear_manga
}

def activar_experimento(game, robot):         
    """
        Esta rutina recibe juego y un robot (POSAVASOS)
        y lo que hace es desplazar el robot hacia la zona de 
        activación del experimento, saca el actuador y lo activa.
    """    
    
    if(game.lado == AZUL):
        objetivox = ACTIVACION_EXPERIMENTOX_AZUL
        objetivoy = ACTIVACION_EXPERIMENTOY_AZUL
    
    elif(game.lado == AMARILLO):
        objetivox = ACTIVACION_EXPERIMENTOX_AMARILLO
        objetivoy = ACTIVACION_EXPERIMENTOY_AMARILLO
    orientacion_final = 90
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)
    acciones[4]()
    game.campo.experimento = False

def recoger_estanteria_neutro(game, robot):         
    """
        Esta rutina recibe juego y un robot (POSAVASOS)
        y lo que hace es desplazar el robot hacia la zona de 
        la estanteria del neutro, saca el actuador y recoge los
        vasos.
    """    
    if(game.lado == AZUL):
        objetivox = game.campo.estanterias_lado_azul[1].pos[0]
        objetivoy = game.campo.estanterias_lado_azul[1].pos[1]
    elif(game.lado == AMARILLO):
        objetivox = game.campo.estanterias_lado_amarillo[1].pos[0]
        objetivoy = game.campo.estanterias_lado_amarillo[1].pos[1]
    orientacion_final = 90
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)
    acciones[2](game)
    if(game.lado == AZUL):
        game.estanterias_lado_azul[1].estado = False
    elif(game.lado == AMARILLO):
        game.estanterias_lado_amarillo[1].estado = False
    

def recoger_estanteria_casa(game, robot):         
    """
        Esta rutina recibe juego y un robot (POSAVASOS)
        y lo que hace es desplazar el robot hacia la zona de 
        la estanteria de casa, saca el actuador y recoge los
        vasos.
    """    
    if(game.lado == AZUL):
        objetivox = game.campo.estanterias_lado_azul[0].pos[0]
        objetivoy = game.campo.estanterias_lado_azul[0].pos[1]
        orientacion_final = 180

    elif(game.lado == AMARILLO):
        objetivox = game.campo.estanterias_lado_amarillo[0].pos[0]
        objetivoy = game.campo.estanterias_lado_amarillo[0].pos[1]
        orientacion_final = 0

    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)
    acciones[2](game)
    if(game.lado == AZUL):
        game.estanterias_lado_azul[0].estado = False
    elif(game.lado == AMARILLO):
        game.estanterias_lado_amarillo[0].estado = False
    
def soltar_vasos_ventosas(game,robot):
    """
        Esta rutina recibe juego y un robot (POSAVASOS)
        y lo que hace es desplazar el robot hacia la bahia de cerca
        de casa, saca el actuador y deja los vasos.
    """    
    if(game.lado == AZUL):
        objetivox = game.campo.estanterias_lado_azul[0].pos[0]
        objetivoy = game.campo.estanterias_lado_azul[0].pos[1]
        orientacion_final = 180
    elif(game.lado == AMARILLO):
        objetivox = game.campo.estanterias_lado_amarillo[0].pos[0]
        objetivoy = game.campo.estanterias_lado_amarillo[0].pos[1]
        orientacion_final = 0
    else:
        print ("Lado no definido, ERROR")
        raise Exception
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)
    acciones[3](game)

def mirar_brujula(game,robot):
    """
        Esta rutina recibe juego y un robot (cualquiera)
        y lo que hace es desplazar el robot hacia una posición
        central y examina el estado de la brúluja .
    """    
    objetivox = CAMARA_X
    objetivoy = CAMARA_Y
    orientacion_final = 90
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)
    acciones[5](game)
    
def aparcar(game,robot):
    """
        Esta rutina recibe juego y un robot (cualquiera)
        y lo que hace es desplazar el robot hacia  uno de 
        los puertos .
    """
    if(game.lado == AZUL):
        if(game.campo.brujula == "N"):
            objetivox = PUERTO_NORTE_AZULX
            objetivoy = PUERTO_NORTE_AZULY
        else:
            objetivox = PUERTO_SUR_AZULX
            objetivoy = PUERTO_SUR_AZULY
        orientacion_final = 180
    elif(game.lado == AMARILLO):
        if(game.campo.brujula == "N"):
            objetivox = PUERTO_NORTE_AMARILLOX
            objetivoy = PUERTO_NORTE_AMARILLOY
        else:
            objetivox = PUERTO_SUR_AMARILLOX
            objetivoy = PUERTO_SUR_AMARILLOY
        orientacion_final = 0
    else:
        print ("Lado no definido, ERROR")
        raise Exception
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[1](robot, objetivo_actual)

def tirar_manga_neutro(game,robot):
    """
        Se le pasa a la función el estado del juego y un Robot (Parejitas),
        desplaza el robot a la manga del neutro y la tira.
    """
    if(game.lado == AZUL):
        objetivox = game.campo.mangas_lado_azul[1].pos[0]
        objetivoy = game.campo.mangas_lado_azul[1].pos[1]
        orientacion_final = 0
        objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
        acciones[1](robot, objetivo_actual)
        acciones[6](game.parejitas, mangas_lado_azul[1])

    elif(game.lado == AMARILLO):
        objetivox = game.campo.mangas_lado_amarillo[1].pos[0]
        objetivoy = game.campo.mangas_lado_amarillo[1].pos[1]
        orientacion_final = 180
        objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
        acciones[1](robot, objetivo_actual)
        acciones[6](game.parejitas, mangas_lado_amarillo[1])

def tirar_manga_casa(game,robot):
    """
        Se le pasa a la función el estado del juego y un Robot (Parejitas),
        desplaza el robot a la manga del neutro y la tira.
    """
    if(game.lado == AZUL):
        objetivox = game.campo.mangas_lado_azul[0].pos[0]
        objetivoy = game.campo.mangas_lado_azul[0].pos[1]
        orientacion_final = 0
        objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
        acciones[1](robot, objetivo_actual)
        acciones[6](game.parejitas, mangas_lado_azul[0])

    elif(game.lado == AMARILLO):
        objetivox = game.campo.mangas_lado_amarillo[0].pos[0]
        objetivoy = game.campo.mangas_lado_amarillo[0].pos[1]
        orientacion_final = 180
        objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
        acciones[1](robot, objetivo_actual)
        acciones[6](game.parejitas, mangas_lado_amarillo[0])

def comer_vasos(game,vaso):
    """
        Se le pasa a la función el estado del juego y un Robot (Parejitas), con
        esto calcula y maniobra para introducir un vaso en el interior de 
        parejitas.
    """
    print("Hasta que Cesucio no me pase el plano de parejitas esto solo se mueve a la posición del vaso")
    objetivox = vaso.pos[0]
    objetivoy = vaso.pos[1]
    orientacion_final = 0
    objetivo_actual = objetivo(objetivox, objetivoy, orientacion_final)
    acciones[0](robot, objetivo_actual)
    vaso.disponible = False

   
    

rutinas = {
    1 : activar_experimento,
    2 : recoger_estanteria_neutro,
    3 : recoger_estanteria_casa,
    4 : soltar_vasos_ventosas,
    5 : mirar_brujula,
    6 : aparcar,
    7 : tirar_manga_neutro,
    8 : tirar_manga_casa,
    9 : comer_vasos
}

def planificador(juego):        # Revisa la clase juego y decide que hacer.

    if(juego.tiempo <= 90):     # Hay tiempo        
        if(juego.posavasos.robot.disponible):  # Posavasos no esta ocupado
            
            print(f"Posción actual {juego.posavasos.robot.pos[0]}, {juego.posavasos.robot.pos[1]}")
            print(f"Angulo actual {juego.posavasos.robot.orientacion}")
            
            if(juego.campo.experimento):
                print("Activo el experimento")
                rutinas[1](juego,juego.posavasos)
                juego.puntos = juego.puntos+15
            elif(juego.posavasos.ventosas_ocupada):
                print("Vamos a la bahia a descargar los vasos")
                rutinas[4](juego,juego.posavasos)
                juego.puntos = juego.puntos+14
            elif (juego.campo.estanterias_lado_azul[1].estado and juego.campo.estanterias_lado_amarillo[1].estado ):
                print("Voy a la estanteria del neutro")
                rutinas[2](juego,juego.posavasos)
            elif (juego.campo.estanterias_lado_azul[0].estado and juego.campo.estanterias_lado_amarillo[0].estado):
                print("Voy a la estanteria de mi lado")
                rutinas[3](juego,juego.posavasos)
            elif (juego.campo.brujula == 'N'):
                print("Voy a mirar la brujula")
                rutinas[5](juego,juego.posavasos)
            else:
                print("No tengo que hacer nada")
                rutinas[6](juego,juego.posavasos)
                juego.puntos = juego.puntos+10
            
        if(juego.parejitas.robot.disponible):
            
            print(f"Posción actual {juego.posavasos.robot.pos[0]}, {juego.posavasos.robot.pos[1]}")
            print(f"Angulo actual {juego.posavasos.robot.orientacion}")
            
            if(juego.campo.vasos_lado_amarillo[9].disponible and juego.campo.vasos_lado_azul[9].disponible):
                if(juego.lado == AZUL):
                    if (juego.campo.vasos_lado_azul[4].disponible):
                        print("Vamos a por el vaso 4")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[4])
                    elif(juego.campo.vasos_lado_azul[5].disponible):
                        print("Vamos a por el vaso 5")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[5])
                    elif(juego.campo.vasos_lado_azul[6].disponible):
                        print("Vamos a por el vaso 6")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[6])
                    elif(juego.campo.vasos_lado_amarillo[7].disponible):
                        print("Vamos a por el vaso 7")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[7])
                    elif(juego.campo.vasos_lado_amarillo[8].disponible):
                        print("Vamos a por el vaso 8")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[8])
                    elif(juego.campo.vasos_lado_amarillo[9].disponible):
                        print("Vamos a por el vaso 9 y a descargar")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[9])
                elif(juego.lado == AMARILLO):
                    if (juego.campo.vasos_lado_amarillo[4].disponible):
                        print("Vamos a por el vaso 4")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[4])
                    elif(juego.campo.vasos_lado_amarillo[5].disponible):
                        print("Vamos a por el vaso 5")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[5])
                    elif(juego.campo.vasos_lado_amarillo[6].disponible):
                        print("Vamos a por el vaso 6")
                        rutinas[9](juego,juego.campo.vasos_lado_amarillo[6])
                    elif(juego.campo.vasos_lado_azul[7].disponible):
                        print("Vamos a por el vaso 7")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[7])
                    elif(juego.campo.vasos_lado_azul[8].disponible):
                        print("Vamos a por el vaso 8")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[8])
                    elif(juego.campo.vasos_lado_azul[9].disponible):
                        print("Vamos a por el vaso 9 y a descargar")
                        rutinas[9](juego,juego.campo.vasos_lado_azul[9])
                        juego.puntos = juego.puntos + 10    
                else:
                    print ("Lado no definido, ERROR")
                    raise Exception
            elif(juego.campo.mangas_lado_azul[1].disponible or juego.campo.mangas_lado_azul[1].disponible):
                if(juego.lado == AZUL):
                    orientacion_final = 0
                    distancia_manga_neutro = math.sqrt(
                        (juego.parejitas.robot.pos[0]-juego.campo.mangas_lado_azul[0].pos[0])**2
                        +
                        (juego.parejitas.robot.pos[1]-juego.campo.mangas_lado_azul[0].pos[1])**2
                        )
                    distancia_manga_casa = math.sqrt(
                        (juego.parejitas.robot.pos[0]-juego.campo.mangas_lado_azul[0].pos[0])**2
                        +
                        (juego.parejitas.robot.pos[1]-juego.campo.mangas_lado_azul[0].pos[1])**2
                        )
                    if(distancia_manga_casa >= distancia_manga_neutro):
                        rutinas[7](juego,juego.parejitas)
                        rutinas[8] (juego,juego.parejitas)
                    else:
                        rutinas[8](juego,juego.parejitas)
                        rutinas[7](juego,juego.parejitas)
                elif(juego.lado == AMARILLO):
                    orientacion_final = 180
                    distancia_manga_neutro = math.sqrt(
                        (juego.parejitas.robot.pos[0]-juego.campo.mangas_lado_amarillo[0].pos[0])**2
                        +
                        (juego.parejitas.robot.pos[1]-juego.campo.mangas_lado_amarillo[0].pos[1])**2
                        )
                    distancia_manga_casa = math.sqrt(
                        (juego.parejitas.robot.pos[0]-juego.campo.mangas_lado_amarillo[0].pos[0])**2
                        +
                        (juego.parejitas.robot.pos[1]-juego.campo.mangas_lado_amarillo[0].pos[1])**2
                        )
                    if(distancia_manga_casa >= distancia_manga_neutro):
                        rutinas[7](juego,juego.parejitas)
                        rutinas[8] (juego,juego.parejitas)
                    else:
                        rutinas[8](juego,juego.parejitas)
                        rutinas[7](juego,juego.parejitas)
                else:
                    print ("Lado no definido, ERROR")
                    raise Exception

            elif (juego.campo.brujula == 'N'):
                print("Voy a mirar la brujula")
                rutinas[5](juego,juego.parejitas)
            
            else:
                print("No tengo que hacer nada")
                rutinas[6](juego,juego.parejitas)
                juego.puntos = juego.puntos+10
        
    else:
        print("Rapido pirate")
        rutinas[6](juego, juego.robot.posavasos)
        rutinas[6](juego, juego.robot.parejitas)
        juego.puntos = juego.puntos+10
        juego.activo = False
    
def main():
    print("Este script es una demo de la estrategia de cada uno de los robots")
    #print("Seleccione el robot con el que simular la demo (1 para posavasos y 2 para parejitas):")
    try:
        lado = int(input("Selecciona el lado (1 para amarillo y 2 para el Azul ):"))
    except ValueError:
        print("Dato no reconocido")
    else:
        if(lado == AMARILLO):
            print("AMARILLO")
            juego = game(AMARILLO, Partida_Amarilla_X , Partida_Amarilla_Y
                        , Partida_Amarilla_X, Partida_Amarilla_Y, 180)
        elif(lado == AZUL):
            print("AZUL")
            juego = game(AZUL, Partida_Azul_X , Partida_Azul_Y ,
                        Partida_Azul_X, Partida_Azul_Y, 0)                
        else:
            print("Error seleccionando lado cargue de nuevo el programa")
            raise Exception
        try:
            dispositivo = int(input("Seleccione el robot con el que simular la demo (1 para parejitas y 2 para posavasos):"))
        except ValueError:
            print("Dato no reconocido")
        else:
            if(dispositivo == POSAVASOS):
                print("Has escogido a posavasos")
                juego.parejitas.robot.disponible = False
            elif(dispositivo == PAREJITAS):
                print("Has escogido a parejitas")
                juego.posavasos.robot.disponible = False
            else:
                print("Número incorrecto, inicie de nuevo el programa")
                raise Exception
            print(f"El juego va a comenzar en {5}")
            time.sleep(1)
            print(f"El juego va a comenzar en {4}")
            time.sleep(1)
            print(f"El juego va a comenzar en {3}")
            time.sleep(1)
            print(f"El juego va a comenzar en {2}")
            time.sleep(1)
            print(f"El juego va a comenzar en {1}")
            time.sleep(1)
            while juego.activo:
                planificador(juego)
                print("Vamos por el segundo de partido:")
                print(juego.tiempo)

    print("Partido acabado")
    print("Resumen:")
    print("Ha durando tantos segundos:")
    print(juego.tiempo)
    print("Hemos conseguido tantos puntos:")
    print(juego.puntos)

main()