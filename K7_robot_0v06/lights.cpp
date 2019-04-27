#include  "Arduino.h"
#include  "pins.h"
#include  "pcf8574_esp.h"


extern PCF857x expanderTwo;



void led (uint8_t color, bool state) {

  switch (color) {
    case 1:
      expanderTwo.write(redLed, !state); // red
      break;
    case 2:
      expanderTwo.write(greenLed, !state); // green
      break;
    case 3:
      expanderTwo.write(blueLed, !state); // blue
      break;
    default:
      break;
  }
}


void backLight (bool state) {
  expanderTwo.write(lcdLed, !state); // lcd back
}
