#ifndef DISTANCE_H
#define DISTANCE_H

#include <Arduino.h>
#include <queue>
#include <cassert>

// Number of ticks to average
const int CURR_DIST_LEN = 5;
// Number of ticks to average
const int LAG_DIST_LEN = 5;
// max possible size of the state_distances_q
const int STATE_DIST_MAX_SIZE = 200;

extern std::queue<float> state_distances_q;
extern std::queue<float> curr_distances_q;
extern std::queue<float> lag_distances_q;

extern float state_distances_sum;
extern float curr_distances_sum;
extern float lag_distances_sum;

extern float curr_distances_avg;
extern float lag_distances_avg;
extern float state_distances_avg;

extern std::queue<float> state_velocity_q;
extern float state_velocity_sum;
extern float state_velocity_avg;

extern float previous_distance;
extern float previous_state_avg;

void update_distance_calcs(float distance, float velocity);
void update_velocity_calcs(float velocity);
void update_averages();

#endif
