//  KARL 7 ROBOT

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
  display.setRotation(2);
  delay (100);
  display.fillCircle(24, 30, 10, BLACK);
  display.fillCircle(60, 30, 10, BLACK);
  display.display();
  encoderEnable ();
//   setupMotor();
  setupSound ();

  // Setup Ultra sound sensor
  setupUltrasound(trigPin, echoPin);


  /*
    goSpin ();
    delay (5000);
    goStop();
    delay(500);
    goForward();
    delay(1000);
    goStop();

    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(i);
    display.display();

  */

  backLight (true);
  for (int i = 200; i < 1000; i += 100) {
    led (i % 3, true);
//    tone (i, 50);
    led (i % 3, false);
  }
  //. backLight (false);


}

// 1 --- is red
// 2 --- is green
uint8_t ledCounter = 1;

void loop() {

 //handleInt ();
  checkMotorFault ();

  //CommandedPositionRight++;
  //CommandedPositionLeft++;

 
  led (ledCounter, true);
//  tone (ledCounter*500, 50);
  delay (500);
  led (ledCounter, false);
  
  ledCounter ++;
  if (ledCounter > 3) ledCounter = 1;
  
  // loop () of Ultra sound sensor
  loopUltrasound(trigPin, echoPin)
  
  /*
    if (PCFInterruptFlagOne) {

      Serial.println("Got an interrupt: ");
      expanderTwo.write(7, expanderOne.read(3)); // 3 is button right
      expanderTwo.write(6, expanderOne.read(2)); // 2 is button left
      PCFInterruptFlagOne = false;
    }

    if (PCFInterruptFlagTwo) {
      Serial.println("Got an interrupt: ");
      if (expanderTwo.read(3) == HIGH) Serial.println("Pin 3 is HIGH!");
      else Serial.println("Pin 3 is LOW!");
      // DO NOTE: When you write LOW to a pin on a PCF8574 it becomes an OUTPUT.
      // It wouldn't generate an interrupt if you were to connect a button to it that pulls it HIGH when you press the button.
      // Any pin you wish to use as input must be written HIGH and be pulled LOW to generate an interrupt.
      //  pcf8574.write(7, pcf8574.read(3));
      PCFInterruptFlagTwo = false;
    }
    Serial.println("Blink.");
    // if(digitalRead(2)==HIGH) digitalWrite(2, LOW);
    // else digitalWrite(2, HIGH);

    //expanderTwo.write(7, HIGH); // red
    //expanderTwo.write(6, HIGH); // green
    // expanderTwo.write(5, HIGH); // blue
    expanderTwo.write(4, HIGH); // lcd back
    delay(1000);
    //expanderTwo.write(7, LOW); // red
    // expanderTwo.write(6, LOW); // green
    // expanderTwo.write(5, LOW); // blue
    expanderTwo.write(4, LOW); // lcd back
    delay(1000);
  */
}
