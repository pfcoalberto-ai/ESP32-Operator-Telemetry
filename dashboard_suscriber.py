import paho.mqtt.client as mqtt
import json
import datetime

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "ruiz_ripalda/telemetry/touch"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("[INFO] Conectado exitosamente al Broker MQTT")
        client.subscribe(TOPIC)
        print(f"[INFO] Suscrito al tópico: {TOPIC}\n")
    else:
        print(f"[ERROR] Fallo en la conexión, código de retorno: {rc}")

def on_message(client, userdata, msg):
    try:
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        data = json.loads(msg.payload.decode('utf-8'))
        
        touch_val = data.get("touch_val")
        state = data.get("state")
        
        print(f"[{timestamp}] Lectura capacitiva: {touch_val} | Estado Operador: {state}")
        
        # Guardar en archivo para registro histórico
        with open("telemetry_history.log", "a") as f:
            f.write(f"{timestamp},{touch_val},{state}\n")
            
    except Exception as e:
        print(f"[ERROR] Error al procesar mensaje: {e}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

print("[INFO] Conectando a la plataforma de telemetría...")
client.connect(BROKER, PORT, 60)

client.loop_forever()
