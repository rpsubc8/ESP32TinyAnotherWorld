#ifndef _MAIN_H
 #define _MAIN_H

 #include "gbConfig.h"
 #include <stdio.h> //para NULL

 //#define uint16_t unsigned short int
 //#define AUTO_DETECT_PLATFORM
 #define SYS_LITTLE_ENDIAN
// #define _WIN32
 
 #define nullptr NULL
 
 #define BYPASS_PROTECTION
 
 
 
 #ifdef use_lib_keyboard_uart
  void do_keyboard_uart(void);
  void keyboard_uart_poll(void);
 #endif

 void read_keyboard(void);

 //void Setup(void);
 //void PreparaBitluniVGA(void);
 //void LiberaBitluniVGA(void);
 //void bitluni_dump_vga(void);
 
#endif

 
