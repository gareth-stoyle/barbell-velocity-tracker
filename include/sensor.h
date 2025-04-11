#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

const int TRIG_PIN = 19;
const int ECHO_PIN = 18;
// Speed of sound in m/s
const float SPEED_OF_SOUND = 0.034;

struct SensorReading {
    long duration = 0;
    float distance = 0.0f;
    float velocity = 0.0f;
};

void init_sensor();
void read_sensor(SensorReading& sensor_reading, float previous_distance, float time_diff);

#endif
