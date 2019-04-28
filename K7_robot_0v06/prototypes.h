void handleInt (void);
void encoderEnable (void);


void setupMotor (void);
void goForward (void) ;
void goSpin(void);
void goStop (void);
void goBackward (void);


void setupSound (void);
void tone( uint16_t , uint16_t );


void backLight (bool);
void led (uint8_t, bool);

void motionHandler (void); 
void moveRobot (uint8_t , uint32_t ,  int8_t );
void turnRobot (uint8_t , int16_t , int8_t  );
