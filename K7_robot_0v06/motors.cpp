#include  "pins.h"
#include  "pcf8574_esp.h"


#define M_left_PWM_CHANNEL_one   12
#define M_left_RESOLUTION    8 // 8 bit resolution
#define M_right_PWM_CHANNEL_one  13
#define M_right_RESOLUTION   8 // 8 bit resolution
#define M_left_PWM_CHANNEL_two   14
#define M_right_PWM_CHANNEL_two  15

uint8_t motorRightOne = 2;
uint8_t motorRightTwo = 4;
uint8_t motorLeftOne = 23;
uint8_t motorLeftTwo = 19;
/*
uint8_t motorRightOne = 2;
uint8_t motorRightTwo = 4;
uint8_t motorLeftOne = 23 ;
uint8_t motorLeftTwo = 19;
*/
extern PCF857x expanderOne;

extern volatile long CurrentPositionRight;
extern volatile long CurrentPositionLeft;
volatile long CommandedPositionLeft = 0;
volatile long CommandedPositionRight = 0;

extern volatile long lastCurrentPositionRight;
extern volatile long lastCurrentPositionLeft;

volatile long IntegralErrorRight = 0;
volatile long IntegralErrorLeft  = 0;

int ProportionalGain = 200; // gains 135 with 8 div
int IntegralGain = 25;//15;
int DerivativeGain = 240; //  135

long TotalErrorRight;
long TotalErrorLeft;

hw_timer_t * tMotor = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR onMotorTimer() {
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

  TotalErrorRight = ((ProportionErrorRight + IntegralErrorRight - DerivativeErrorRight) / 8 );// / 256); // + 511; // divide off fractional & offset
  TotalErrorLeft  = ((ProportionErrorLeft  + IntegralErrorLeft  - DerivativeErrorLeft)  / 8); // + 511; // divide off fractional & offset


  if (TotalErrorRight > 150) TotalErrorRight = 150; // clip
  if (TotalErrorLeft  > 150) TotalErrorLeft  = 150;
  if (TotalErrorRight < -150) TotalErrorRight = -150; // clip
  if (TotalErrorLeft  < -150) TotalErrorLeft  = -150;

  /*
  if (TotalErrorRight > 0) {
    int16_t errSwapPlus =  255 - TotalErrorRight;
    
    //ledcWrite(M_right_PWM_CHANNEL_two, 255);
    //ledcWrite(M_right_PWM_CHANNEL_one, errSwapPlus);

  } else if  (TotalErrorRight < 0) {
    errSwap = 255 -  (TotalErrorRight * -1);
    //ledcWrite(M_right_PWM_CHANNEL_two, errSwap);
    //ledcWrite(M_right_PWM_CHANNEL_one, 255);
  }

  if (TotalErrorLeft > 0) {
    int16_t errSwapPlus =  255 - TotalErrorLeft;
    //ledcWrite(M_left_PWM_CHANNEL_two, 255);
    //ledcWrite(M_left_PWM_CHANNEL_one, errSwapPlus);

  } else if  (TotalErrorLeft < 0) {
    errSwap = 255 -  (TotalErrorLeft * -1);
    //ledcWrite(M_left_PWM_CHANNEL_two, errSwap);
    //ledcWrite(M_left_PWM_CHANNEL_one, 255);
  }
  */
  portEXIT_CRITICAL_ISR(&timerMux);

}

void pwm_setup()
{
  ledcSetup(M_left_PWM_CHANNEL_one, 10000, M_left_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorLeftOne, M_left_PWM_CHANNEL_one);          // Attach channel to pin
  ledcSetup(M_right_PWM_CHANNEL_one, 10000, M_right_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorRightOne, M_right_PWM_CHANNEL_one);          // Attach channel to pin

  ledcWrite(M_left_PWM_CHANNEL_one, 0); // set PWM to off
  ledcWrite(M_right_PWM_CHANNEL_one, 0);

  ledcSetup(M_left_PWM_CHANNEL_two, 10000, M_left_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorLeftTwo, M_left_PWM_CHANNEL_two);          // Attach channel to pin
  ledcSetup(M_right_PWM_CHANNEL_two, 10000, M_right_RESOLUTION);  // Set up PWM channel
  ledcAttachPin(motorRightTwo, M_right_PWM_CHANNEL_two);          // Attach channel to pin

  ledcWrite(M_left_PWM_CHANNEL_two, 0); // set PWM to off
  ledcWrite(M_right_PWM_CHANNEL_two, 0);
}

