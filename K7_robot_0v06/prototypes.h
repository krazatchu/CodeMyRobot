#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_

void handleInt(void);
void encoderEnable(void);


void setupMotor(void);
void goForward(void) ;
void goSpin(void);
void goStop(void);
void goBackward(void);


void setupSound(void);
void setupWifi(void);
void tone(float, float);


void backLight(bool);
void led(uint8_t, bool);

// Ultrasound sensor setup()
void setupUltrasound(uint8_t, uint8_t);
unsigned char loopUltrasound(uint8_t, uint8_t);

void playTetris(void);
#endif