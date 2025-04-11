#ifndef REP_STATE_H
#define REP_STATE_H

#include <Arduino.h>
#include <queue>

struct RepState {
    std::queue<float> state_distances_q;
    std::queue<float> curr_distances_q;
    std::queue<float> lag_distances_q;

    float state_distances_sum = 0.0f;
    float curr_distances_sum = 0.0f;
    float lag_distances_sum = 0.0f;

    float curr_distances_avg = 0.0f;
    float lag_distances_avg = 0.0f;
    float state_distances_avg = 0.0f;

    std::queue<float> state_velocity_q;
    float state_velocity_sum = 0.0f;
    float state_velocity_avg = 0.0f;

    float previous_distance = 0.0f;
    float previous_state_avg = 200.0f; // Default offset to prevent early trigger
    float peak_velocity = 0.0f;
};

#endif