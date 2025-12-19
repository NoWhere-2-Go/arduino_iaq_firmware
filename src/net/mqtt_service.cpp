#include "mqtt_service.h"
#include <ArduinoJson.h>
#include "secrets.h"

void MQTTService::begin() {
    mqttClient.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);
    mqttClient.connect(MQTT_BROKER, MQTT_PORT);
}

bool MQTTService::connected() {
    return mqttClient.connected();
}

void MQTTService::loop() {
    mqttClient.poll();
}

void MQTTService::publish(const IAQReading& r) {
    auto document = JsonDocument();

    document["co2"] = r.co2;
    document["temp_scd30"] = r.temp_scd30;
    document["humidity_scd30"] = r.humidity_scd30;
    document["temp_bme"] = r.temp_bme;
    document["humidity_bme"] = r.humidity_bme;
    document["pressure"] = r.pressure;
    document["gas"] = r.gas_resistance;
    document["altitude"] = r.altitude;
    document["ts"] = r.timestamp;

    mqttClient.beginMessage(MQTT_TOPIC);
    serializeJson(document, mqttClient);
    mqttClient.endMessage();
}
