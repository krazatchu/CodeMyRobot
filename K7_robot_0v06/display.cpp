#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"
#include "display.h"

extern Adafruit_PCD8544 display;

void display_init(uint8_t contrast) {
  display.begin(contrast);
  display.setRotation(2);  // Rotate the screen so text is upright.
  display.clearDisplayRAM();
  display.clearDisplay();
}

void display_text(const char *text, uint8_t x, uint8_t y, uint8_t size) {
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}
