#include "Arduino.h"
#include "prototypes.h"
#include "display.h"

// defines variables
long duration;
int distance;
bool c = true;

void setupUltrasound(uint8_t trigPin, uint8_t echoPin) {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}
void loopUltrasound(uint8_t trigPin, uint8_t echoPin) {
  // Clears the trigPin
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
  
  if (distance > 20)
  {
    // -1 back
    // 1 forward
    display_update_face(HEART_EYES);
    moveRobot(1, 80, 1);
    delay(5);
  }
  else {
    // -1 left
    // 1 right
    display_update_face(BIG_SMILE);
    moveRobot(1, 40, -1);
//    turnRobot(1, 80, -1);
    c = false;
  }

//  return distance;
}
