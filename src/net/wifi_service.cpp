#include "wifi_service.h"
#include <WiFiNINA.h>
#include "secrets.h"

void WiFiService::begin() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool WiFiService::connected() {
    return WiFi.status() == WL_CONNECTED;
}
