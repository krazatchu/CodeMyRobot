#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"
#include "display.h"

extern Adafruit_PCD8544 display;

/* Initialize the display. */
void display_init(uint8_t contrast) {
  display.begin(contrast);
  display.setRotation(2);  // Rotate the screen so text is upright.
  display.clearDisplayRAM();
  display.clearDisplay();
}

/* Quick way to print a line of text. */
void display_print(const char *text, uint8_t x, uint8_t y, uint8_t size) {
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

/* Default expression: two filled circle eyes and a smiling mouth. */
void display_smiling_face(void) {
  display.clearDisplay();
  display.setRotation(0);
  delay(100);
  display.fillCircle(24, 30, 8, BLACK);
  display.fillCircle(60, 30, 8, BLACK);

  display.setCursor(30,32);
  display.setTextSize(2);
  display.setRotation(1);
  display.print(")");
  display.display();
}

/* Eye blinks for 500 ms and then back to default expression. */
void display_eyeblink(void) {
  display.setRotation(0);
  display.clearDisplay();

  display.drawLine(18, 31, 30, 30, BLACK);
  display.drawLine(54, 30, 64, 31, BLACK);

  display.setCursor(30,32);
  display.setTextSize(2);
  display.setRotation(1);
  display.print(")");
  display.display();

  delay(500);

  display_smiling_face();
}
