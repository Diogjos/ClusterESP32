#include <WiFi.h>
#include <PubSubClient.h>

// Configurações de Wi-Fi
const char* ssid = ""; // Nome da rede Wi-Fi
const char* password = ""; // Senha da rede Wi-Fi

// Configurações do Broker MQTT
const char* mqtt_server = ""; // IP do broker MQTT
const int mqtt_port = 1883; // Porta do broker MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Função para verificar se um número é primo
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Função de callback para receber mensagens
void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    // Processa o intervalo recebido
    int start = message.substring(0, message.indexOf(',')).toInt();
    int end = message.substring(message.indexOf(',') + 1).toInt();

    String primes = "";
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            primes += String(i) + ",";
        }
    }

    // Publica os resultados
    client.publish("result", primes.c_str());
    Serial.println("Resultados enviados: " + primes);
}

void setup() {
    Serial.begin(115200);

    // Conecta ao Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado ao Wi-Fi");

    // Conecta ao Broker MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback); // Configura a função de callback
    while (!client.connected()) {
        Serial.println("Conectando ao Broker MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado ao Broker MQTT");
            client.subscribe("task"); // Inscreve-se no tópico "task"
        } else {
            Serial.print("Falha na conexão, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

void loop() {
    client.loop();
}
