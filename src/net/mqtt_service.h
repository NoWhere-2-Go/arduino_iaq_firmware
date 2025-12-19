#pragma once
#include <ArduinoMqttClient.h>
#include "WiFiSSLClient.h"
#include "models/iaq_reading.h"

class MQTTService {
public:
    void begin();
    void loop();
    bool connected();
    void publish(const IAQReading& reading);

private:
    WiFiSSLClient wifiClient;
    MqttClient mqttClient{wifiClient};
};
