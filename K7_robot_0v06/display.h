#ifndef DISPLAY_H_
#define DISPLAY_H_

enum face_t {
  SMILE = 0,
  BIG_SMILE = 1,
  HEART_EYES = 2,
  THE_STARE = 3,
  MEH = 4,
  SURPRISED = 5,
  SURPRISED_EYES_OPEN = 6,
  ANGRY = 7,
  SAD = 8,
  BLINK = 9,

  MAX_FACE
};

// Change/update the face.
void display_update_face(face_t new_face_state, bool force=false);

// Get the current face.
face_t display_get_face(void);

// Manage the face and eyes blinking. Needs to be called from loop().
void display_manage_face(void);

// Initialize the display.
void display_init(uint8_t contrast=60);

// Quick test print.
void display_text(const char *text, uint8_t x=0, uint8_t y=0, uint8_t size=1);

// Quick integer print.
void display_int(int integer, uint8_t x=0, uint8_t y=0, uint8_t size=1);

// Faces.
void display_smiling_face(void);
void display_eyeblink(void);
void display_heart_eyes(void);
void display_the_stare(void);
void display_bigger_smile(void);
void display_meh(void);
void display_surprised(void);
void display_surprised_eyes_open(void);
void display_angry(void);
void display_sad(void);

#endif
