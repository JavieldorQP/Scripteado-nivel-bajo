from funcionesTyM import calculo_giro_avanzo_giro
import time
import math
import random
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1


def simula_movimiento(juego, robot, Objx, Objy, orientacion_final):

    if(robot == PAREJITAS):
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
            juego.parejitas.robot.pos[0], juego.parejitas.robot.pos[1], Objx, Objy, juego.parejitas.robot.orientacion, orientacion_final)
        feedback = control(distancia, angulo_giro1+angulo_giro2, juego)
        if(feedback):
            juego.parejitas.robot.pos[0] = Objx
            juego.parejitas.robot.pos[1] = Objy
            juego.parejitas.robot.orientacion = orientacion_final
        else:
            print("Error")

    else:
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
            juego.posavasos.robot.pos[0], juego.posavasos.robot.pos[1], Objx, Objy, juego.posavasos.robot.orientacion, orientacion_final)
        feedback = control(distancia, angulo_giro1+angulo_giro2, juego)
        if(feedback):
            juego.posavasos.robot.pos[0] = Objx
            juego.posavasos.robot.pos[1] = Objy
            juego.posavasos.robot.orientacion = orientacion_final
        else:
            print("Error imposible llegar")


def control(angulo, distancia, juego):
    """Función que simula el nivel bajo, de momento no hace más que generar delays
    en función de las distancias recorridas, en el futuro sera capaz de simular
    enemigos y errores."""
    vdefecto = 500  # 0.5 m/s
    wdefecto = 360  # 1Hz
    print("Tiempo que tarda el robot:")
    tiempo = abs((distancia/vdefecto))+abs((angulo/wdefecto))
    juego.tiempo = juego.tiempo+tiempo
    print(tiempo)
    time.sleep(tiempo)
    return 1


def actuadores(accion, juego):
    """Función que se encarga de simular el comportamiento del actuador y
    simula un tiempo para recoger los vasos."""
    if(accion == "R"):  # Toca recoger
        print("Recogiendo vasos")
        # send_mensaje("AR"):Realidad
        time.sleep(8)  # Simulacion
        juego.posavasos.ventosas_ocupada = True
        print("Vasos recogidos")
        juego.tiempo = juego.tiempo+8  # Añado tiempos
    elif(accion == "S"):
        print("Soltando vasos")
        # send_mensaje("AS"):Realidad
        time.sleep(14)  # Simulacion
        print("Vasos sueltos")
        juego.tiempo = juego.tiempo+14  # Añado tiempos
        juego.posavasos.ventosas_ocupada = False


def camara(juego):
    juego.brujula = random.choice(["N", "S"])
    time.sleep(4)
    juego.tiempo = juego.tiempo+4
