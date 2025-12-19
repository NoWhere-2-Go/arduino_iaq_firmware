#include "bme680_sensor.h"
#include <Wire.h>

bool BME680Sensor::begin() {
    if (!bme.begin()) return false;

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
    return true;
}

bool BME680Sensor::read(IAQReading& r) {
    if (!bme.performReading()) return false;

    r.temp_bme = bme.temperature;
    r.humidity_bme = bme.humidity;
    r.pressure = bme.pressure / 100.0f;
    r.gas_resistance = bme.gas_resistance / 1000.0f;
    r.altitude = bme.readAltitude(1013.25);

    return true;
}