void testMotorDirection()
{
  uint8_t counter = 0;

  pwm_setup();
  
  // Test left motor direction
  Serial.printf("[Testing Left] pin 1=%d, pin 2=%d\r\n", motorLeftOne, motorLeftTwo);
  Serial.printf("[Right] pin 1=%d, pin 2=%d\r\n", motorRightOne, motorRightTwo);

  counter = 0;
  CurrentPositionRight = 0;
  CurrentPositionLeft = 0;

  //digitalWrite (motorLeftTwo, LOW);
  ledcWrite(M_left_PWM_CHANNEL_one, 255); //
  //digitalWrite (motorLeftOne, HIGH);
  ledcWrite(M_left_PWM_CHANNEL_two, 128);

  while ((CurrentPositionLeft < 16 && CurrentPositionLeft > -16) && (CurrentPositionRight < 16 && CurrentPositionRight > -16))
  {
    Serial.printf("[Current Position] Left: %d, Right: %d\r\n", CurrentPositionLeft, CurrentPositionRight);
    counter++;
    if (counter == 0)
    {
      break;
    }
  }

  //digitalWrite(motorLeftOne, LOW);
  ledcWrite(M_left_PWM_CHANNEL_two, 255);
  delay(1000);

  // Adjust pins based on information recevied
  if (CurrentPositionLeft <= -16)
  {
    motorLeftOne = 23;
    motorLeftTwo = 19;
    Serial.printf("Left motor direction was wrong, corrected to pin 1=%d, pin 2=%d\r\n", motorLeftOne, motorLeftTwo);
  }
  if ((CurrentPositionRight >= 16) || (CurrentPositionRight <= -16))
  {
    Serial.println("Whoops, tested right accidentally");
    // Pins assigned to left actually controls right motor
    if (CurrentPositionRight >= 16)
    {
      Serial.println("Right motor direction was correct");
      motorRightOne = 23;
      motorRightOne = 19;
    }
    if (CurrentPositionRight <= -16)
    {
      motorRightOne = 19;
      motorRightOne = 23;
      Serial.printf("Right motor direction was wrong, corrected to pin 1=%d, pin 2=%d\r\n", motorLeftOne, motorLeftTwo);
    }

    motorLeftOne = 2;
    motorLeftTwo = 4;
    // Test the adjusted left motor

    pwm_setup();

    //digitalWrite (motorLeftTwo, LOW);
    ledcWrite(M_left_PWM_CHANNEL_one, 255); //
    //digitalWrite (motorLeftOne, HIGH);
    ledcWrite(M_left_PWM_CHANNEL_two, 128);

    counter = 0;
    CurrentPositionRight = 0;
    CurrentPositionLeft = 0;
    while ((CurrentPositionLeft < 16 && CurrentPositionLeft > -16) && (CurrentPositionRight < 16 && CurrentPositionRight > -16))
    {
      Serial.printf("[Current Position] Left: %d, Right: %d\r\n", CurrentPositionLeft, CurrentPositionRight);
      counter++;
      if (counter == 0)
      {
        break;
      }
    }
    //digitalWrite(motorLeftOne, LOW);
    ledcWrite(M_left_PWM_CHANNEL_two, 255);
    delay(1000);

    if (CurrentPositionLeft <= -16)
    {
      motorLeftOne = 4;
      motorLeftTwo = 2;
    }
  }
  else
  {
    Serial.println("Left test complete");

    pwm_setup();

    Serial.printf("[Testing Right] pin 1=%d, pin 2=%d\r\n", motorRightOne, motorRightTwo);
    Serial.printf("[Left] pin 1=%d, pin 2=%d\r\n", motorLeftOne, motorLeftTwo);
    CurrentPositionRight = 0;
    CurrentPositionLeft = 0;

    // digitalWrite (motorRightTwo, LOW);
    ledcWrite(M_right_PWM_CHANNEL_one, 255); //
    // digitalWrite (motorRightOne, HIGH);
    ledcWrite(M_right_PWM_CHANNEL_two, 128);


    counter = 0;
    while ((CurrentPositionLeft < 16 && CurrentPositionLeft > -16) && (CurrentPositionRight < 16 && CurrentPositionRight > -16))
    {
      Serial.printf("[Current Position] Left: %d, Right: %d\r\n", CurrentPositionLeft, CurrentPositionRight);
      counter++;
      if (counter == 0)
      {
        break;
      }
    }
    // digitalWrite (motorRightOne, LOW);
    ledcWrite(M_right_PWM_CHANNEL_two, 255);
    delay(1000);

    if (CurrentPositionRight < -16)
    {
      motorRightOne = 2;
      motorRightTwo = 4;
      Serial.printf("Right motor direction was wrong, corrected to pin 1=%d, pin 2=%d", motorRightOne, motorRightTwo);
    }
  }
  
  pwm_setup();

  // Go Forward after test 
/*
  digitalWrite (motorLeftTwo, LOW);
  digitalWrite (motorRightTwo, LOW);
  digitalWrite (motorLeftOne, HIGH);
  digitalWrite (motorRightOne, HIGH);
  delay(500);
  digitalWrite (motorLeftOne, LOW);
  digitalWrite (motorRightOne, LOW);
*/ 
}

