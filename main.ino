#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de red Wi-Fi
const char* ssid = "Totalplay-84B1";
const char* password = "84B1AE2ADYPun5dp";

// Configuración Broker MQTT (ej. broker.hivemq.com o IP/Host de tu servidor)
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "ruiz_ripalda/telemetry/touch";

WiFiClient espClient;
PubSubClient client(espClient);

// Pin capacitivo del ESP32 (GPIO4 equivale a TOUCH0)
const int TOUCH_PIN = 4;
const int TOUCH_THRESHOLD = 750; // Ajustar según el nivel de sensibilidad deseado

unsigned long lastMsg = 0;
const long interval = 500; // Enviar lecturas cada 500 ms

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32_TouchNode_";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado a MQTT!");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;

    // Leer el valor del sensor capacitivo (rango típico: ~0 a 100)
    int touchValue = touchRead(TOUCH_PIN);
    
    // Evaluar si está presionado o no
    bool isTouched = (touchValue < TOUCH_THRESHOLD);

    // Formatear payload en JSON
    String payload = "{\"touch_val\":" + String(touchValue) + 
                     ",\"state\":\"" + (isTouched ? "TOUCHED" : "RELEASED") + "\"}";

    Serial.print("Enviando Telemetría: ");
    Serial.println(payload);

    // Publicar al broker MQTT
    client.publish(mqtt_topic, payload.c_str());
  }
}
