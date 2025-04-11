#include <Arduino.h>
#include "main.h"
#include "logging.h"
#include "sensor.h"
#include "distance.h"
#include "rep_state.h"
#include "rep_phase.h"
#include "bluetooth.h"

RepState rep_state;

void setup() {
    Serial.begin(115200);
    bluetooth_setup();
    while (!deviceConnected) {
        delay(1000);  
    }
    init_sensor();
    old_time = micros();
    delayMicroseconds(10000);
}


void loop() {
    float time_diff = 0.0f; // seconds
    long duration = 0;
    float distance = 0.0f;
    float velocity = 0.0f;
    unsigned long current_time = 0;
    PhaseState current_state = old_state;

    if (ticks <= MAX_TICKS && deviceConnected) {
        // Update time
        current_time = micros();
        time_diff = (current_time - old_time) / MICROSECONDS_PER_SECOND;  // seconds
        duration = read_sensor();  // Calculating the distance in meters
        distance = (duration * SPEED_OF_SOUND / 2) / 100;
        velocity = (distance - rep_state.previous_distance) / time_diff;

        if (!rep_state.curr_distances_q.empty() && is_outlier(distance, rep_state.curr_distances_avg)) {
            Serial.println("Outlier detected, ignoring distance value:");
            Serial.println(distance);
            ticks++;
            return;
        }

        // stop velocity from starting off with ridiculously high reading
        if (rep_state.previous_distance == 0.0) velocity = 0.0;
        if (velocity > rep_state.peak_velocity) rep_state.peak_velocity = velocity;

        update_distance_calcs(rep_state, distance, velocity);
        update_velocity_calcs(rep_state, velocity);
        update_averages(rep_state);

        current_state = get_state(current_state, rep_state);
        
        if (current_state != old_state) handle_state_change(rep_state, velocity);

        if (DEBUG && ticks % 25 == 1) {
            debug_output(time_diff, current_time, distance, current_state, rep_state, velocity);
        }

        old_time = current_time;
        old_state = current_state;
        rep_state.previous_distance = distance;
        delayMicroseconds(LOOP_DELAY);
        ticks++;
    }
}
