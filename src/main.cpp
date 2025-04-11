#include <Arduino.h>
#include "main.h"
#include "logging.h"
#include "sensor.h"
#include "distance.h"
#include "rep_state.h"
#include "rep_phase.h"
#include "bluetooth.h"

RepState rep_state;
SensorReading sensor_reading;

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
    unsigned long current_time = 0;
    PhaseState current_state = old_state;

    if (ticks <= MAX_TICKS && deviceConnected) {
        // Update time
        current_time = micros();
        time_diff = (current_time - old_time) / MICROSECONDS_PER_SECOND;  // seconds

        read_sensor(sensor_reading, rep_state.previous_distance, time_diff);  // Calculating the distance in meters

        if (!rep_state.curr_distances_q.empty() && is_outlier(sensor_reading.distance, rep_state.curr_distances_avg)) {
            Serial.println("Outlier detected, ignoring distance value:");
            Serial.println(sensor_reading.distance);
            ticks++;
            return;
        }

        // stop velocity from starting off with ridiculously high reading
        if (rep_state.previous_distance == 0.0) sensor_reading.velocity = 0.0;
        if (sensor_reading.velocity > rep_state.peak_velocity) rep_state.peak_velocity = sensor_reading.velocity;

        update_distance_calcs(rep_state, sensor_reading);
        update_velocity_calcs(rep_state, sensor_reading);
        update_averages(rep_state);

        current_state = get_state(current_state, rep_state);
        
        if (current_state != old_state) handle_state_change(rep_state, sensor_reading);

        if (DEBUG && ticks % 25 == 1) {
            debug_output(time_diff, current_time, current_state, rep_state, sensor_reading);
        }

        old_time = current_time;
        old_state = current_state;
        rep_state.previous_distance = sensor_reading.distance;
        delayMicroseconds(LOOP_DELAY);
        ticks++;
    }
}
