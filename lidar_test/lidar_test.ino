//  CARL 7 ROBOT

#include  "pins.h"
#include  "prototypes.h"
#include "pcf8574_esp.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"

PCF857x expanderTwo(0x20, &Wire);
PCF857x expanderOne(0x21, &Wire);
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 12, -1); // (SCLK) // (DIN) // (D/C) // (RST) on IO EXPANDER


extern volatile long CommandedPositionLeft;
extern volatile long CommandedPositionRight;


volatile bool PCFInterruptFlagTwo = false;
void ICACHE_RAM_ATTR PCFInterruptTwo() {
  PCFInterruptFlagTwo = true;
}
volatile bool PCFInterruptFlagOne = false;
void ICACHE_RAM_ATTR PCFInterruptOne() {
  PCFInterruptFlagOne = true;
}

void checkMotorFault (void ) {

  if (PCFInterruptFlagOne == true) {
    PCFInterruptFlagOne = false;
    bool fault = expanderOne.read(7);
    if (!fault)  Serial.println("mot fault");
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println("K7 ROBOT STARTING");

  Wire.begin(21, 22);
  Wire.setClock(100000L);

  expanderTwo.begin();
  pinMode(32, INPUT_PULLUP);
  expanderTwo.resetInterruptPin();
  attachInterrupt(digitalPinToInterrupt(32), PCFInterruptTwo, FALLING);

  expanderOne.begin();
  pinMode(35, INPUT_PULLUP);
  expanderOne.resetInterruptPin();
  attachInterrupt(digitalPinToInterrupt(35), PCFInterruptOne, FALLING);

  display.begin();
  display.setContrast(60);
  display.clearDisplayRAM();
  display.clearDisplay();
  delay (100);
  display.fillCircle(24, 30, 10, BLACK);
  display.fillCircle(60, 30, 10, BLACK);
  display.fillRect(26, 10, 20, 5, BLACK);
  display.display();
  display.clearDisplay();
  encoderEnable ();

  // setupMotor();
  setupSound ();
  setupLidar();

  backLight (true);
  //. backLight (false);
}

uint8_t ledCounter = 1;
uint16_t range = 0;

/*
 * Displays the range on the screen. 
 */
void displayRange(uint16_t range){
   display.clearDisplay();
   display.setRotation(2);
   display.setCursor(0, 0);
   display.setTextSize(3);
   display.println(range);
   display.display();
}

/**
 * Displays a string on the screen.
 */
void displayString(String string){
   display.clearDisplay();
   display.setRotation(2);
   display.setCursor(0, 0);
   display.setTextSize(2);
   display.println(string);
   display.display();
}

/**
 * Checks if the range is too close.
 */
boolean tooClose(uint16_t range){
  if(range<50){
    return true;
  }else{
    return false;  
  }
  
}

void loop() {
  Serial.println(expanderTwo.read(2));
  range = getMeasurement();
  Serial.println(range);
  displayRange(range);
  //tone (50, range);
  if(tooClose(range)){
    displayString("Too Close");  
  }
  
}
