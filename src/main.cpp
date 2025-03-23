#include <Arduino.h>

// defines consts
const int trigPin = 19;
const int echoPin = 18;
const float speedOfSound = 0.034; // Speed of sound in m/s
const float maxDistance = 2.0; // Maximum distance in m, anything else is anomolous
const float minDistance = 0.02; // Minimum distance in m, anything else is anomolous

// defines variables
long duration;
float old_distance, current_distance, distance_diff;
unsigned long old_time, current_time;
float time_diff; // seconds
float bar_velocity;
float max_bar_velocity;
int ticks;

void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(115200); // Starts the serial communication
    old_time = micros();
    ticks = 0;
    old_distance = 0.0;
    max_bar_velocity = 0.0;
}


void loop() {
    if (ticks < 200) {
        // Clears the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
        // Calculating the distance in meters
        current_distance = (duration * speedOfSound / 2) / 100;

        // If the distance is not within the range, assume it's anomolous and skip
        if (current_distance > maxDistance || current_distance < minDistance) {
            printf("Skipping anomolous distance: %d\n", current_distance);
            return;
        }
        distance_diff = current_distance - old_distance;

        Serial.printf("Old Distance: %.4f - Distance: %.4f - Distance Diff: %.4f\n", old_distance, current_distance, distance_diff);

        current_time = micros();
        time_diff = (current_time - old_time) / 1000000.0; // seconds
        Serial.printf("Old Time: %lu - Time: %lu - Time Diff (seconds): %.6f\n", old_time, current_time, time_diff);

        bar_velocity = distance_diff / time_diff;
        if (ticks < 10) {
            ticks++;
            old_distance = current_distance;
            old_time = current_time;
            delayMicroseconds(20000);
            return;  // Skip first 10 iterations
        }
        else if (bar_velocity > max_bar_velocity) {
            max_bar_velocity = bar_velocity;
        }

        Serial.printf("Bar Velocity: %.4f -- Max Bar Velocity: %.4f\n\n", bar_velocity, max_bar_velocity);


        old_distance = current_distance;
        old_time = current_time;
        ticks++;
        delayMicroseconds(20000);
    }
}
