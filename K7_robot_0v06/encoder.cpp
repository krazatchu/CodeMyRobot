
#include  "pins.h"
#include  "pcf8574_esp.h"

extern PCF857x expanderOne;
//volatile int interruptCounter = 0;
//int numberOfInterrupts = 0;

volatile byte lastEncodedRight = 0; // used for quadrature decode
volatile long CurrentPositionRight  = 0;
volatile long lastCurrentPositionRight  = 0;

volatile byte lastEncodedLeft = 0; // used for quadrature decode
volatile long CurrentPositionLeft  = 0;
volatile long lastCurrentPositionLeft  = 0;



portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR handleLeftInterrupt() {
  portENTER_CRITICAL_ISR(&mux);

  byte MSB = digitalRead(encoderLeftA); //
  byte LSB = digitalRead(encoderLeftB); //

  byte encoded = (MSB << 1) | LSB; // convert the 2 pins status to single number
  byte sum  = (lastEncodedLeft << 2) | encoded; // add in previous status

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) CurrentPositionLeft ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) CurrentPositionLeft --;

  lastEncodedLeft = encoded; // store for next

  portEXIT_CRITICAL_ISR(&mux);
}



void IRAM_ATTR handleRightInterrupt() {
  portENTER_CRITICAL_ISR(&mux);

  byte MSB = digitalRead(encoderRightA); //
  byte LSB = digitalRead(encoderRightB); //

  byte encoded = (MSB << 1) | LSB; // convert the 2 pins status to single number
  byte sum  = (lastEncodedRight << 2) | encoded; // add in previous status

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) CurrentPositionRight ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) CurrentPositionRight --;

  lastEncodedRight = encoded; // store for next

  portEXIT_CRITICAL_ISR(&mux);
}




void encoderEnable (void) {
  expanderOne.write(0, LOW);
  expanderOne.write(1, LOW);
  attachInterrupt(digitalPinToInterrupt(encoderRightA), handleRightInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderRightB), handleRightInterrupt, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderLeftA), handleLeftInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderLeftB), handleLeftInterrupt, CHANGE);
}

void encoderDisable (void) {
  expanderOne.write(0, HIGH);
  expanderOne.write(1, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoderRightA), handleRightInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderRightB), handleRightInterrupt, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderLeftA), handleLeftInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderLeftB), handleLeftInterrupt, CHANGE);
}


void handleInt (void) {
  if ( CurrentPositionRight != lastCurrentPositionRight) {
    Serial.print("RIGHT ENC POS: ");
    Serial.println(CurrentPositionRight);
    //lastCurrentPositionRight = CurrentPositionRight;

  }
  if ( CurrentPositionLeft != lastCurrentPositionLeft) {
    Serial.print("LEFT ENC POS: ");
    Serial.println(CurrentPositionLeft);
  //  lastCurrentPositionLeft = CurrentPositionLeft;
  }
  // portENTER_CRITICAL(&mux);
  // interruptCounter--;
  // portEXIT_CRITICAL(&mux);
  // numberOfInterrupts++;
}
