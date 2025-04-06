#include "sensor.h"

void init_sensor() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(115200);
}

long read_sensor() {
    long reading;
    // Clears the trigPin
    digitalWrite(TRIG_PIN, LOW);
    // delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    reading = pulseIn(ECHO_PIN, HIGH);
    return reading;
}