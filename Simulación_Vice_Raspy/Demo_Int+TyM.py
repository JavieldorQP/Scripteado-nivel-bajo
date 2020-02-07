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
ACTIVACION_EXPERIMENTOY_AZUL = Partida_Azul_Y + 1070 
ACTIVACION_EXPERIMENTOX_AMARILLO = -ACTIVACION_EXPERIMENTOX_AZUL
ACTIVACION_EXPERIMENTOY_AMARILLO = ACTIVACION_EXPERIMENTOY_AZUL
# Estanterias:
ESTANTERIA_VASOS_1X = Partida_Azul_X 
ESTANTERIA_VASOS_1Y = Partida_Azul_Y -250 
ESTANTERIA_VASOS_2X = Partida_Azul_X + 850
ESTANTERIA_VASOS_2Y =  ACTIVACION_EXPERIMENTOY_AZUL
ESTANTERIA_VASOS_3X = -ESTANTERIA_VASOS_2X
ESTANTERIA_VASOS_3Y = ESTANTERIA_VASOS_2Y
ESTANTERIA_VASOS_4X = -ESTANTERIA_VASOS_1X
ESTANTERIA_VASOS_4Y = ESTANTERIA_VASOS_1Y
# Bahias laterales:
BAHIA_AZULX = Partida_Azul_X
BAHIA_AZULY = Partida_Azul_Y + 500
BAHIA_AMARILLOX = -BAHIA_AZULX
BAHIA_AMARILLOY = BAHIA_AZULY
# Bahias centrales:
BAHIA_CENTRAL_AZULX = Partida_Azul_X + 1650
BAHIA_CENTRAL_AZULY = Partida_Azul_Y - 450
BAHIA_CENTRAL_AMARILLOX = -BAHIA_CENTRAL_AZULX
BAHIA_CENTRAL_AMARILLOY = -BAHIA_CENTRAL_AZULY

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

# Posición para ver la camara:      #Completamente orientativo
CAMARA_X = 0
CAMARA_Y = 1000
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
PAREJITAS = 1
POSAVASOS = 2
# Lados
AMARILLO = 1
AZUL = 2
#Vaso sobre el que se definen los demas:
VASO_1X = -1200
VASO_1Y = +50

class robot:  # Clase tipo robot donde se almacenan todos los valores insteresantes del propio estado del robot
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
        self.actuador_banda = False

posiciones_vasos = [(VASO_1X,VASO_1Y),(VASO_1X + 150, VASO_1Y + 120 ),
(VASO_1X,VASO_1Y + 800 ) ,(VASO_1X + 150, VASO_1Y + 490),
(VASO_1X + 530, VASO_1Y + 1100 ),(VASO_1X +650,VASO_1Y + 800),
(VASO_1X + 800, VASO_1Y + 400),(VASO_1X + 970,VASO_1Y),
(VASO_1X + 705,VASO_1Y - 755),(VASO_1X + 765, VASO_1Y - 400),
(VASO_1X + 1035 ,VASO_1Y - 400),(VASO_1X + 1095,VASO_1Y - 755)]

class vaso:
    def __init__(self,posx ,posy):
        self.pos = (posx, posy)
        self.estado = True

vasos =  []
for numero_vaso in (1,12):
    vasos +=  [vaso(posiciones_vasos [numero_vaso-1][0],posiciones_vasos [numero_vaso-1][1])]

class estanterias():  #
    def __init__(self, estanteria_casa, estanteria_neutro_cerca, estanteria3, estanteria4):
        self.estanteria_casa = estanteria_casa
        self.estanteria_neutro_cerca = estanteria_neutro_cerca
        self.estanteria3 = estanteria3
        self.estanteria4 = estanteria4


class game:  # Clase tipo game, donde se almacenan toda la información del partido y con un vistazo obtienes toda la información
    def __init__(self, lado, pos_inicio_parejitasx, pos_inicio_parejitasy, pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial):
        self.Activo = True
        self.tiempo = 0
        self.puntos = 0
        self.lado = lado
        self.posavasos = Posavasos(
            pos_inicio_posavasosx, pos_inicio_posavasosy, orientacion_inicial)
        self.parejitas = Parejitas(
            pos_inicio_parejitasx, pos_inicio_parejitasy, orientacion_inicial)
        #self.vasos = vasos(True, True, True, True)
        self.estanterias = estanterias(True, True, True, True)
        self.experimento = True
        self.brujula = "D"


# Revisa el estado de la clase juego y decide que hacer, hecho esto cede el contro a ejecutor


