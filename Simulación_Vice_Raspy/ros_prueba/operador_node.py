import rospy
from eurobot_ws.msg import OperadorStatus, Instruccion 
from geometry_msgs.msg import Pose2D
from Operador import Operador

# mantiene un estado entre ["Moviendo", "Parado", "Error"]
# Publica dos mensajes, estado, y odometria
# Estado contiene [Timestamp, Estado, Id Instruccion, Instruccion, arg1, arg2, arg3]
# Odom es un Pose2D

instruccion_nueva = Instruccion()

def sub_instruccion_callback(msg):
    instruccion_nueva = msg

if __name__ == "__main__":
    # Subscribers (en este caso solo instruccion)
    sub_instruccion = rospy.Subscriber("/instruccion", Instruccion, callback=sub_instruccion_callback)

    # Publishers (Odometria y estado de la MiniDK2)
    pub_odom = rospy.Publisher("/odom", Pose2D, queue_size=10)
    pub_status = rospy.Publisher("/actuadores", OperadorStatus, queue_size=10)
    pub_status = rospy.Publisher("/estado_MDK2", OperadorStatus, queue_size=10)

    # Instanciamos el operador
    port = rospy.get_param("/mdk2_port")            # Iniciamos estos parametros con el launch file
    bitrate = rospy.get_param("/mdk2_bitrate")
    timeout = rospy.get_param("/mdk2_timeout")
    op = Operador(port, bitrate, timeout)

    while not rospy.is_shutdown():
        # Actualizamos el estado
        # op.actualizar_estado()                  LO SIENTO RICARDO ESTO NO ESTA PREPARADO

        # Enviamos la instruccion si no es la instruccion presente o la proxima
        if(op.instruccion_actual.id != instruccion_nueva.id and
        op.instruccion_proxima.id != instruccion_nueva.id):
            op.enviar_instruccion(instruccion_nueva)

        # Publicamos la odometria
        pub_odom.publish(op.odom)
        
        # Publicamos el estado de la miniDK2
        pub_status.publish(op.status)