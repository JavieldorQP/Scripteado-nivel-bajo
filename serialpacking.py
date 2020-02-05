"""

"""

# Funcion propia del protocolo


def pose_to_msg(Posicion):
    """
        pose_to_msg:: Num -> String
        Utiliza un entero de tres cifras y devuelve un string con el formato que se utiliza 
        en la comunicación para los datos tipos posición posiciones 
        >>> pose_to_msg(23)
        '+0023'
        >>> pose_to_msg(-24)
        '-0024'
    """
    if(Posicion >= 0):                                              # Si el numero es positivo o 0
        if (Posicion >= 1000):                                      # Si el numero es superior a 1000
            MSG = "+" + str(int(Posicion))
        elif (Posicion >= 100):
            MSG = "+0" + str(int(Posicion))
        elif (Posicion >= 10):
            MSG = "+00" + str(int(Posicion))
        else:  # lif (Posicion >= 0):
            MSG = "+000" + str(int(Posicion))
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
            MSG = "-" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 100):
            MSG = "-0" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 10):
            MSG = "-00" + str(abs(int(Posicion)))
        elif (abs(Posicion) >= 0):
            MSG = "-000" + str(abs(int(Posicion)))
    return MSG

def angle_to_msg(Grados):
    """
        angle_to_msg:: Num -> String
        Utiliza un entero de tres cifras y devuelve un string con el formato que se utiliza 
        en la comunicación para los datos tipos posición posiciones 
        >>> angle_to_msg(123)
        '+123'
        >>> angle_to_msg(-124)
        '-124'
        >>> angle_to_msg(9)
        '+009'
        >>> angle_to_msg(-14)
        '-014'
        >>> angle_to_msg(-174)
        '-174'
        >>> angle_to_msg(90)
        '+090'
    """
    if (Grados >= 100):
        MSG = "+" + str(int(Grados))
    elif (Grados >= 10):
        MSG = "+0" + str(int(Grados))
    elif (Grados >= 0):
        MSG = "+00" + str(int(Grados))
    elif (Grados > -10):
        MSG = "-00" + str(abs(int(Grados)))
    elif (Grados > -100):
        MSG = "-0" + str(abs(int(Grados)))
    elif (Grados > -1000):
        MSG = "-" + str(abs(int(Grados)))
    else:
        print("Error")
    return MSG
if __name__ == "__main__":
    import doctest
    doctest.testmod()
