#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>
#include "rep_phase.h"

#define DEBUG 1

void debug_output(float time_diff, float current_time, PhaseState current_state, RepState& rep_state, SensorReading& sensor_reading);

#endif
