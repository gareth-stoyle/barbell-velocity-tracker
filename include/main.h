#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "sensor.h"
#include "logging.h"

void setup();
bool is_outlier(float distance, float avg);
void loop();

const int LOOP_DELAY = 2000;
const int MAX_TICKS = 22500;
const float MICROSECONDS_PER_SECOND = 1000000.0;

// no. of loops
int ticks = 0;

unsigned long old_time = 0;

#endif