void setupMotor (void) {

  expanderOne.write (6, HIGH); // enable slp

  pinMode (motorRightOne, OUTPUT);
  pinMode (motorRightTwo, OUTPUT);
  pinMode (motorLeftOne, OUTPUT);
  pinMode (motorLeftTwo, OUTPUT);



  testMotorDirection();

  pwm_setup();

  tMotor = timerBegin(3, 80, true); // 1 uS rate after prescaler of 80
  timerAttachInterrupt(tMotor, &onMotorTimer, true);
  timerAlarmWrite(tMotor, 10000, true);  // 500  // 500 Hz = 2000 uS
  timerAlarmEnable(tMotor);
  //digitalWrite (motorLeftTwo, LOW);
  //digitalWrite (motorRightTwo, LOW);
}


int16_t velocity = 0;
long targetPositionRight = 0;
long targetPositionLeft  = 0;
unsigned long previousMillis = 0;
int16_t turnt = 0;

void motionHandler (void) {
  //static uint32_t cnt = 0;

  static unsigned long currentMillis = 0;
  int16_t errSwapPlus;
  int16_t errSwap;
    
  if (turnt != 0) {

    currentMillis = millis();
    if (currentMillis - previousMillis >= 10 ) {
      previousMillis = currentMillis;

      CommandedPositionRight += turnt;
      CommandedPositionLeft  -= turnt;

      if ( (turnt > 0) && (targetPositionRight <= CurrentPositionRight) ) turnt = 0; // check for stop
      if ( (turnt > 0) && (targetPositionLeft  >=  CurrentPositionLeft) ) turnt = 0;

      if ( (turnt < 0) && (targetPositionRight >= CurrentPositionRight) ) turnt = 0;
      if ( (turnt < 0) && (targetPositionLeft  <=  CurrentPositionLeft) ) turnt = 0;

      if (turnt == 0) {
        CommandedPositionRight = targetPositionRight;
        CommandedPositionLeft  = targetPositionLeft;
      }
    }
  }

  if ( velocity != 0) {
    
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10 ) {
      previousMillis = currentMillis;

      CommandedPositionRight += velocity;
      CommandedPositionLeft  += velocity;

      if ( (velocity > 0) && (targetPositionRight <= CurrentPositionRight) ) velocity = 0; // check for stop
      if ( (velocity > 0) && (targetPositionLeft  <=  CurrentPositionLeft) ) velocity = 0;

      if ( (velocity < 0) && (targetPositionRight >= CurrentPositionRight) ) velocity = 0;
      if ( (velocity < 0) && (targetPositionLeft  >=  CurrentPositionLeft) ) velocity = 0;

      if (velocity == 0) {
        CommandedPositionRight = targetPositionRight;
        CommandedPositionLeft  = targetPositionLeft;
      }
    }
  }  
    
  if (TotalErrorRight > 0) {
    errSwapPlus =  255 - TotalErrorRight;
    
    ledcWrite(M_right_PWM_CHANNEL_two, 255);
    ledcWrite(M_right_PWM_CHANNEL_one, errSwapPlus);

  } else if  (TotalErrorRight < 0) {
    errSwap = 255 -  (TotalErrorRight * -1);
    ledcWrite(M_right_PWM_CHANNEL_two, errSwap);
    ledcWrite(M_right_PWM_CHANNEL_one, 255);
  }

  if (TotalErrorLeft > 0) {
    errSwapPlus =  255 - TotalErrorLeft;
    ledcWrite(M_left_PWM_CHANNEL_two, 255);
    ledcWrite(M_left_PWM_CHANNEL_one, errSwapPlus);

  } else if  (TotalErrorLeft < 0) {
    errSwap = 255 -  (TotalErrorLeft * -1);
    ledcWrite(M_left_PWM_CHANNEL_two, errSwap);
    ledcWrite(M_left_PWM_CHANNEL_one, 255);
  }

}

