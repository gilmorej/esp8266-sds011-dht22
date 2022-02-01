#pragma once

struct particleSensorState_t {
    float pm25 = 0;
    float pm10 = 0;
    float temp = 0;
    float humidity = 0;
    float heatIndex = 0;
    boolean valid = false;
};
