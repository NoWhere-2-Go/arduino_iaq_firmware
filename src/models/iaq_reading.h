#pragma once
#include <stdint.h>

struct IAQReading {
    float co2;
    float temp_scd30;
    float humidity_scd30;
    float temp_bme;
    float humidity_bme;
    float pressure;
    float gas_resistance;
    float altitude;
    uint32_t timestamp;
};
