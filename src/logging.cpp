#include <Arduino.h>
#include "logging.h"

void debug_output(float time_diff, float current_time, float distance, PhaseState current_state, RepState& rep_state, float velocity) {
    String log_message = "";
    log_message += "Time Diff: " + String(time_diff, 3) + " sec | ";
    log_message += "Distance: " + String(distance, 3) + " | ";
    log_message += "Curr Dist Avg: " + String(rep_state.curr_distances_avg, 3) + " | ";
    log_message += "Lag Dist Avg: " + String(rep_state.lag_distances_avg, 3) + " | ";
    log_message += "State Dist Avg: " + String(rep_state.state_distances_avg, 3) + " | ";
    log_message += "State Avg Velocity: " + String(rep_state.state_velocity_avg, 2) + " | ";
    // log_message += "Velocity: " + String(velocity, 2) + " | ";
    log_message += "State: " + String(phaseStateToStr(current_state));
    
    Serial.println(log_message);
    // Serial.print("Current time: ");
    // Serial.print(current_time / 1000000.0);
    // Serial.print(" -- ");
    // Serial.print("Free heap memory: ");
    // Serial.println(esp_get_free_heap_size());
}