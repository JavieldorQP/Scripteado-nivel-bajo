
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
# Vasos (Con definir bien el vaso 1 basta):
VASO_1X = -1200
VASO_1Y = -200
VASO_2X = VASO_1X + 150 
VASO_2Y = VASO_1Y + 120
VASO_3X = VASO_1X
VASO_3Y = VASO_1Y + 800
VASO_4X = VASO_2X
VASO_4Y = VASO_1Y + 490
VASO_5X = VASO_1X + 530
VASO_5Y = VASO_3X + 300
VASO_6X = VASO_1X +650
VASO_6Y = VASO_3Y
VASO_7X = VASO_1X + 800
VASO_7Y = VASO_1Y + 400
VASO_8X = VASO_1X + 970
VASO_8Y = VASO_1Y
VASO_9X = VASO_1X + 705
VASO_9Y = VASO_1Y - 755
VASO_10X = VASO_9X + 60
VASO_10Y = VASO_9Y + 355
VASO_11X = VASO_9X + 330
VASO_11Y = VASO_10Y
VASO_12X = VASO_9X + 390
VASO_12Y = VASO_9Y

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
VASO_3 = 8
VASO_4 = 9
VASO_5 = 10
VASO_6 = 11
VASO_7 = 12
VASO_8 = 13
VASO_S7 = 14
VASO_S8 = 15
DESCARGAR_VASOS = 16
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1
# Lados
códigos = [1,2]
AMARILLO = 1
AZUL = 2
#Función trayectoria correcta para siguiente Vaso

class robot:         # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot:
    def __init__(self, posx, posy, orientacion):
        self.disponible = True
        self.pos = [posx, posy]
        self.velocidad = [0, 0]
        self.velocidadangular = [0]
        self.orientacion = orientacion
        self.tiempo_bloqueado = 0                                  # Característica propia de simulación, es el tiempo que le falta parar estar disponible

class Posavasos:     # Clase que representa toda la información de posavasos:
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)                 
        self.ventosas_ocupada = False                               # No ha cogido ningún vaso


class Parejitas:  # Clase que representa toda la información de parejitas
    def __init__(self, posx, posy, orientacion):
        self.robot = robot(posx, posy, orientacion)  
        self.compuertas = False                                     # Estan abiertas
        self.actuador_manga = False                                 # Actuador para tirar las mangas de viento
        self.bandera = 0
        self.deposito_total = 0                                            # Ha sacado ya la bandera
        self.deposito_vasos_rojo = 0
        self.deposito_vasos_verde = 0

