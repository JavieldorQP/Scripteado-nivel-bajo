"""

"""

# Funcion propia del protocolo


def POSE_TO_MSG(Posicion):
    """
        pose_to_msg:: Num -> String
        Utiliza un entero de tres cifras y devuelve un string con el formato que se utiliza 
        en la comunicación para los datos tipos posición posiciones 
        >>> POSE_TO_MSG(23)
        '+0023'
        >>> POSE_TO_MSG(-24)
        '-0024'
    """
    if(Posicion >= 0):                                              # Si el numero es positivo o 0
        if (Posicion >= 1000):                                      # Si el numero es superior a 1000
            MSG = "+" + str(Posicion)
        elif (Posicion >= 100):
            MSG = "+0" + str(Posicion)
        elif (Posicion >= 10):
            MSG = "+00" + str(Posicion)
        else:  # lif (Posicion >= 0):
            MSG = "+000" + str(Posicion)
        # if Posicion < 0:
        #     MSG = "-" + MSG
        # else:
        #     MSG = '+' + MSG
    # if posicion < 0:
    #     signo = "-"
    # else:
    #     signo = "+"
    # return signo + msg

    if(Posicion < 0):
        if (abs(Posicion) >= 1000):                                 # Si el numero es superior a 1000
            MSG = "-" + str(abs(Posicion))
        elif (abs(Posicion) >= 100):
            MSG = "-0" + str(abs(Posicion))
        elif (abs(Posicion) >= 10):
            MSG = "-00" + str(abs(Posicion))
        elif (abs(Posicion) >= 0):
            MSG = "-000" + str(abs(Posicion))
    return MSG


if __name__ == "__main__":
    import doctest
    doctest.testmod()
