#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "sensor.h"
#include "logging.h"

void setup();
bool is_outlier(float distance, float avg);
void loop();

// Speed of sound in m/s
const float SPEED_OF_SOUND = 0.034;

// 30% deviation from the rolling average 
const float OUTLIER_THRESH = 0.5;

const int LOOP_DELAY = 2000;

// no. of loops
int ticks = 0;

unsigned long old_time = 0;

#endif