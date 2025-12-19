#include <Arduino.h>
#include <Wire.h>
#include "sensors/bme680_sensor.h"
#include "sensors/scd30_sensor.h"
#include "net/wifi_service.h"
#include "net/mqtt_service.h"

BME680Sensor bme;
SCD30Sensor scd30;
WiFiService wifi;
MQTTService mqtt;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  bme.begin();
  scd30.begin();
  WiFiService::begin();
  mqtt.begin();
}

void loop() {
  if (!WiFiService::connected()) return;
  if (!mqtt.connected()) return;

  mqtt.loop();

  IAQReading reading{};
  reading.timestamp = millis();

  if (scd30.read(reading)) {
    bme.read(reading);
    mqtt.publish(reading);
    Serial.println("Published IAQ reading");
  }

  delay(2000);
}
