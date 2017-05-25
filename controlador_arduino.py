'''
código de teste para mandar mensagens para um broker cloudmqtt
'''
import paho.mqtt.client as paho

# funções callback
def on_connect(mosq, obj, rc):
    print("rc: " + str(rc))

def ao_receber_msg(mosq, obj, msg):
    # print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    print("\n\nMensagem recebida pelo tópico: "+msg.topic+"\nConteúdo: "+str(msg.payload))

def ao_enviar(mosq, obj, mid):
    print("mid: " + str(mid))

def ao_se_inscrever(mosq, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
# função de log pra debug
def on_log(mosq, obj, level, string):
    print(string)
# Objeto que vai lidar com a conexão
mqttc = paho.Client()

# Atribuir as funcões criadas ao objeto mqttc
mqttc.on_message = ao_receber_msg
mqttc.on_connect = on_connect
mqttc.on_publish = ao_enviar
mqttc.on_subscribe = ao_se_inscrever

# descomentar para ativar as mensagens de debug
#mqttc.on_log = on_log

# Conectar
mqttc.username_pw_set('cdhwvxkk', 'uoHN9SWskIFf')

mqttc.connect('m13.cloudmqtt.com', 17093)


# loop que mantem a conexão para lidar com msgs recebidas. Sai quando algum erro acontece
topico = ""
mensg = ""
while True:
    topico = input("digite o tópico: ")
    mensg = input("digite a msg: ")
    # publicar msg
    mqttc.publish(topico, mensg)
