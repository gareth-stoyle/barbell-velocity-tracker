#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

const int TRIG_PIN = 19;
const int ECHO_PIN = 18;

void init_sensor();
long read_sensor();

#endif