class vasos:         # Almacena la información de la disponibilidad de los vasos
    def __init__(self):
        self.vaso1 = True                                         
        self.vaso2 = True
        self.vaso3 = True
        self.vaso4 = True
        self.vaso5 = True
        self.vaso6 = True
        self.vaso7 = True
        self.vaso8 = True
        self.vaso9 = True
        self.vaso10 = True
        self.vaso11 = True
        self.vaso12 = True
        self.vaso_S1 = True                                         
        self.vaso_S2 = True
        self.vaso_S3 = True
        self.vaso_S4 = True
        self.vaso_S5 = True
        self.vaso_S6 = True
        self.vaso_S7 = True
        self.vaso_S8 = True
        self.vaso_S9 = True
        self.vaso_S10 = True
        self.vaso_S11 = True
        self.vaso_S12 = True

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
        self.vasos = vasos                  # Disponibilidad de los vasos
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
        if(juego.parejitas.robot.disponible):
            print("Parejitas es tu turno")
            if(juego.parejitas.deposito_total == 8):
                print("Nuestro deposito esta lleno")
                ejecutor(BAHIA_SOLTAR, PAREJITAS, juego)
            elif(juego.vasos.vaso1):
                print ("A por un vaso:")
                ejecutor(VASO_4, PAREJITAS, juego)    
            elif(juego.vasos.vaso3):
                print ("A por un vaso:")
                ejecutor(VASO_3, PAREJITAS, juego)
            elif(juego.vasos.vaso5):
                print ("A por un vaso:")
                ejecutor(VASO_5, PAREJITAS, juego)
            elif(juego.vasos.vaso6):
                print ("A por un vaso:")
                ejecutor(VASO_6, PAREJITAS, juego)
            elif(juego.vasos.vasoS7):
                print ("A por un vaso:")
                ejecutor(VASO_S7, PAREJITAS, juego)
            elif(juego.vasos.vaso_S8):
                print ("A por un vaso:")
                ejecutor(VASO_S8, PAREJITAS, juego)
            else:                                                   # Si ya no tiene nada más que hacer que vuelva a casa para asegurar los puntos de anclaje
                print("No tengo que hacer nada")
                ejecutor(CASA, PAREJITAS, juego)

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
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.experimento = False
        juego.puntos = juego.puntos+15
        juego.posavasos.robot.disponible = False
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t

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
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        t = actuadores("S", juego) + t
        juego.puntos = juego.puntos+14
        juego.posavasos.robot.disponible = False
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t
    elif(orden == ESTANTERIAS_CERCA):
        if(juego.lado == AMARILLO):
            Objx = ESTANTERIA_VASOS_4X
            Objy = ESTANTERIA_VASOS_4Y
            Orientacion_final = 0
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_1X
            Objy = ESTANTERIA_VASOS_1Y
            Orientacion_final = 180
        t=simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        t=actuadores("R", juego) + t
        juego.posavasos.robot.disponible = False
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t
        juego.estanterias.estanteria_casa = False

    elif(orden == ESTANTERIAS_NEUTRO_CERCA):
        if(juego.lado == AMARILLO):
            Objx = ESTANTERIA_VASOS_3X
            Objy = ESTANTERIA_VASOS_3Y
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_2X
            Objy = ESTANTERIA_VASOS_2Y
        Orientacion_final = 90
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        t = actuadores("R", juego)
        juego.estanterias.estanteria_neutro_cerca = False
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t
        juego.posavasos.robot.disponible = False

    elif(orden == ACTUALIZAR_BRUJULA):
        Objx = CAMARA_X
        Objy = CAMARA_Y
        Orientacion_final = 90
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        t = camara(juego) + t
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t
        juego.posavasos.robot.disponible = False
    
    elif(orden == VASO_4):
        if(juego.lado == AMARILLO):
            Objx = -VASO_4X
            Objy = VASO_4Y
            Orientacion_final = 45
        elif(juego.lado == AZUL):
            Objx = VASO_4X
            Objy = VASO_4Y
            Orientacion_final = 135
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso4 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False

    elif(orden == VASO_3):
        if(juego.lado == AMARILLO):
            Objx = -VASO_3X
            Objy = VASO_3Y
            Orientacion_final = 135
        elif(juego.lado == AZUL):
            Objx = VASO_3X
            Objy = VASO_3Y
            Orientacion_final = 45
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso3 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False
    elif(orden == VASO_5):
        if(juego.lado == AMARILLO):
            Objx = -VASO_5X
            Objy = VASO_5Y
            Orientacion_final = 240
        elif(juego.lado == AZUL):
            Objx = VASO_5X
            Objy = VASO_5Y
            Orientacion_final = 300
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso5 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False
    elif(orden == VASO_6):
        if(juego.lado == AMARILLO):
            Objx = -VASO_6X
            Objy = VASO_6Y
            Orientacion_final = 210
        elif(juego.lado == AZUL):
            Objx = VASO_6X
            Objy = VASO_6Y
            Orientacion_final = 330
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso6 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False
    elif(orden == VASO_S7):
        if(juego.lado == AMARILLO):
            Objx = VASO_7X
            Objy = VASO_7Y
            Orientacion_final = 250
        elif(juego.lado == AZUL):
            Objx = -VASO_7X
            Objy = VASO_7Y
            Orientacion_final = 290
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso_S7 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False
    elif(orden == VASO_S8):
        if(juego.lado == AMARILLO):
            Objx = VASO_8X
            Objy = VASO_8Y
            Orientacion_final = 270
        elif(juego.lado == AZUL):
            Objx = -VASO_8X
            Objy = VASO_8Y
            Orientacion_final = 270
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso_S8 = False
        juego.parejitas.depostito_total = juego.parejitas.depostito_total + 1 
        juego.parejitas.robot.tiempo_bloqueado = juego.tiempo + t
        juego.parejitas.robot.disponible = False
    elif(orden == DESCARGAR_VASOS):
        #ACTUADORES PAREJITAS
        if(juego.lado == AMARILLO):
            if(juego.vasos.vaso_S10 or juego.vasos.vaso_S11):
                Objx = BAHIA_CENTRAL_AMARILLOX
                Objy = BAHIA_CENTRAL_AMARILLOY
                Orientacion_final = 270
            else:
                Objx = BAHIA_AMARILLOX
                Objy = BAHIA_AMARILLOY
        elif(juego.lado == AZUL):
            if(juego.vasos.vaso_S10 or juego.vasos.vaso_S11):
                Objx = BAHIA_CENTRAL_AZULX
                Objy = BAHIA_CENTRAL_AZULY
                Orientacion_final = 270
        t = simula_movimiento(juego, robot, Objx, Objy, Orientacion_final)
        juego.vasos.vaso_S10 = False
        juego.vasos.vaso_S11 = False
        #Actuador parejitas se abre
        juego.parejitas.depostito_total = 0
        juego.puntos = juego.puntos + 24
        juego.posavasos.robot.tiempo_bloqueado = juego.tiempo + t
        juego.posavasos.robot.disponible = False

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
    
    try:
        entrada = input("")
        lado    = int(valor)
    except ValueError:
        print(f"Este valor {entrada} es convertible a entero!")
    except Exception:
        print("Excepcion no reconocida")
    else:
        if lado in códigos:
            if(lado == 1 ):    # AMARILLO
                print("AMARILLO")
                juego = game(AMARILLO, PUERTO_SUR_AMARILLOX, PUERTO_SUR_AMARILLOY +
                            20, PUERTO_SUR_AMARILLOX, PUERTO_SUR_AMARILLOY-20, 180)

            elif (lado == 2 ): # AZUL
                print("AZUL")
                juego = game(AZUL, PUERTO_SUR_AZULX, PUERTO_SUR_AZULY+20,
                            PUERTO_SUR_AZULX, PUERTO_SUR_AZULY-20, 0)
            else:
                print(f"Confguracion de lado {lado} no reconocida")

            print("Situación inicial:")
            print("Posicion de parejitas")
            print(juego.parejitas.robot.pos[0])
            print(juego.parejitas.robot.pos[1])
            print("Posicion de posavasos")
            print(juego.posavasos.robot.pos[0])
            print(juego.posavasos.robot.pos[1])

            while juego.Activo:                                             # Mientras  el juego este en su trancurso
                planificador(juego)                                            
                juego.tiempo = juego.tiempo+1
                if(juego.tiempo == juego.posavasos.robot.tiempo_bloqueado):
                    juego.posavasos.robot.disponible = True
                    print("Posavasos ha acabado su anterior tarea")
                    time.sleep(5)
                if(juego.tiempo == juego.parejitas.robot.tiempo_bloqueado):
                    juego.parejitas.robot.disponible = True    
                    print("Parejitas ha acabado su anterior tarea")
                    time.sleep(5)

            #Una vez acabado se resume
            print("Partido acabado")
            print("Resumen:")
            print("Ha durando tantos segundos:")
            print(juego.tiempo)
            print("Hemos conseguido tantos puntos:")
            print(juego.puntos)
        else:
            print(f"El código {código} no es un valor válido")
    finally:
        print("programa terminado")

main()
