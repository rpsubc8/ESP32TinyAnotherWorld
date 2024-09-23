#ifndef _TECLADO_PS2KBD_H
 #define _TECLADO_PS2KBD_H

#include "gbConfig.h"
#include "hardware.h"
#include <Arduino.h>


//Def keys BEGIN
#define KEY_F1 0x05
#define KEY_F2 0x06
#define KEY_F3 0x04
#define KEY_F4 0x0C
#define KEY_F5 0x03
#define KEY_F6 0x0B
#define KEY_F7 0x83
#define KEY_F8 0x0A
#define KEY_F9 0x01
#define KEY_F10 0x09
#define KEY_F11 0x78
#define KEY_F12 0x07
#define KEY_ESC 0x76
#define KEY_CURSOR_LEFT 0x6B
#define KEY_CURSOR_DOWN 0x72
#define KEY_CURSOR_RIGHT 0x74
#define KEY_CURSOR_UP 0x75
#define KEY_ALT_GR 0x11
#define KEY_ENTER 0x5A
#define KEY_HOME 0xE06C
#define KEY_END 0xE069
#define KEY_PAGE_UP 0xE07D
#define KEY_PAGE_DOWN 0xE07A
#define KEY_PAUSE 0xE11477E1F014E077
#define KEY_BACKSPACE 0x66
#define KEY_DELETE 0x71

#define PS2_KC_A 0x1C
#define PS2_KC_B 0x32
#define PS2_KC_C 0x21
#define PS2_KC_D 0x23
#define PS2_KC_E 0x24
#define PS2_KC_F 0x2B
#define PS2_KC_G 0x34
#define PS2_KC_H 0x33
#define PS2_KC_I 0x43
#define PS2_KC_J 0x3B
#define PS2_KC_K 0x42
#define PS2_KC_L 0x4B
#define PS2_KC_M 0x3A
#define PS2_KC_N 0x31
#define PS2_KC_O 0x44
#define PS2_KC_P 0x4D
#define PS2_KC_Q 0x15
#define PS2_KC_R 0x2D
#define PS2_KC_S 0x1B
#define PS2_KC_T 0x2C
#define PS2_KC_U 0x3C
#define PS2_KC_V 0x2A
#define PS2_KC_W 0x1D
#define PS2_KC_X 0x22
#define PS2_KC_Y 0x35
#define PS2_KC_Z 0x1A

#define PS2_KC_SPACE 0x29

#define PS2_KC_0 0x45
#define PS2_KC_1 0x16
#define PS2_KC_2 0x1E
#define PS2_KC_3 0x26
#define PS2_KC_4 0x25
#define PS2_KC_5 0x2E
#define PS2_KC_6 0x36
#define PS2_KC_7 0x3D
#define PS2_KC_8 0x3E
#define PS2_KC_9 0x46

#define PS2_KEY_0 0X30
#define PS2_KEY_1 0X31
#define PS2_KEY_2 0X32
#define PS2_KEY_3 0X33
#define PS2_KEY_4 0X34
#define PS2_KEY_5 0X35
#define PS2_KEY_6 0X36
#define PS2_KEY_7 0X37
#define PS2_KEY_8 0X38
#define PS2_KEY_9 0X39

#define PS2_KC_ENTER 0x5A

#define PS2_KC_F1 0x05

#define PS2_KC_SEMI 0x4C
#define PS2_KC_APOS 0x52
#define PS2_KC_COMMA 0x41
#define PS2_KC_EQUAL 0x55
#define PS2_KC_DOT 0x49
#define PS2_KC_TAB 0x0D
#define PS2_KC_CTRL 0X14
#define PS2_KC_SCROLL 0x7E
#define PS2_KC_L_SHIFT 0x12
#define PS2_KC_R_SHIFT 0x59
#define PS2_KEY_KP_DIV 0x2F
//Def keys END



void IRAM_ATTR kb_interruptHandler(void);
void kb_begin(void);


unsigned char checkAndCleanKey(unsigned char scancode);
unsigned char checkKey(unsigned char scancode);


void ClearKeyboard(void);

#endif
