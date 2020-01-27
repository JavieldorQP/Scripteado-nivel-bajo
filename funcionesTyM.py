import math


def calculo_giro_avanzo(posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion):
    """
        calculo_giro_avanzo:: Int [Posrobotx,Posroboty,Posobx,Posoby,Yaw] -> Int Angle
        Recibe la posición del robot, su orientación y la posición del objetivo
        y calcula el ángulo de giro que ha de realizar.
        >>> calculo_giro(0,0,10,10,-45)
        math.degrees(90)
        >>> calculo_giro(0,0,10,10,45)
        0
        >>> calculo_giro(0,0,0,10,-30)
        math.degrees(120)
    """
    distancia_x = posobjetivox-posrobotx
    distancia_y = posobjetivoy-posroboty
    distancia_total = math.sqrt(distancia_x**2+distancia_y**2)
    if(distancia_x != 0):
        angulo_objetivo = math.atan(distancia_y/distancia_x)
    else:
        if(distancia_y > 0):
            angulo_objetivo = math.pi/2
        elif (distancia_y < 0):
            angulo_objetivo = 3*math.pi/2
        else:
            angulo_objetivo = 0

    angulo_giro = angulo_objetivo-orientacion
    if(abs(angulo_giro) > 180):
        if(angulo_giro > 0):
            angulo_giro = angulo_giro-360
        else:
            angulo_giro = angulo_giro+360
    print("Se ha de girar este angulo:")
    print(math.degrees(angulo_giro))
    print("Y esta es la distancia que se ha de recorrer: ")
    print(distancia_total)
    return distancia_total, angulo_giro


def instrucciones_giro_avanzo(posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion):
    distancia_total, angulo_giro = calculo_giro_avanzo(
        posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion)
    angulo_giro = int(math.degrees(angulo_giro))
    instruccion_giro = "G"+str(angulo_giro)
    print(instruccion_giro)
    instruccion_distancia = "D"+str(int(distancia_total))
    print(instruccion_distancia)
    return instruccion_giro, instruccion_distancia
