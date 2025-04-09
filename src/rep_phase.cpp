#include "rep_phase.h"
#include "bluetooth.h"

int phase_ticks = 0;
int old_state = IDLE;
float top_of_rep = 0.0f;

int get_state(int current_state, float current_state_avg, float curr_distances_avg, float lag_distances_avg) {
    // IDLE state: take current state average and current rolling tick average
    // check if threshold is crossed 
    phase_ticks++;
    if (current_state == IDLE && phase_ticks >= PHASE_MIN_TICKS) {
        if (current_state_avg - curr_distances_avg > ECC_REP_THRESH) {
            // update top of rep to be whatever the average of the last IDLE state was.
            top_of_rep = current_state_avg;
            phase_ticks = 1;
            return ECC;
        }
    }

    // ECC state: take lag rolling tick average and current rolling tick average
    // if current - lagging is >= a threshold, move state to concentric
    if (current_state == ECC && phase_ticks >= PHASE_MIN_TICKS) {
        if ((curr_distances_avg - lag_distances_avg) >= CONC_REP_THRESH) {
            phase_ticks = 1;
            return CONC;
        }
    }

    // CONC state: detect transition to IDLE (rep completed)
    if (current_state == CONC && phase_ticks >= PHASE_MIN_TICKS) {
        // Check if the movement has slowed significantly from curr ticks to lagging ticks
        if (fabs(curr_distances_avg - lag_distances_avg) <= CONC_IDLE_THRESH) {
            // Check if we are close to or above `top_of_rep` (rep is complete)
            if ((top_of_rep - curr_distances_avg) <= TOP_REP_THRESH) {
                phase_ticks = 1;
                return IDLE;
            }
        }
    }

    // If none match, state stays the same
    return current_state;
}

void handle_state_change(float velocity) {
    // If state changed, empty Q, and do any other necessary output / calcs
    // if (DEBUG) {
    //     Serial.printf("Phase: %d -- Mean Vel: %.4f m/s\n", old_state, state_velocity_avg);
    // }
    // if (old_state == CONC) {
    //     // cast state_velocity_avg as string and send bluetooth notification
    //     char buf[16];
    //     snprintf(buf, sizeof(buf), "%.4f", state_velocity_avg);
    //     pCharacteristic->setValue(buf);
    //     pCharacteristic->notify();
    // }
    if (DEBUG) {
        char debugBuf[64];
        snprintf(debugBuf, sizeof(debugBuf), "Phase: %d -- Mean Vel: %.4f m/s\n", old_state, state_velocity_avg);
        pCharacteristic->setValue(debugBuf);
        pCharacteristic->notify();
    }

    state_distances_q = curr_distances_q;
    state_distances_sum = curr_distances_sum;
    state_distances_avg = curr_distances_avg;

    // reset state velocity measurements
    state_velocity_q = {};
    state_velocity_q.push(velocity);
    state_velocity_sum = velocity;
    state_velocity_avg = velocity;

    assert(phase_ticks >= state_velocity_q.size());
}