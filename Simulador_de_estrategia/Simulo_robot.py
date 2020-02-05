from funcionesTyM import calculo_giro_avanzo_giro
import time
import math
import random
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1


def simula_movimiento(juego, robot, Objx, Objy, orientacion_final):
    """
    Simula movimientos:
        Se encarga de calcular los ángulos y distancias que tiene que recorrer el robot,
        además de calcularlos tambien hace una aproximación de lo que tardaria el robot
        en realizar esta acción
    """

    if(robot == PAREJITAS):                                                          # Si el robot es parejitas:                                                                                   
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(            # Función que calcula el angulo y la distancia a recorrer
            juego.parejitas.robot.pos[0], juego.parejitas.robot.pos[1], Objx, Objy, juego.parejitas.robot.orientacion, orientacion_final)
        feedback,tiempo = control(distancia, angulo_giro1+angulo_giro2, juego)      # Función que simula y calcula los tiempos que tarda el robot
        if(feedback):                                                                # Si todo va bien a nivel bajo actualizo:
            juego.parejitas.robot.pos[0] = Objx
            juego.parejitas.robot.pos[1] = Objy
            juego.parejitas.robot.orientacion = orientacion_final
            return int(tiempo)                                                            # Devuelvo el tiempo     
        else:                                                                        # Si se detecta un error a nivel bajo:
            print("Error")
            return 0                                                                 # Devolveria el flag de error                             

    else:                                                                            # Si el robot es posavasos:                                                   
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
            juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion, orientacion_final)
        feedback,tiempo = control(distancia, angulo_giro1+angulo_giro2, juego)
        if(feedback):
            juego.posavasos.robot.pos[0] = Objx
            juego.posavasos.robot.pos[1] = Objy
            juego.posavasos.robot.orientacion = orientacion_final
            return int(tiempo) 
        else:
            print("Error imposible llegar")
            return 0


def control(angulo, distancia, juego):
    """Función que simula el nivel bajo, de momento no hace más que generar delays
    en función de las distancias recorridas, en el futuro sera capaz de simular
    enemigos y errores."""
    vdefecto = 500                                                                  # Se considera que va a esta velocidad 0.5 m/s   
    wdefecto = 360                                                                  # Se considera que va a esta velocidad anguar1Hz
    duracion = abs((distancia/vdefecto))+abs((angulo/wdefecto))                     # Calcula el tiempo que tarda    
    return 1, duracion                                                              # Devuelve si ha habido un error y la duración


def actuadores(accion, juego):
    """Función que se encarga de simular el comportamiento del actuador y
    simula un tiempo para recoger los vasos."""
    if(accion == "R"):  # Toca recoger
        print("Recogiendo vasos")
        # send_mensaje("AR"):Realidad
        juego.posavasos.ventosas_ocupada = True
        print("Vasos recogidos")
        return 8
        
    elif(accion == "S"):
        print("Soltando vasos")
        # send_mensaje("AS"):Realidad
        print("Vasos sueltos")
        juego.posavasos.ventosas_ocupada = False
        return 14


def camara(juego):
    juego.brujula = random.choice(["N", "S"])
    return 4
