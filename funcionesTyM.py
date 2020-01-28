import math


def calculo_giro_avanzo_giro(posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion_original, orientacion_final):
    """
        calculo_giro_avanzo:: Int [Posrobotx,Posroboty,Posobx,Posoby,Original_Yaw,Final_Yaw] -> Int Angle
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
    distancia_total = math.sqrt(
        distancia_x**2+distancia_y**2)   # Calculo el módulo

    # Si no es un caso especial, se aplica la arcotangente
    if(distancia_x != 0):
        angulo_objetivo = math.atan(distancia_y/distancia_x)
    # Para los casos especiales donde la arcotangete puede dar problemas
    else:
        if(distancia_y > 0):
            angulo_objetivo = math.pi/2
        elif (distancia_y < 0):
            angulo_objetivo = 3*math.pi/2
        else:
            angulo_objetivo = 0
    # Uno de los ángulos de giro se calcula asi:
    angulo_giro1 = angulo_objetivo-orientacion_original
    if(abs(angulo_giro1) > math.pi):  # Si te has equivocado y era el otro
        if(angulo_giro1 > 0):
            angulo_giro1 = angulo_giro1-2*math.pi
        else:
            angulo_giro1 = angulo_giro1+2*math.pi
    # Para calcular el otro ángulo se plantea algo similar a lo anterior
    angulo_giro2 = orientacion_final-angulo_objetivo
    if(abs(angulo_giro2) > math.pi):
        if(angulo_giro2 > 0):
            angulo_giro2 = angulo_giro2-2*math.pi
        else:
            angulo_giro2 = angulo_giro2+2*math.pi
    # Printeos para asegurar que todo ha ido bien
    print("Se ha de girar este angulo primero:")
    print(math.degrees(angulo_giro1))
    print("Y esta es la distancia que se ha de recorrer: ")
    print(distancia_total)
    print("Por último se gira:")
    print(angulo_giro2)
    return distancia_total, angulo_giro1, angulo_giro2


def instrucciones_giro_avanzo_giro(posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion, orientacion_final):
    """
        instrucciones_giro_avanzo:: Int [Posrobotx,Posroboty,Posobx,Posoby,Yaw] -> String 
        Recibe la posición del robot, su orientación, la posición del objetivo
        y la orientación final y todo esto lo codifica en instrucciones para el nivel bajo
    """
    distancia_total, angulo_giro1, angulo_giro2 = calculo_giro_avanzo_giro(
        posrobotx, posroboty, posobjetivox, posobjetivoy, orientacion, orientacion_final)
    angulo_giro1 = int(math.degrees(angulo_giro1))
    instruccion_giro1 = "G"+str(angulo_giro1)
    print(instruccion_giro1)
    instruccion_distancia = "D"+str(int(distancia_total))
    print(instruccion_distancia)
    angulo_giro2 = int(math.degrees(angulo_giro2))
    instruccion_giro2 = "G"+str(angulo_giro2)
    print(instruccion_giro2)
    return instruccion_giro1, instruccion_distancia, instruccion_giro2


def instrucciones_lidar(posrobotx, posroboty, poslidarx, poslidary):
    # filtrado paredes en proceso
    # Poder indicar la cantidad de robots del enemigo estaría guay
    distancia_filtro = 600  # 60cm
    radio_seguridad = 300  # 40cm
    medio_lado_robot = 200  # 20cm
    distancia_total = math.sqrt(poslidarx**2+poslidary**2)
    if(distancia_total < distancia_filtro):
        # Si no es un caso especial, se aplica la arcotangente
        if(poslidarx != 0):
            angulo_objeto = math.atan(poslidary/poslidarx)
    # Para los casos especiales donde la arcotangete puede dar problemas
        else:
            if(poslidary > 0):
                angulo_objeto = math.pi/2
            else:
                angulo_objeto = 0
        if(angulo_objeto > 0):
            proyeccion_de_seguridad = distancia_total * \
                math.cos(angulo_objeto)-radio_seguridad
            if(proyeccion_de_seguridad < medio_lado_robot):
                return 1, 'S'  # Para, que te lo comes
            else:
                return 0, 'V'  # Reduce la velocidad
        elif(angulo_objeto < 0):
            proyeccion_de_seguridad = +radio_seguridad - \
                distancia_total*math.cos(angulo_objeto)
            if(proyeccion_de_seguridad > -1*medio_lado_robot):
                return 1, 'S'  # Para, que te lo comes
            else:
                return 0, 'V'  # Reduce la velocidad, en el futuro dependera de la zona y el ángulo
        else:
            return 0, '0'

    else:
        return 0, '0'
