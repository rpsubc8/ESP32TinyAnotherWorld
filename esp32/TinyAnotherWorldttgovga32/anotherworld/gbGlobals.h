#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H

 #include "gbConfig.h"

 //JJ #include <SDL/SDL.h> //SDL1
// #include <stdio.h> 
 
//extern FILE * gb_log;

 extern unsigned char gb_forcePSRAMused;

 extern unsigned short int gb_use_lib_offset_x;
 extern unsigned short int gb_use_lib_offset_y;
 extern unsigned short int gb_use_lib_video;
 extern unsigned short int gb_use_lib_width;
 extern unsigned short int gb_use_lib_height;

 extern volatile unsigned char gb_snd_shift;

 extern unsigned int gb_ramfree_ini;
 extern unsigned int gb_ramfree_setupEnd;

 extern unsigned char gb_show_osd_main_menu;
 extern unsigned char gb_language_en;
 extern unsigned char gb_id_menu_cur;

 extern short int gb_osd_ventana_ini;
 extern short int gb_osd_ventana_fin;

 extern unsigned char gb_use_keyb_poll_ms;
 extern unsigned int gb_keyb_poll_time_cur;
 extern unsigned int gb_keyb_poll_time_prev;

 extern unsigned char gb_use_vga_poll_ms;
 extern unsigned int gb_vga_poll_time_cur;
 extern unsigned int gb_vga_poll_time_prev; 


 #ifdef use_lib_esp32sound
  //extern volatile unsigned char gb_snd_cont; //contador canales
  extern volatile unsigned char gb_snd_play[4];
  extern volatile unsigned char * gb_snd_data[4];
  extern volatile unsigned short int gb_snd_len[4];
  extern volatile unsigned short int gb_snd_pos_loop[4];
  extern volatile unsigned short int gb_snd_pos_cur[4];
 #endif

 extern volatile unsigned char gb_keymap[32];
 extern unsigned char gb_key_video[9]; //Teclado modos video provisional

 extern unsigned char * gb_vram[4]; //4 buffer video en SRAM
 extern unsigned char * gb_memory; //614400 bytes global memory engine
 extern unsigned char * _pages[4];
 extern unsigned char *_curPagePtr1, *_curPagePtr2, *_curPagePtr3;

 extern unsigned char gb_paleta16[16];
 extern unsigned char gb_paleta16_id[16]; //Identificador sin BBGGRR
 
 extern unsigned char gb_vga_8colors; //1 8 colores, 0 64 colores

 extern unsigned char gb_setup_end;
 
 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 extern unsigned char gb_const_colorNormal[64]; //8 y 64 colores
 extern unsigned char gb_sync_bits;
 extern const unsigned char pin_config8[];
 extern const unsigned char pin_config64[];
 extern const unsigned int *gb_ptrVideo_cur; 
 extern unsigned char gb_vga_videomode_cur;
 extern unsigned char gb_vga_videomode_cur_menu; //El seleccionado en el OSD
 extern unsigned short int gb_vga_ancho;
 extern unsigned short int gb_vga_alto; 
 //JJ extern SDL_Surface * gb_screen;
 extern unsigned int gb_fps;
 extern unsigned int gb_fps_time_ini;
 
 extern unsigned char gb_use_speed_fast;
 extern unsigned char gb_use_keyb_left;
 extern unsigned char gb_use_keyb_right;
 extern unsigned char gb_use_keyb_up;
 extern unsigned char gb_use_keyb_down;
 extern unsigned char gb_use_keyb_space;
 extern unsigned char gb_use_keyb_return;
 extern unsigned int gb_real_vga_time_ini;
 
 extern unsigned char gb_use_game_part;
 extern unsigned char gb_use_send_game_part;

 extern unsigned char gb_use_debug;
 

 extern unsigned char gb_do_action_key_f1;
 extern unsigned char gb_do_action_key_reset;
 extern unsigned char gb_do_action_key_show_osd;
 
 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
 #endif

#endif

 
