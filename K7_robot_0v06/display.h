#ifndef DISPLAY_H_
#define DISPLAY_H_

void display_init(uint8_t contrast=60);
void display_text(const char *text, uint8_t x=0, uint8_t y=0, uint8_t size=1);
void display_int(int integer, uint8_t x=0, uint8_t y=0, uint8_t size=1);

void display_smiling_face(void);
void display_eyeblink(void);
void display_heart_eyes(void);
void display_the_stare(void);
void display_bigger_smile(void);
void display_meh(void);
void display_surprised(void);
void display_surprised_eyes_open(void);


#endif
