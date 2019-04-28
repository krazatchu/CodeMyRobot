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
void moveRobot (uint8_t , uint32_t ,  int8_t );
void turnRobot (uint8_t , int16_t , int8_t  );

//IR Receiver
void enableIR();
void controlIRReceive();

void playTetris(void);
#endif
