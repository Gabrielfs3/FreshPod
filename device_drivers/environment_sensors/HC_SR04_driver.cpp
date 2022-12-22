#include <Arduino.h>
#include "HC_SR04_driver.h"

// defines variables
long duration;
float distance;

 HC_SR04_driver:: HC_SR04_driver(int pinx, int piny)
{
    echoPin=pinx;
    trigPin=piny;
}
void HC_SR04_driver::Init()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

float HC_SR04_driver::get_dist()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance= duration*0.034/2;

    return distance;
}