#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_

void handleInt(void);
void encoderEnable(void);

void setupMotor(void);
void setupSound(void);
void setupWifi(void);

void tone(float, float);

void backLight(bool);
void led(uint8_t, bool);

// Ultrasound sensor setup()
void setupUltrasound(uint8_t, uint8_t);
unsigned char loopUltrasound(uint8_t, uint8_t);
void motionHandler (void); 

// Moves robot forward or backwards
// \param robotSpeed integer range [1-25]
// \param robotSteps 80 steps is one wheel revolution, number of steps
// \param robotDirection 1 for forward, -1 for backwards
void moveRobot (uint8_t robotSpeed, uint32_t robotSteps,  int8_t robotDirection);

// Turns robot left or right
// \param robotSpeed integer range [1-25]
// \param robotSteps 80 steps is one wheel revolution, wheels move opposite direction
// \param robotDirection 1 for right, -1 for left
void turnRobot (uint8_t robotSpeed, int16_t robotSteps, int8_t robotDirection );

//IR Receiver
void enableIR();
void controlIRReceive();

void playTetris(void);
#endif
