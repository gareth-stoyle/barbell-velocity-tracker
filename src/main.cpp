#include <Arduino.h>
#include "main.h"
#include "logging.h"
#include "sensor.h"
#include "distance.h"
#include "rep_phase.h"
#include <queue>
#include <cassert>

void setup() {
    init_sensor();
    old_time = micros();
    delayMicroseconds(10000);
}

bool is_outlier(float distance, float avg) {
    return fabs(distance - avg) > (OUTLIER_THRESH * avg);
}

void loop() {
    float time_diff = 0.0f; // seconds
    long duration = 0;
    float distance = 0.0f;
    float velocity = 0.0f;
    unsigned long current_time = 0;
    int current_state = old_state;

    if (ticks <= 1000) {
        // Update time
        current_time = micros();
        time_diff = (current_time - old_time) / 1000000.0;  // seconds
        duration = read_sensor();  // Calculating the distance in meters
        distance = (duration * SPEED_OF_SOUND / 2) / 100;
        velocity = (distance - previous_distance) / time_diff;

        // stop velocity from starting off with ridiculously high reading
        if (previous_distance == 0.0) velocity = 0.0;

        if (!curr_distances_q.empty() && is_outlier(distance, curr_distances_avg)) {
            Serial.println("Outlier detected, ignoring distance value:");
            Serial.println(distance);
            ticks++;
            return;
        }

        update_distance_calcs(distance, velocity);
        update_velocity_calcs(velocity);
        update_averages();

        current_state = get_state(current_state, state_distances_avg, curr_distances_avg, lag_distances_avg);
        
        if (current_state != old_state) handle_state_change(velocity);

        if (DEBUG && ticks % 25 == 1) {
            debug_output(time_diff, current_time, distance, current_state, curr_distances_avg, lag_distances_avg, state_distances_avg, state_velocity_avg, velocity);
        }

        old_time = current_time;
        old_state = current_state;
        previous_distance = distance;
        ticks++;
        delayMicroseconds(350);
    }
}
