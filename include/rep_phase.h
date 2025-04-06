#ifndef REP_PHASE_H
#define REP_PHASE_H

#include <Arduino.h>
#include "distance.h"

#define DEBUG 1
// Rep phases
#define IDLE 0
#define ECC 1
#define CONC 2

// min number of ticks before a phase can be exited
const int PHASE_MIN_TICKS = 5;
// distance reduction to signify an eccentric phase
const float ECC_REP_THRESH = 0.03;
// distance increase to signify a concentric phase
const float CONC_REP_THRESH = 0.005;
// distance from previously calculated top_of_rep to say we may be at top again
const float TOP_REP_THRESH = 0.01;
// curr_distances_avg & lag_distances avg within this amount during concentric rep
// indicates transition to idle
const float CONC_IDLE_THRESH = 0.01;

extern int phase_ticks;
// state of the previous loop
extern int old_state;
// Top of rep to help determine when CONC phase over
extern float top_of_rep;

int get_state(int current_state, float current_state_avg, float curr_distances_avg, float lag_distances_avg);
void handle_state_change(float velocity);

#endif
