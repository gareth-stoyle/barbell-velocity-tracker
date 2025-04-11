#include "sensor.h"

void init_sensor() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void read_sensor(SensorReading& sensor_reading, float previous_distance, float time_diff) {
    long reading;
    // Clears the trigPin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    sensor_reading.duration = pulseIn(ECHO_PIN, HIGH);
    sensor_reading.distance = (sensor_reading.duration * SPEED_OF_SOUND / 2) / 100;
    sensor_reading.velocity = (sensor_reading.distance - previous_distance) / time_diff;
}