#include <Adafruit_BME680.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
#include "secrets.h"

// wifi configuration
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
// mqtt configuration
const char* mqtt_broker = MQTT_BROKER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_topic = MQTT_TOPIC;

WiFiSSLClient wifiClient;
MqttClient mqttClient(wifiClient);
Adafruit_BME680 bme;
SCD30 scd30;

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  Serial.print("Connecting to MQTT broker...");
  mqttClient.setUsernamePassword(mqtt_username, mqtt_password);

  while (!mqttClient.connect(mqtt_broker, mqtt_port)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT connected!");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();

  if (!bme.begin()) {
    Serial.println("Could not find BME680 sensor!");
    while (1);
  }

  if (!scd30.begin()) {
    Serial.println("Could not find SCD30 sensor!");
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms

  scd30.setMeasurementInterval(2);

  connectWiFi();
  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.poll();

  // read sensors
  if (scd30.dataAvailable()) {
    StaticJsonDocument<256> doc;

    doc["co2"] = scd30.getCO2();
    doc["temperature_scd30"] = scd30.getTemperature();
    doc["humidity_scd30"] = scd30.getHumidity();

    // BME680 readings
    if (bme.performReading()) {
      doc["temperature_bme680"] = bme.temperature;
      doc["humidity_bme680"] = bme.humidity;
      doc["pressure"] = bme.pressure / 100.0; // convert to hPa
      doc["gas_resistance"] = bme.gas_resistance / 1000.0; // convert to KOhms
      doc["altitude"] = bme.readAltitude(1013.25); // sea level pressure
    }

    doc["timestamp"] = millis();
    doc["device_id"] = "arduino_nano_33_iot_001";

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // publish to MQTT
    mqttClient.beginMessage(mqtt_topic);
    mqttClient.print(jsonBuffer);
    mqttClient.endMessage();

    Serial.println("Published: ");
    Serial.println(jsonBuffer);
  }

  delay(2000);
}