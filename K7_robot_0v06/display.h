#ifndef DISPLAY_H_
#define DISPLAY_H_

void display_init(uint8_t contrast=60);
void display_print(const char *text, uint8_t x=0, uint8_t y=0, uint8_t size=1);

void display_smiling_face(void);
void display_eyeblink(void);

#endif
