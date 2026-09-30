# ESP32 Touch Telemetry System

Embedded telemetry solution to monitor machine-operator interaction in real time using capacitive touch sensors on an ESP32 edge node over MQTT.

## Files
- `main.ino`: ESP32 C++ firmware sending capacitive touch readings over MQTT.
- `dashboard_subscriber.py`: Python client receiving and logging live telemetry data.

## Setup & Hardware
- ESP32 Development Board (Touch Sensor Pin GPIO4 / TOUCH0)
- Threshold set to 750 (Unpressed ~1098, Pressed ~509)
- MQTT Broker: `broker.hivemq.com`
