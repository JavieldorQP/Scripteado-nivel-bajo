
from Eurocomunicacion_raspy_funciones import envio_instrucciones_actuadores
import time

#envio_instrucciones_actuadores("C")
#envio_instrucciones_actuadores("C")
#print(" He salido")
#time.sleep(100)
envio_instrucciones_actuadores("S050")
print(" He salido")
time.sleep(10)
envio_instrucciones_actuadores("B090")
#time.sleep(20)

