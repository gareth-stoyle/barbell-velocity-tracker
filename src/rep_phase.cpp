#include "rep_phase.h"
#include "bluetooth.h"

String data_buf = "";
int phase_ticks = 0;
PhaseState old_state = PhaseState::IDLE;
float top_of_rep = 0.0f;

const char* phaseStateToStr(PhaseState state) {
    switch (state) {
        case PhaseState::IDLE: return "IDLE";
        case PhaseState::ECC:  return "ECC";
        case PhaseState::CONC: return "CONC";
        default: return "UNKNOWN";
    }
}

PhaseState get_state(PhaseState current_state, RepState& rep_state) {
    // IDLE state: take current state average and current rolling tick average
    // check if threshold is crossed 
    phase_ticks++;
    if (current_state == PhaseState::IDLE && phase_ticks >= PHASE_MIN_TICKS) {
        if (rep_state.state_distances_avg - rep_state.curr_distances_avg > ECC_REP_THRESH) {
            // update top of rep to be whatever the average of the last IDLE state was.
            top_of_rep = rep_state.state_distances_avg;
            phase_ticks = 1;
            return PhaseState::ECC;
        }
    }

    // ECC state: take lag rolling tick average and current rolling tick average
    // if current - lagging is >= a threshold, move state to concentric
    if (current_state == PhaseState::ECC && phase_ticks >= PHASE_MIN_TICKS) {
        if ((rep_state.curr_distances_avg - rep_state.lag_distances_avg) >= CONC_REP_THRESH && rep_state.previous_state_avg - rep_state.curr_distances_avg >= CONC_REP_DIST_THRESH) {
            phase_ticks = 1;
            return PhaseState::CONC;
        }
    }

    // CONC state: detect transition to IDLE (rep completed)
    if (current_state == PhaseState::CONC && phase_ticks >= PHASE_MIN_TICKS) {
        // Check if the movement has slowed significantly from curr ticks to lagging ticks
        if (fabs(rep_state.curr_distances_avg - rep_state.lag_distances_avg) <= CONC_IDLE_THRESH) {
            // Check if we are close to or above `top_of_rep` (rep is complete)
            if ((top_of_rep - rep_state.curr_distances_avg) <= TOP_REP_THRESH) {
                phase_ticks = 1;
                return PhaseState::IDLE;
            }
        }
    }

    // If none match, state stays the same
    return current_state;
}

void handle_state_change(RepState& rep_state, float velocity) {
    // If state changed, empty Q, and do any other necessary output / calcs
    if (DEBUG) {
        Serial.printf("Phase: %d -- Mean Vel: %.2f m/s -- Peak Vel: %.2f\n", old_state, rep_state.state_velocity_avg, rep_state.peak_velocity);
    }

    if (old_state == PhaseState::CONC) {
        // no comma
        data_buf += String(rep_state.state_velocity_avg, 2);
        // Send bluetooth notification
        pCharacteristic->setValue(data_buf.c_str());
        pCharacteristic->notify();
        data_buf = "";
    } else {
        data_buf += String(rep_state.state_velocity_avg, 2) + ",";
    }

    rep_state.previous_state_avg = rep_state.state_distances_avg;

    rep_state.state_distances_q = rep_state.curr_distances_q;
    rep_state.state_distances_sum = rep_state.curr_distances_sum;
    rep_state.state_distances_avg = rep_state.curr_distances_avg;

    // reset state velocity measurements
    rep_state.state_velocity_q = {};
    rep_state.state_velocity_q.push(velocity);
    rep_state.state_velocity_sum = velocity;
    rep_state.state_velocity_avg = velocity;

    rep_state.peak_velocity = 0.0;

    assert(phase_ticks >= rep_state.state_velocity_q.size());
}