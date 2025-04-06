#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

#define DEBUG 1

void debug_output(float time_diff, float current_time, float distance, int current_state, float curr_distances_avg, float lag_distances_avg, float state_distances_avg, float state_velocity_avg, float velocity);

#endif