void moveRobot (uint8_t robotSpeed, uint32_t robotSteps,  int8_t robotDirection) {

  if ( (robotSpeed > 0) &&  (robotDirection != 0) && (robotSteps > 0) ) {

    if ( robotDirection == 1 ) {
      velocity =  robotSpeed;
      targetPositionRight = CurrentPositionRight + robotSteps;
      targetPositionLeft  = CurrentPositionLeft  + robotSteps;
    } else if ( robotDirection == -1 ) {
      velocity =  (int8_t) ( -1 * robotSpeed );
      targetPositionRight = CurrentPositionRight - robotSteps;
      targetPositionLeft  =  CurrentPositionLeft - robotSteps;
    }
  }


  // CommandedPositionRight++;
  // CommandedPositionLeft++;

}

void turnRobot (uint8_t robotSpeed, int16_t robotSteps, int8_t robotDirection ) {
  if ( (robotSpeed > 0) &&  (robotDirection != 0) && (robotSteps > 0) ) {

    if ( robotDirection == 1 ) {
      turnt =  robotSpeed;
      targetPositionRight = CurrentPositionRight + robotSteps;
      targetPositionLeft  = CurrentPositionLeft  - robotSteps;
    } else if ( robotDirection == -1 ) {
      turnt =  (int8_t) ( -1 * robotSpeed );
      targetPositionRight = CurrentPositionRight - robotSteps;
      targetPositionLeft  =  CurrentPositionLeft + robotSteps;
    }

  }

}

void goForward (void  ) {

  // digitalWrite (motorRightOne, HIGH);
  //  digitalWrite (motorRightTwo, LOW);

  // digitalWrite (motorLeftOne, HIGH);
  // digitalWrite (motorLeftTwo, LOW);
}

void goBackward (void) {

  // digitalWrite (motorRightOne, LOW);
  //digitalWrite (motorRightTwo, HIGH);

  // digitalWrite (motorLeftOne, LOW);
  // digitalWrite (motorLeftTwo, HIGH);
}



void goSpin (void) {

  // digitalWrite (motorRightOne, HIGH);
  // digitalWrite (motorRightTwo, LOW);

  // digitalWrite (motorLeftOne, LOW);
  // digitalWrite (motorLeftTwo, HIGH);
}


void goStop (void) {

  // digitalWrite (motorRightOne, LOW);
  // digitalWrite (motorRightTwo, LOW);

  // digitalWrite (motorLeftOne, LOW);
  // digitalWrite (motorLeftTwo, LOW);
}
