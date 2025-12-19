#include "scd30_sensor.h"

bool SCD30Sensor::begin() {
    return scd30.begin();
}

bool SCD30Sensor::read(IAQReading& r) {
    if (!scd30.dataAvailable()) return false;

    r.co2 = scd30.getCO2();
    r.temp_scd30 = scd30.getTemperature();
    r.humidity_scd30 = scd30.getHumidity();

    return true;
}
