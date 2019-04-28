#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"
#include "display.h"

extern Adafruit_PCD8544 display;

// Globals for display management.
uint32_t previous_blink_ms = 0;
uint32_t blink_again = 1500; // Time between blinks.
uint32_t blink_length = 300; // Time for blink.
face_t face_state = SMILE;

// Change/update the face.
void display_update_face(face_t new_face_state, bool force) {
  if (!force && face_state == new_face_state) {
    return;
  }

  face_state = new_face_state;

  switch(face_state) {
    case SMILE               : display_smiling_face(); break;
    case BIG_SMILE           : display_bigger_smile(); break;
    case HEART_EYES          : display_heart_eyes(); break;
    case THE_STARE           : display_the_stare(); break;
    case MEH                 : display_meh(); break;
    case SURPRISED           : display_surprised(); break;
    case SURPRISED_EYES_OPEN : display_surprised_eyes_open(); break;
    case ANGRY               : display_angry(); break;
    case SAD                 : display_sad(); break;
    case BLINK               : display_eyeblink(); break;
  }
}

// Get the current face.
face_t display_get_face(void) {
    return face_state;
}

// Manage the face and eyes blinking. Needs to be called from loop().
void display_manage_face(void) {
  // Manage the time of the blink.
  if (face_state == SMILE) {
    if (millis() - previous_blink_ms >= blink_again) {
      // Time to blink again!
      display_update_face(BLINK);
      previous_blink_ms = millis();
    }
  }
  else if (face_state == BLINK) {
    if (millis() - previous_blink_ms >= blink_length) {
      // Time to smile.
      display_update_face(SMILE);
      previous_blink_ms = millis();
    }
  }
}

/* Initialize the display. */
void display_init(uint8_t contrast) {
  display.begin(contrast);
  display.setRotation(2);  // Rotate the screen so text is upright.
  display.clearDisplayRAM();
  display.clearDisplay();
}

/* Quick way to print a line of text. */
void display_text(const char *text, uint8_t x, uint8_t y, uint8_t size) {
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

/* Quick way to print an int. */
void display_int(int integer, uint8_t x, uint8_t y, uint8_t size) {
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(integer);
  display.display();
}

/* Default expression: two filled circle eyes and a smiling mouth. */
void display_smiling_face(void) {
  display.clearDisplay();
  display.setRotation(0);

  display.fillCircle(24, 30, 8, BLACK);
  display.fillCircle(60, 30, 8, BLACK);

  display.setCursor(30,32);
  display.setTextSize(2);
  display.setRotation(1);
  display.print(")");
  display.display();
}

void display_heart_eyes(void) {
  display.clearDisplay();
  display.setRotation(2);

//left eye
  display.fillCircle(21, 10, 5, BLACK);
  display.fillCircle(29, 10, 5, BLACK);
  display.fillTriangle(15, 10, 35, 10, 25, 19, BLACK);

//right eye
  display.fillCircle(51, 10, 5, BLACK);
  display.fillCircle(59, 10, 5, BLACK);
  display.fillTriangle(45, 10, 65, 10, 55, 19, BLACK);

//mouth
  display.fillCircle(40, 30, 5, BLACK);
  display.fillCircle(40, 28, 5, WHITE);
  display.display();
}

void display_the_stare(void) {
  display.clearDisplay();
  display.setRotation(2);


 //left eye
   display.fillRect(15, 7, 20, 2, BLACK);
   display.fillCircle(21, 10, 2, BLACK);
   display.fillRect(15, 12, 20, 2, BLACK);

//right eye
   display.fillRect(45, 7, 20, 2, BLACK);
   display.fillCircle(51, 10, 2, BLACK);
   display.fillRect(45, 12, 20, 2, BLACK);

//mouth
  display.fillCircle(40, 33, 10, BLACK);
  display.fillCircle(40, 35, 10, WHITE);

  display.display();
}

void display_bigger_smile(void) {
  display.clearDisplay();
  display.setRotation(0);

//left eye
  display.fillCircle(60, 30, 12, BLACK);

//right eye
  display.fillCircle(24, 30, 12, BLACK);

//mouth
  display.fillCircle(40, 10, 5, BLACK);
  display.fillCircle(40, 15, 5, WHITE);
  display.display();
}

void display_meh(void) {
  display.clearDisplay();
  display.setRotation(0);


//left eye
  display.fillCircle(60, 30, 12, BLACK);

//right eye
  display.fillCircle(24, 30, 12, BLACK);

//mouth
  display.fillRect(20, 5, 35, 5, BLACK);

  display.display();
}

void display_surprised(void){
  display.clearDisplay();
  display.setRotation(0);

//left eye
  display.fillCircle(60, 30, 12, BLACK);

//right eye
  display.fillCircle(24, 30, 12, BLACK);

//mouth
  display.fillCircle(40, 10, 5, BLACK);


  display.display();
}

void display_surprised_eyes_open(void){
  display.clearDisplay();
  display.setRotation(0);

//left eye
  display.fillCircle(60, 30, 12, BLACK);
  display.fillCircle(60, 30, 8, WHITE);

//right eye
  display.fillCircle(24, 30, 12, BLACK);
  display.fillCircle(24, 30, 8, WHITE);

//mouth
  display.fillCircle(40, 10, 5, BLACK);

  display.display();
}

void display_angry(void) {
  display.clearDisplay();
  display.setRotation(2);

 //left eyebrow
   display.drawLine(20, 2, 30, 7, BLACK);
   display.drawLine(21, 2, 31, 7, BLACK);
   display.drawLine(22, 2, 32, 7, BLACK);

 //left eye
   display.fillCircle(21, 10, 5, BLACK);


//right eyebrow
   display.drawLine(52, 2, 37, 8, BLACK);
   display.drawLine(53, 2, 38, 8, BLACK);
   display.drawLine(54, 2, 39, 8, BLACK);

//right eye
   display.fillCircle(51, 10, 5, BLACK);

//mouth
  display.fillCircle(40, 33, 10, BLACK);
  display.fillCircle(40, 35, 10, WHITE);

  display.display();
}

void display_sad(void) {
  display.clearDisplay();
  display.setRotation(2);

 //left eyebrow
   display.drawLine(18, 2, 8, 10, BLACK);
   display.drawLine(19, 2, 9, 10, BLACK);
   display.drawLine(20, 2, 10, 10, BLACK);

 //left eye
   display.fillCircle(21, 10, 5, BLACK);

//right eyebrow
   display.drawLine(46, 1, 60, 6, BLACK);
   display.drawLine(47, 1, 61, 6, BLACK);
   display.drawLine(48, 1, 62, 6, BLACK);

//right eye
   display.fillCircle(51, 10, 5, BLACK);

//tear
   display.drawLine(52, 22, 54, 18, BLACK);
   display.drawLine(56, 22, 54, 17, BLACK);
   display.fillCircle(54, 22, 2, BLACK);

//mouth
  display.fillCircle(40, 33, 10, BLACK);
  display.fillCircle(40, 35, 10, WHITE);

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
}

