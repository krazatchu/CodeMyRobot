//  KARL 7 ROBOT

#include "pins.h"
#include "prototypes.h"
#include "pcf8574_esp.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"
#include "display.h"

// Replace with your network credentials
bool wifiEnabled = true;
bool apMode = true;
char* ssid     = "ESP32-Max-2";
char* password = "123456789";

PCF857x expanderTwo(0x20, &Wire);
PCF857x expanderOne(0x21, &Wire);
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 12, -1); // (SCLK) // (DIN) // (D/C) // (RST) on IO EXPANDER

extern volatile long CommandedPositionLeft;
extern volatile long CommandedPositionRight;

volatile bool startupSound = true;
volatile bool loopSound = false;
volatile bool tetrisMusic = false;

volatile bool PCFInterruptFlagTwo = false;
void ICACHE_RAM_ATTR PCFInterruptTwo() {
  PCFInterruptFlagTwo = true;
}
volatile bool PCFInterruptFlagOne = false;
void ICACHE_RAM_ATTR PCFInterruptOne() {
  PCFInterruptFlagOne = true;
}



void setup() {
  Serial.begin(115200);
  delay (1000);
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

  display_init(60);
  display_update_face(SMILE, true);

  encoderEnable();
  setupMotor();
  setupSound();
  setupWifi();

  // Setup Ultra sound sensor
  //setupUltrasound(trigPin, echoPin);


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

    if (startupSound == true)
      tone(i, 50);

    led (i % 3, false);
  }
  if (tetrisMusic == true)
    playTetris();

  //. backLight (false);

}

// 1 --- is red
// 2 --- is green
uint8_t ledCounter = 1;

void buttonIRQhandler (void) {

  if ( PCFInterruptFlagOne == true) {
    PCFInterruptFlagOne = false;

    uint8_t readAll = expanderOne.read8();
    bool buttonOneFlag  = ~ readAll & (1 << 2);
    bool buttonTwoFlag  = ~ readAll & (1 << 3);
    bool tofIrqFlag     = ~ readAll & (1 << 5);
    bool motorFaultFlag = ~ readAll & (1 << 7);

    // customize the code following here:
    if (buttonOneFlag) {
      buttonOneFlag = false;
      Serial.println("B1 Left");
    }
    if (buttonTwoFlag) {
      buttonTwoFlag = false;
      Serial.println("B2 Right");
    }

    if (tofIrqFlag) {
      tofIrqFlag = false;
      Serial.println("TOF irq");
    }

    if (motorFaultFlag) {
      motorFaultFlag = false;
      Serial.println("Motor Fault");
    }
  }
}

void loop() {
  // Display management.
  // Use display_update_face() to set a new face.
  display_manage_face();

  buttonIRQhandler ();

  motionHandler ();

  //loopUltrasound(trigPin, echoPin);
  // loop () of Ultra sound sensor
  //  ledCounter ++;
  //  if (ledCounter > 3) ledCounter = 1;

  if (PCFInterruptFlagOne) {

    Serial.println("Got a button!");
    if (  expanderOne.read(3) )  { // 3 is button right
      moveRobot (5, 40, 1);
      //turnRobot (3, 40, 1);
    }

    // Left button.
    if (expanderOne.read(2)) {
      // Cycle through the faces.
      display_update_face((face_t)((display_get_face() + 1) % MAX_FACE));
    }

    PCFInterruptFlagOne = false;
  }
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
