import paho.mqtt.client as mqtt

# Função para receber mensagens
def on_message(client, userdata, message):
    print(f"Resultado: {message.payload.decode()}")

# Configurações do Broker MQTT
broker_ip = "192.168.0.103"  # IP do broker MQTT
broker_port = 1883  # Porta do broker MQTT

# Cria o cliente MQTT
client = mqtt.Client()
client.connect(broker_ip, broker_port, 60)
client.subscribe("result")
client.on_message = on_message

# Envia tarefas para os ESP32s
client.publish("task", "1,50")  # ESP32 1
client.publish("task", "51,100")  # ESP32 2

# Mantém a conexão ativa
client.loop_forever()