def planificador(juego):
    if(juego.tiempo <= 90):  # Hay tiempo
        
        if(juego.posavasos.robot.disponible):  # Posavasos no esta ocupado
            if(juego.experimento):
                print("Activo el experimento")
                ejecutor(ACTIVAR_EXPERIMENTO, POSAVASOS, juego)
                
            elif(juego.posavasos.ventosas_ocupada):
                print("Vamos a la bahia a descargar los vasos")
                ejecutor(BAHIA_SOLTAR, POSAVASOS, juego)
                
            elif (juego.estanterias.estanteria_neutro_cerca):
                print("Voy a la estanteria del medio")
                ejecutor(ESTANTERIAS_NEUTRO_CERCA, POSAVASOS, juego)
                #time.sleep(40)
            elif (juego.estanterias.estanteria_casa):
                print("Voy a la estanteria de mi lado")
                ejecutor(ESTANTERIAS_CERCA, POSAVASOS, juego)
                
            elif (juego.brujula == 'N'):
                print("Voy a mirar la brujula")
                ejecutor(ACTUALIZAR_BRUJULA, POSAVASOS, juego)
                #time.sleep(40)
            else:
                print("No tengo que hacer nada")
                ejecutor(CASA, POSAVASOS, juego)
    # if(juego.parejitas.disponible):
    #    print("Parejitas es tu turno")
    else:
        print("Rapido pirate")
        ejecutor(CASA, POSAVASOS, juego)


# Ejecutor recrea TyM parcialmente, es decir recibe que ha de hacer y lo traduce en  instrucciones
def ejecutor(orden, robot, juego):
    # (Falta por implementar la creación de los waypoints, asi que solo hace gira->avanza)
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
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        juego.experimento = False
        juego.puntos = juego.puntos+15
        juego.posavasos.robot.pos[0] = Objx
        juego.posavasos.robot.pos[1] = Objy
        juego.posavasos.robot.orientacion = Orientacion_final
        

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
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        #envio_instrucciones_actuadores("B250")
        juego.puntos = juego.puntos+14
        juego.posavasos.ventosas_ocupada = False
        juego.posavasos.robot.pos[0] = Objx
        juego.posavasos.robot.pos[1] = Objy
        time.sleep(26)
        juego.posavasos.robot.orientacion = Orientacion_final

    elif(orden == ESTANTERIAS_CERCA):
        if(juego.lado == AMARILLO):
            Objx = ESTANTERIA_VASOS_4X
            Objy = ESTANTERIA_VASOS_4Y
            Orientacion_final = 0
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_1X
            Objy = ESTANTERIA_VASOS_1Y
            Orientacion_final = 180
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        juego.posavasos.ventosas_ocupada = True
        juego.estanterias.estanteria_casa = False
        juego.posavasos.robot.pos[0]=Objx
        juego.posavasos.robot.pos[1]=Objy
        juego.posavasos.robot.orientacion = Orientacion_final
    
    elif(orden == ESTANTERIAS_NEUTRO_CERCA):
        if(juego.lado == AMARILLO):
            Objx = ESTANTERIA_VASOS_3X
            Objy = ESTANTERIA_VASOS_3Y
        elif(juego.lado == AZUL):
            Objx = ESTANTERIA_VASOS_2X
            Objy = ESTANTERIA_VASOS_2Y
        Orientacion_final = 90
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        #envio_instrucciones_actuadores("S250")
        juego.posavasos.ventosas_ocupada = True
        juego.estanterias.estanteria_neutro_cerca = False
        juego.posavasos.robot.pos[0] = Objx
        juego.posavasos.robot.pos[1] = Objy
        juego.posavasos.robot.orientacion = Orientacion_final
        
    elif(orden == ACTUALIZAR_BRUJULA):
        Objx = CAMARA_X
        Objy = CAMARA_Y
        Orientacion_final = 90
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        juego.brujula = 'N'
        juego.posavasos.robot.pos[0] = Objx
        juego.posavasos.robot.pos[1] = Objy
        juego.posavasos.robot.orientacion = Orientacion_final
    
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
        instruccion_giro1, instruccion_distancia, instruccion_giro2 = instrucciones_giro_avanzo_giro(juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion,Orientacion_final)
        envio_instrucciones_traccion(instruccion_giro1,instruccion_distancia,instruccion_giro2)
        juego.puntos = juego.puntos+10
        juego.Activo = False
        juego.posavasos.robot.pos[0] = Objx
        juego.posavasos.robot.pos[1] = Objy
        juego.posavasos.robot.orientacion = Orientacion_final
    print(f"Posción actual {juego.posavasos.robot.pos[0]}, {juego.posavasos.robot.pos[1]}")
    print(f"Angulo actual {juego.posavasos.robot.orientacion}")
    

def main():
    print("Este script es una demo de la estrategia de cada uno de los robots")
    #print("Seleccione el robot con el que simular la demo (1 para posavasos y 2 para parejitas):")
    try:
        dispositivo = int(input("Seleccione el robot con el que simular la demo (1 para parejitas y 2 para parejitas):"))
    except ValueError:
        print("Dato no reconocido")
    else:
        if(dispositivo == POSAVASOS):
            print("Has escogido a posavasos")
        elif(dispositivo == PAREJITAS):
            print("Has escogido a parejitas")
        else:
            print("Número incorrecto, inicie de nuevo el programa")
            raise Exception
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
            print("Situación inicial:")
            print("Posicion de parejitas")
            print(juego.parejitas.robot.pos[0])
            print(juego.parejitas.robot.pos[1])
            print("Posicion de posavasos")
            print(juego.posavasos.robot.pos[0])
            print(juego.posavasos.robot.pos[1])
            while juego.Activo:
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