#ifndef DISTANCE_H
#define DISTANCE_H

#include <Arduino.h>
#include "rep_state.h"
#include "sensor.h"
#include <cassert>

// Number of ticks to average
const int CURR_DIST_LEN = 5;
// Number of ticks to average
const int LAG_DIST_LEN = 5;
// max possible size of the state_distances_q
const int STATE_DIST_MAX_SIZE = 200;
// % accepted deviation from the rolling average 
const float OUTLIER_THRESH = 0.5;

void update_distance_calcs(RepState& state, SensorReading& sensor_reading);
void update_velocity_calcs(RepState& state, SensorReading& sensor_reading);
void update_averages(RepState& state);
bool is_outlier(float distance, float avg);

#endif
