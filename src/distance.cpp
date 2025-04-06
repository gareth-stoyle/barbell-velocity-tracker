#include "distance.h"

// Queues for whole state, current block and lag block distances
std::queue<float> state_distances_q = {};
std::queue<float> curr_distances_q = {};
std::queue<float> lag_distances_q = {};

float state_distances_sum = 0.0f;
float curr_distances_sum = 0.0f;
float lag_distances_sum = 0.0f;

float curr_distances_avg = 0.0f;
float lag_distances_avg = 0.0f;
float state_distances_avg = 0.0f;

std::queue<float> state_velocity_q = {};
float state_velocity_sum = 0.0f;
float state_velocity_avg = 0.0f;

float previous_distance = 0.0f;

void update_distance_calcs(float distance, float velocity) {
    float removed_curr_dist_val = 0.0f;
    float removed_state_dist_val = 0.0f;
    assert(lag_distances_q.size() <= curr_distances_q.size());

    // If lag_distances_q is full, move the front element to state_distances_q
    if (lag_distances_q.size() == LAG_DIST_LEN) {
        removed_state_dist_val = lag_distances_q.front();
        lag_distances_sum -= removed_state_dist_val;
        lag_distances_q.pop();

        state_distances_q.push(removed_state_dist_val);
        state_distances_sum += removed_state_dist_val;
    }

    // If curr_distances_q is full (contains 5 elements), move the front element to lag_distances_q
    if (curr_distances_q.size() == CURR_DIST_LEN) {
        removed_curr_dist_val = curr_distances_q.front();
        // Remove the oldest value from curr_distances_q and adjust the running sum
        curr_distances_sum -= removed_curr_dist_val;
        curr_distances_q.pop();

        // Add the removed value to lag_distances_q and adjust the running sum
        lag_distances_q.push(removed_curr_dist_val);
        lag_distances_sum += removed_curr_dist_val;
    }

    // Add the new value to curr_distances_q and adjust the running sum
    curr_distances_q.push(distance);
    curr_distances_sum += distance;

    // keep state_distances_q limited in size
    if (state_distances_q.size() > STATE_DIST_MAX_SIZE) {
        // Remove the oldest value from curr_distances_q and adjust the running sum
        state_distances_sum -= state_distances_q.front();
        state_distances_q.pop();
    }
}

void update_velocity_calcs(float velocity) {
    // add to state_velocity_q no matter what
    state_velocity_q.push(velocity);
    state_velocity_sum += velocity;

    // keep state_velocity_q limited in size
    if (state_velocity_q.size() > STATE_DIST_MAX_SIZE) {
        state_velocity_sum -= state_velocity_q.front();
        state_velocity_q.pop();
    }
}

void update_averages() {
    // Calculate averages        
    curr_distances_avg = (curr_distances_q.empty()) ? 0.0 : curr_distances_sum / curr_distances_q.size();
    lag_distances_avg = (lag_distances_q.empty()) ? 0.0 : lag_distances_sum / lag_distances_q.size();
    state_distances_avg = (state_distances_q.empty()) ? 0.0 : state_distances_sum / state_distances_q.size();
    state_velocity_avg = (state_velocity_q.empty()) ? 0.0 : state_velocity_sum / state_velocity_q.size();
}