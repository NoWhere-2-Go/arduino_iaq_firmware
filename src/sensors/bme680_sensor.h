#pragma once
#include <Adafruit_BME680.h>
#include "models/iaq_reading.h"

class BME680Sensor {
public:
    bool begin();
    bool read(IAQReading& reading);

private:
    Adafruit_BME680 bme;
};
