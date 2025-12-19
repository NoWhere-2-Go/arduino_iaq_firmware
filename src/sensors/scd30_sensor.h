#pragma once
#include "SparkFun_SCD30_Arduino_Library.h"
#include "models/iaq_reading.h"

class SCD30Sensor {
public:
    bool begin();
    bool read(IAQReading& reading);

private:
    SCD30 scd30;
};
