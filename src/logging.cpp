#include <Arduino.h>
#include "logging.h"

void debug_output(float time_diff, float current_time, float distance, int current_state, float curr_distances_avg, float lag_distances_avg, float state_distances_avg, float state_velocity_avg, float velocity) {
    // Serial.printf("Time Diff: %.4f sec | Distance: %.4f | Curr Dist Avg: %.4f | Lag Dist Avg: %.4f | State Dist Avg: %.4f | State Avg Velocity: %.4f | Velocity: %.4f | State: %d\n", 
    //     time_diff, distance, curr_distances_avg, lag_distances_avg, state_distances_avg, state_velocity_avg, velocity, current_state);
    String log_message = "";
    log_message += "Time Diff: " + String(time_diff, 3) + " sec | ";
    log_message += "Distance: " + String(distance, 3) + " | ";
    log_message += "Curr Dist Avg: " + String(curr_distances_avg, 3) + " | ";
    log_message += "Lag Dist Avg: " + String(lag_distances_avg, 3) + " | ";
    log_message += "State Dist Avg: " + String(state_distances_avg, 3) + " | ";
    log_message += "State Avg Velocity: " + String(state_velocity_avg, 2) + " | ";
    // log_message += "Velocity: " + String(velocity, 2) + " | ";
    log_message += "State: " + String(current_state);
    
    Serial.println(log_message);
    // Serial.print("Current time: ");
    // Serial.print(current_time / 1000000.0);
    // Serial.print(" -- ");
    // Serial.print("Free heap memory: ");
    // Serial.println(esp_get_free_heap_size());
}