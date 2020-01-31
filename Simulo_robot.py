from funcionesTyM import calculo_giro_avanzo_giro
import time
import math
# ROBOTS
PAREJITAS = 0
POSAVASOS = 1


def simula_movimiento(juego, robot, Objx, Objy, orientacion_final):
    if(robot == PAREJITAS):
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
            juego.parejitas.pos[0], juego.parejitas.pos[1], Objx, Objy, juego.parejitas.orientacion, orientacion_final)
        print(distancia)
        print(angulo_giro1)
        print(angulo_giro2)
        feedback = control(distancia, angulo_giro1+angulo_giro2, juego)
        if(feedback):
            juego.parejitas.pos[0] = Objx
            juego.parejitas.pos[1] = Objy
            juego.parejitas.orientacion = orientacion_final
            juego.experimento = False
            juego.puntos = juego.puntos+15
        else:
            print("Error")
    else:
        distancia, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
            juego.posavasos.pos[0], juego.posavasos.pos[1], Objx, Objy, juego.posavasos.orientacion, orientacion_final)
        print(distancia)
        print(angulo_giro1)
        print(angulo_giro2)
        feedback = control(distancia, angulo_giro1+angulo_giro2, juego)
        if(feedback):
            juego.posavasos.pos[0] = Objx
            juego.posavasos.pos[1] = Objy
            juego.posavasos.orientacion = orientacion_final
            juego.experimento = False
            juego.puntos = juego.puntos+15
        else:
            print("Error")


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
        juego.robot.ventosas_ocupada = True
        print("Vasos recogidos")
        juego.tiempo = juego.tiempo+5  # Añado tiempos
    elif(accion == "S"):
        print("Soltando vasos")
        # send_mensaje("AS"):Realidad
        time.sleep(14)  # Simulacion
        print("Vasos sueltos")
        juego.tiempo = juego.tiempo+10  # Añado tiempos
        juego.puntos = juego.puntos+14
        juego.robot.ventosas_ocupada = False
