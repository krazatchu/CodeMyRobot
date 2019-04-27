#include  "pins.h"
#include  "pcf8574_esp.h"


#define M_left_PWM_CHANNEL   1
#define M_left_RESOLUTION    8 // 8 bit resolution
#define M_right_PWM_CHANNEL  2
#define M_right_RESOLUTION   8 // 8 bit resolution

extern PCF857x expanderOne;

extern volatile long CurrentPositionRight;
extern volatile long CurrentPositionLeft;
volatile long CommandedPositionLeft = 0;
volatile long CommandedPositionRight = 0;

extern volatile long lastCurrentPositionRight;
extern volatile long lastCurrentPositionLeft;

volatile long IntegralErrorRight = 0;
volatile long IntegralErrorLeft  = 0;

int ProportionalGain = 35; // gains 125
int IntegralGain = 1;//1;
int DerivativeGain = 300; //  400

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);

  long VelocityRight =  ( lastCurrentPositionRight - CurrentPositionRight ); // most noise sensitive item goes first
  long VelocityLeft  =  ( lastCurrentPositionLeft  - CurrentPositionLeft  ); // most noise sensitive item goes first

  lastCurrentPositionRight = CurrentPositionRight; // save current postion as last position for next time
  lastCurrentPositionLeft  = CurrentPositionLeft;  // save current postion as last position for next time

  long PositionErrorRight = ( lastCurrentPositionRight - CommandedPositionRight ); // compute errors
  long PositionErrorLeft  = ( lastCurrentPositionLeft  - CommandedPositionLeft  ); // compute errors

  IntegralErrorRight += ( PositionErrorRight  * IntegralGain );
  IntegralErrorLeft  += ( PositionErrorLeft   * IntegralGain );

  if (IntegralErrorRight > 128) IntegralErrorRight = 128; // clip
  if (IntegralErrorLeft  > 128) IntegralErrorLeft  = 128;
  if (IntegralErrorRight < -128) IntegralErrorRight = -128; // clip
  if (IntegralErrorLeft  < -128) IntegralErrorLeft  = -128;

  long ProportionErrorRight =  (long) PositionErrorRight * ProportionalGain;
  long ProportionErrorLeft  =  (long) PositionErrorLeft  * ProportionalGain;

  long DerivativeErrorRight = ( VelocityRight  *  DerivativeGain );
  long DerivativeErrorLeft  = ( VelocityLeft   *  DerivativeGain );

  long TotalErrorRight = ((ProportionErrorRight + IntegralErrorRight - DerivativeErrorRight) / 256); // + 511; // divide off fractional & offset
  long TotalErrorLeft  = ((ProportionErrorLeft  + IntegralErrorLeft  - DerivativeErrorLeft)  / 256); // + 511; // divide off fractional & offset

  if (TotalErrorRight > 100) TotalErrorRight = 100; // clip
  if (TotalErrorLeft  > 100) TotalErrorLeft  = 100;
  if (TotalErrorRight < -100) TotalErrorRight = -100; // clip
  if (TotalErrorLeft  < -100) TotalErrorLeft  = -100;

  if (TotalErrorRight > 0) {
    digitalWrite (motorRightTwo, HIGH);
    ledcWrite(M_right_PWM_CHANNEL, TotalErrorRight);
  } else if  (TotalErrorRight < 0) {
    digitalWrite (motorRightTwo, LOW);
    ledcWrite(M_right_PWM_CHANNEL, TotalErrorRight);
  }
  if ((TotalErrorRight < 3) && (TotalErrorRight > -3)) {
    ledcWrite(M_right_PWM_CHANNEL, 0);
  }
 if (TotalErrorLeft > 0) {
    digitalWrite (motorLeftTwo, HIGH);
    ledcWrite(M_left_PWM_CHANNEL, TotalErrorLeft);
  } else if  (TotalErrorLeft < 0) {
    digitalWrite (motorLeftTwo, LOW);
    ledcWrite(M_left_PWM_CHANNEL, TotalErrorLeft);
  }
  if ((TotalErrorLeft < 3) && (TotalErrorLeft > -3)) {
     ledcWrite(M_left_PWM_CHANNEL, 0);
  }



  portEXIT_CRITICAL_ISR(&timerMux);

}


void setupMotor (void) {

  expanderOne.write (6, HIGH); // enable slp

  pinMode (motorRightOne, OUTPUT);
  pinMode (motorRightTwo, OUTPUT);
  pinMode (motorLeftOne, OUTPUT);
  pinMode (motorLeftTwo, OUTPUT);

  timer = timerBegin(0, 80, true); // 1 uS rate after prescaler of 80
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true);  // 500  // 500 Hz = 2000 uS
  timerAlarmEnable(timer);

  ledcSetup(M_left_PWM_CHANNEL, 20000, M_left_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorLeftOne, M_left_PWM_CHANNEL);          // Attach channel to pin
  ledcSetup(M_right_PWM_CHANNEL, 20000, M_right_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorRightOne, M_right_PWM_CHANNEL);          // Attach channel to pin

  ledcWrite(M_left_PWM_CHANNEL, 0); // set PWM to off
  ledcWrite(M_right_PWM_CHANNEL, 0);

  digitalWrite (motorLeftTwo, LOW);
  digitalWrite (motorRightTwo, LOW);
}

void goForward (void) {

  digitalWrite (motorRightOne, HIGH);
  digitalWrite (motorRightTwo, LOW);

  digitalWrite (motorLeftOne, HIGH);
  digitalWrite (motorLeftTwo, LOW);
}

void goBackward (void) {

  digitalWrite (motorRightOne, LOW);
  digitalWrite (motorRightTwo, HIGH);

  digitalWrite (motorLeftOne, LOW);
  digitalWrite (motorLeftTwo, HIGH);
}



void goSpin (void) {

  digitalWrite (motorRightOne, HIGH);
  digitalWrite (motorRightTwo, LOW);

  digitalWrite (motorLeftOne, LOW);
  digitalWrite (motorLeftTwo, HIGH);
}


void goStop (void) {

  digitalWrite (motorRightOne, LOW);
  digitalWrite (motorRightTwo, LOW);

  digitalWrite (motorLeftOne, LOW);
  digitalWrite (motorLeftTwo, LOW);
}
