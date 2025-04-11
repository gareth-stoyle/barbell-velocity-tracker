#include "distance.h"


void update_distance_calcs(RepState& rep_state, SensorReading& sensor_reading) {
    float removed_curr_dist_val = 0.0f;
    float removed_state_dist_val = 0.0f;
    assert(rep_state.lag_distances_q.size() <= rep_state.curr_distances_q.size());

    // If lag_distances_q is full, move the front element to state_distances_q
    if (rep_state.lag_distances_q.size() == LAG_DIST_LEN) {
        removed_state_dist_val = rep_state.lag_distances_q.front();
        rep_state.lag_distances_sum -= removed_state_dist_val;
        rep_state.lag_distances_q.pop();

        rep_state.state_distances_q.push(removed_state_dist_val);
        rep_state.state_distances_sum += removed_state_dist_val;
    }

    // If curr_distances_q is full (contains 5 elements), move the front element to lag_distances_q
    if (rep_state.curr_distances_q.size() == CURR_DIST_LEN) {
        removed_curr_dist_val = rep_state.curr_distances_q.front();
        // Remove the oldest value from curr_distances_q and adjust the running sum
        rep_state.curr_distances_sum -= removed_curr_dist_val;
        rep_state.curr_distances_q.pop();

        // Add the removed value to lag_distances_q and adjust the running sum
        rep_state.lag_distances_q.push(removed_curr_dist_val);
        rep_state.lag_distances_sum += removed_curr_dist_val;
    }

    // Add the new value to curr_distances_q and adjust the running sum
    rep_state.curr_distances_q.push(sensor_reading.distance);
    rep_state.curr_distances_sum += sensor_reading.distance;

    // keep state_distances_q limited in size
    if (rep_state.state_distances_q.size() > STATE_DIST_MAX_SIZE) {
        // Remove the oldest value from curr_distances_q and adjust the running sum
        rep_state.state_distances_sum -= rep_state.state_distances_q.front();
        rep_state.state_distances_q.pop();
    }
}

void update_velocity_calcs(RepState& rep_state, SensorReading& sensor_reading) {
    // add to state_velocity_q no matter what
    rep_state.state_velocity_q.push(sensor_reading.velocity);
    rep_state.state_velocity_sum += sensor_reading.velocity;

    // keep state_velocity_q limited in size
    if (rep_state.state_velocity_q.size() > STATE_DIST_MAX_SIZE) {
        rep_state.state_velocity_sum -= rep_state.state_velocity_q.front();
        rep_state.state_velocity_q.pop();
    }
}

void update_averages(RepState& rep_state) {
    // Calculate averages        
    rep_state.curr_distances_avg = (rep_state.curr_distances_q.empty()) ? 0.0 : rep_state.curr_distances_sum / rep_state.curr_distances_q.size();
    rep_state.lag_distances_avg = (rep_state.lag_distances_q.empty()) ? 0.0 : rep_state.lag_distances_sum / rep_state.lag_distances_q.size();
    rep_state.state_distances_avg = (rep_state.state_distances_q.empty()) ? 0.0 : rep_state.state_distances_sum / rep_state.state_distances_q.size();
    rep_state.state_velocity_avg = (rep_state.state_velocity_q.empty()) ? 0.0 : rep_state.state_velocity_sum / rep_state.state_velocity_q.size();
}

bool is_outlier(float distance, float avg) {
    return fabs(distance - avg) > (OUTLIER_THRESH * avg);
}