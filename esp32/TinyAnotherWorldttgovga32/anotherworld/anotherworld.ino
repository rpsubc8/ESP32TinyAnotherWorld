//Port mod ESP32 by ackerman
/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

//-lmingw32 -lSDLmain -lSDL -lzlib
//quitar zlib  

#include "gbConfig.h"
#include "hardware.h"
#include "gbGlobals.h"
#include "osd.h"
#include "PS2Kbd.h"
#include "vga6bit.h"
#include "main.h"

#include "engine.h"
#include "sys.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//JJ #include <SDL/SDL.h>
//JJ #include <windows.h>
//JJ #include <iostream> //No lo necesito
#include "video.h"
#include <Arduino.h>

#ifdef use_lib_esp32sound
 #include <driver/dac.h>

 //No necesita rtc_io.reg ni soc.h, compila PLATFORMIO,Arduino IDE y ArduinoDROID
 //DAC 1 GPIO25
 #define DR_REG_RTCIO_BASE 0x3ff48400
 #define RTC_IO_PAD_DAC1_REG (DR_REG_RTCIO_BASE + 0x84)
 #define RTC_IO_PDAC1_DAC 0x000000FF
 #define RTC_IO_PDAC1_DAC_S 19

 //DAC 2 GPIO26
 #define RTC_IO_PAD_DAC2_REG (DR_REG_RTCIO_BASE + 0x88)
 #define RTC_IO_PDAC2_DAC 0x000000FF
 #define RTC_IO_PDAC2_DAC_S 19


 #define SENS_SAR_DAC_CTRL2_REG (DR_REG_SENS_BASE + 0x009c)
 #define SENS_DAC_CW_EN1_M  (BIT(24))
 #define SENS_DAC_CW_EN2_M  (BIT(25))

 //2048 8 Khz
 #define max_dac_buffer 2048
 static DRAM_ATTR unsigned char gb_dac_buf_r[max_dac_buffer]; 
 unsigned int *gb_dac_buf32_r= (unsigned int *)gb_dac_buf_r;
 //unsigned int *gb_wav_raw32_r= (unsigned int *)gb_wav_raw_r; 

 volatile unsigned int gb_dac_read=0;
 volatile unsigned int gb_dac_write=max_dac_buffer>>1; //max_dac_buffer/2;
 volatile unsigned int gb_dac_write32=max_dac_buffer>>3; //(max_dac_buffer/2)/4; 

 volatile unsigned char gb_spk_data_r= 0; //canal derecho
 volatile unsigned char gb_spk_data_r_before= 0;
 volatile unsigned char gb_spk_data_l= 0; //canal izquierdo
 volatile unsigned char gb_spk_data_l_before= 0;
 volatile unsigned int gb_contador_muestra=0;
 volatile unsigned int gb_contador_muestra32=0;

 hw_timer_t *gb_timerSound = NULL;
 hw_timer_t *gb_timerRellenoSonido = NULL;

 //volatile unsigned char gb_snd_cont=0;
 volatile unsigned char gb_snd_play[4];
 volatile unsigned char * gb_snd_data[4];
 volatile unsigned short int gb_snd_len[4];
 volatile unsigned short int gb_snd_pos_loop[4];
 volatile unsigned short int gb_snd_pos_cur[4];

 void IRAM_ATTR onTimerSoundDAC(void);
 void IRAM_ATTR GeneraRAW(void); 
#endif

unsigned char gb_forcePSRAMused=0;
unsigned char gb_key_video[9]; //Modos de video inicio

volatile unsigned char gb_snd_shift=0;
#ifdef use_lib_esp32sound_mono_channel
 volatile unsigned char gb_sound_mono_channel=1;
#else
 volatile unsigned char gb_sound_mono_channel=0;
#endif 


unsigned short int gb_use_lib_offset_x= use_lib_offset_x;
unsigned short int gb_use_lib_offset_y= use_lib_offset_y;
unsigned short int gb_use_lib_video= use_lib_video;
unsigned short int gb_use_lib_width= use_lib_width;
unsigned short int gb_use_lib_height= use_lib_height;

unsigned char gb_use_keyb_left=0;
unsigned char gb_use_keyb_right=0;
unsigned char gb_use_keyb_up=0;
unsigned char gb_use_keyb_down=0;
unsigned char gb_use_keyb_space=0;
unsigned char gb_use_keyb_return=0;

#ifdef use_lib_keyboard_uart
 unsigned int gb_curTime_keyboard_before_uart;
 unsigned int gb_curTime_keyboard_uart;
 //void do_keyboard_uart(void);
#endif 

unsigned char gb_do_action_key_f1=0;
unsigned char gb_do_action_key_reset=0;
unsigned char gb_do_action_key_show_osd=0; //Muestra fps

#ifdef use_lib_log_serial
 unsigned char gb_use_debug=1;
#else 
 unsigned char gb_use_debug=0;
#endif 

unsigned int gb_ramfree_ini;
unsigned int gb_ramfree_setupEnd;

unsigned char gb_use_keyb_poll_ms= 10; //10 ms poll keyboard
unsigned int gb_keyb_poll_time_cur=0;
unsigned int gb_keyb_poll_time_prev=0;

unsigned char gb_use_vga_poll_ms= 20; //20 ms poll vga
unsigned int gb_vga_poll_time_cur= 0;
unsigned int gb_vga_poll_time_prev= 0;

unsigned char gb_show_osd_main_menu=0;
unsigned char gb_language_en=1;
unsigned char gb_id_menu_cur=0;

short int gb_osd_ventana_ini=0;
short int gb_osd_ventana_fin=15;

unsigned char gb_vga_videomode_cur_menu= use_lib_video; //El que seleccionas en menu OSD


 const unsigned char pin_config8[] = {  
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC
 };  
 const unsigned char pin_config64[] = {  
  PIN_RED_LOW,
  PIN_RED_HIGH,
  PIN_GREEN_LOW,
  PIN_GREEN_HIGH,
  PIN_BLUE_LOW,
  PIN_BLUE_HIGH,
  PIN_HSYNC,
  PIN_VSYNC
 };

unsigned char * gb_vram[4]; //4 buffer video en SRAM
unsigned char * gb_memory; //614400 bytes global memory engine
unsigned char * _pages[4];
unsigned char *_curPagePtr1, *_curPagePtr2, *_curPagePtr3;

unsigned char gb_paleta16[16];
unsigned char gb_paleta16_id[16]; //Identificador sin BBGGRR

const unsigned int *gb_ptrVideo_cur= VgaMode_vga_mode_360x200; //VgaMode_vga_mode_320x200;
unsigned char gb_vga_videomode_cur= video_mode_360x200x70hz_bitluni; //0 //video_mode_vga320x200x70hz_bitluni; //2
unsigned short int gb_vga_ancho= 360; //320;
unsigned short int gb_vga_alto= 200;

unsigned char gb_vga_8colors=0; //1 8 colores, 0 64 colores

unsigned char gb_setup_end=0;

unsigned char gb_use_send_game_part=0;
#ifdef use_lib_ultra_mini_test
 unsigned char gb_use_game_part= 1; //proteccion
#else
 #ifdef use_lib_mini_test
  //unsigned char gb_use_game_part= 1; //proteccion
  unsigned char gb_use_game_part= 2;
 #else
  unsigned char gb_use_game_part= 2; //2 bypass protection
 #endif
#endif
unsigned char gb_use_speed_fast=0;
unsigned int gb_real_vga_time_ini=0;

//JJ SDL_Surface * gb_screen;
unsigned char gb_const_colorNormal[64]; //8 y 64 colores
unsigned char ** gb_buffer_vga;
unsigned int **gb_buffer_vga32;
unsigned char gb_sync_bits;
unsigned int gb_fps=0;
unsigned int gb_fps_time_ini=0;


static const char *USAGE = 
	"Raw - Another World Interpreter\n"
	"Usage: raw [OPTIONS]...\n"
	"  --datapath=PATH   Path to where the game is installed (default '.')\n"
	"  --savepath=PATH   Path to where the save files are stored (default '.')\n";

static bool parseOption(const char *arg, const char *longCmd, const char **opt) {
	bool ret = false;
	if (arg[0] == '-' && arg[1] == '-') {
		if (strncmp(arg + 2, longCmd, strlen(longCmd)) == 0) {
			*opt = arg + 2 + strlen(longCmd);
			ret = true;
		}
	}
	return ret;
}


//***************************************
//void PreparaBitluniVGA()
//{
/*JJ  
 gb_buffer_vga = (unsigned char **)malloc(480*sizeof(unsigned char **)*640);
 gb_buffer_vga32= (unsigned int **)gb_buffer_vga;
 for (int y=0;y<480;y++)
 {      
  gb_buffer_vga[y]= (unsigned char *)malloc(640);
  gb_buffer_vga32[y]= (unsigned int *)gb_buffer_vga32[y];
  memset(gb_buffer_vga[y],0,320);
 }
 */
//}

//***************************************
//void LiberaBitluniVGA()
//{
/*JJ    
 for (int y=0;y<480;y++)
 {      
  free(gb_buffer_vga[y]);
 }
 free(gb_buffer_vga);
 */
//}

//***************************************
//void bitluni_dump_vga()
//{
 /*JJ
 Uint8* p = (Uint8*)gb_screen->pixels + gb_screen->pitch;
 for (int y=0;y<200;y++)
 {
  p = (Uint8*)gb_screen->pixels + (y*gb_screen->pitch);  
  //for (int x=0;x<256;x++)
  for (int x=0;x<320;x++)
  {
   *p++= gb_buffer_vga[y][x];
  }
 }   
 */
//}


/*
	We use here a design pattern found in Doom3:
	An Abstract Class pointer pointing to the implementation on the Heap.
*/
//extern System *System_SDL_create();
extern System *stub ;//= System_SDL_create();

//FILE * gb_log;

//JJ #undef main
//int main(int argc, char *argv[]) 

//JJ Engine* e;
Engine* e = new Engine(stub, ".", ".");

int main(int argc, char **argv)
{
 Serial.printf("Init\n");
 Serial.printf("Sizeof Point:%d Polygon:%d\n",sizeof(struct Point),sizeof(struct Polygon)); //Point:4 Polygon:206 

 return 0;
}


//*******************************
void PrepareColorsBitluniVGA()
{  
 //(color & RGBAXMask) | SBits;
 if (gb_vga_8colors == 1)
 {//8 colores
  if (gb_use_debug==1){ Serial.printf("PrepareColorsBitluniVGA 8 colores\r\n"); }
  for (unsigned char i=0;i<8;i++)
  {
   gb_const_colorNormal[i]= (i & 0x07) | gb_sync_bits;
  }
 }
 else
 {//64 colores
  if (gb_use_debug==1){ Serial.printf("PrepareColorsBitluniVGA 64 colores\r\n"); }
  for (unsigned char i=0;i<64;i++)
  {
   gb_const_colorNormal[i]= (i & 0x3F) | gb_sync_bits;
  }
 }

 //memset(gb_paleta16,0,16); //incide color
 //memset(gb_paleta16,gb_const_colorNormal[0],16); //color directo
}

void ResetPaleta16()
{
 memset(gb_paleta16,gb_const_colorNormal[0],16); //color directo
 memset(gb_paleta16_id,0,16);
}


//**************************************************
void ActivarVideoPorTeclas()
{
 unsigned char is8colors=0;
 //unsigned char is8colors= gb_vga_8colors;
 unsigned char usepllcteforce=0;
 unsigned char usecustompll=0;
 unsigned int p0=0;
 unsigned int p1=0;
 unsigned int p2=0;
 unsigned int p3=0; 

 //unsigned char video=10; //320x200 use_lib_bitlunivga 6bpp
 //unsigned char video=13; //320x240 use_lib_bitlunivga 6bpp
 //unsigned char video=69;  //512x384 use_lib_bitlunivga 6bpp

 unsigned char video= use_lib_video;
 gb_vga_ancho= use_lib_width;
 gb_vga_alto= use_lib_height;
 //unsigned char video= 8;  //360x200x70hz_bitluni_6bpp
 //unsigned char video=9;  //360x200x70hz_bitluni_6bpp
 //unsigned char video=16;  //512x384 use_lib_bitlunivga 6bpp

 unsigned int tiempo_ahora;
 unsigned int tiempo_antes;
 //unsigned char video=255;
  
 tiempo_antes= tiempo_ahora= millis();
 
 while ((tiempo_ahora-tiempo_antes)<use_lib_boot_time_select_vga)
 {
  tiempo_ahora= millis();

  memset(gb_key_video,0xFF,sizeof(gb_key_video));
  
  read_keyboard();
  #ifdef use_lib_keyboard_uart
   keyboard_uart_poll();
   ///read_keyboard_uart();
   do_keyboard_uart();
  #endif

  if (gb_key_video[0]== 0) {video=0;} //0  360x200x70hz bitluni(6bpp)
  if (gb_key_video[1]== 0) {video=1;} //1  360x200x70hz bitluni apll(6bpp
  if (gb_key_video[2]== 0) {video=2;} //2  320x200x70hz bitluni(6bpp)
  if (gb_key_video[3]== 0) {video=3;} //3  320x200x70hz fabgl(6bpp)
  if (gb_key_video[4]== 0) {video=4;} //4  320x200x70hz bitluni apll(6bpp)
  if (gb_key_video[5]== 0) {video=5;} //5  320x240x60hz bitluni(6bpp)           
  if (gb_key_video[6]== 0) {video=6;} //6  320x240x60hz fabgl(6bpp)
  if (gb_key_video[7]== 0) {video=7;} //7  320x240x60hz bitluni apll(6bpp)
  if (gb_key_video[8]== 0) {video=8;} //8  512x384x60hz fabgl(6bpp)
 }
 

 if (video!=255)
 {//Cambio modo video
  if (gb_use_debug==1){ Serial.printf("Video mode change\r\n"); }
  switch (video)
  {
   //case 0: gb_vga_videomode_cur= 0; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; break; //360x200x70hz bitluni(3bpp)
   //case 1: gb_vga_videomode_cur= 1; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; break; //360x200x70hz bitluni apll(3bpp)
   //case 2: gb_vga_videomode_cur= 2; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; break; //320x200x70hz bitluni(3bpp)
   //case 3: gb_vga_videomode_cur= 3; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; break; //320x200x70hz fabgl(3bpp)
   //case 4: gb_vga_videomode_cur= 4; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; break; //320x200x70hz bitluni apll(3bpp)
   //case 5: gb_vga_videomode_cur= 5; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; break; //320x240x60hz bitluni(3bpp)
   //case 6: gb_vga_videomode_cur= 6; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; break; //320x240x60hz fabgl(3bpp)
   //case 7: gb_vga_videomode_cur= 7; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; break; //320x240x60hz bitluni apll(3bpp)

   case 0: gb_vga_videomode_cur= 0; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; gb_vga_ancho= 360; gb_vga_alto=200; break;
   case 1: gb_vga_videomode_cur= 1; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; gb_vga_ancho= 360; gb_vga_alto=200; break;
   case 2: gb_vga_videomode_cur= 2; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; gb_vga_ancho= 320; gb_vga_alto=200; break;
   case 3: gb_vga_videomode_cur= 3; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; gb_vga_ancho= 320; gb_vga_alto=200; break;
   case 4: gb_vga_videomode_cur= 4; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; gb_vga_ancho= 320; gb_vga_alto=200; break;
   case 5: gb_vga_videomode_cur= 5; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; gb_vga_ancho= 320; gb_vga_alto=240; break;
   case 6: gb_vga_videomode_cur= 6; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; gb_vga_ancho= 320; gb_vga_alto=240; break;
   case 7: gb_vga_videomode_cur= 7; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; gb_vga_ancho= 320; gb_vga_alto=240; break;   


   case 8: gb_vga_videomode_cur= 8; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_512x384fabgl; usepllcteforce=1; p0=0x0000; p1=0x00C0; p2=0x0005; p3=0x0001; usecustompll=0; gb_vga_ancho= 512; gb_vga_alto=384; break; //512x384fabgl
   default: break;
  }

 }
 


 if (gb_use_debug==1){ Serial.printf("Video mode set %d\r\n",video); }

//   OSDUpdateAnchoAltoVGA(gb_vga_videomode_cur,is8colors);//Set is8colors gb_vga_8colors

 OSDUpdateAnchoAltoVGA(video,is8colors);

 #ifdef use_lib_video4buffers_psram
 #else
  if (video==8)
  {//Si elijo modo 512x384 que requiere PSRAM, libero SRAM y fuerzo PSRAM
   ForcePSRAMVideo();
  }
 #endif 

   //vga_init(pin_config,VgaMode_vga_mode_320x240,false,0,0,0,0,0,0);
   vga_init((is8colors==1)?pin_config8:pin_config64,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3,usecustompll,is8colors, gb_vga_videomode_cur);
   gb_sync_bits= vga_get_sync_bits();
   gb_buffer_vga = vga_get_framebuffer();
   gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
   PrepareColorsBitluniVGA(); //Llamar despues de tener gb_sync_bits  
   ResetPaleta16(); //Resetea paleta emulador, todo a negro   
}



#ifdef use_lib_keyboard_uart
 char gb_buf_uart[BUFFER_SIZE_UART];
 unsigned char gb_rlen_uart=0;

  //array de 54 teclas
  //0,
  //SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
  //SDLK_g, SDLK_h, SDLK_i, SDLK_j, SDLK_k, SDLK_l,
  //SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r,
  //SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x,
  //SDLK_y, SDLK_z,
  //SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT,
  //SDLK_SPACE,
  //SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
  //SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
  //SDLK_SEMICOLON, SDLK_QUOTEDBL, SDLK_COMMA, SDLK_EQUALS,
  //SDLK_PERIOD, SDLK_SLASH, SDLK_RETURN, SDLK_TAB, SDLK_LCTRL,
  //SDLK_DELETE, SDLK_SCROLLOCK, SDLK_LSHIFT

 void do_keyboard_uart()
 {
  unsigned int contBuf=0;
  unsigned short int auxAddr;
  
  //unsigned char aux_Kmap[54];
  //memset(aux_Kmap,0xFF,sizeof(aux_Kmap));

  gb_use_keyb_up=0;
  gb_use_keyb_down=0;
  gb_use_keyb_left=0;
  gb_use_keyb_right=0;
  gb_use_keyb_space=0;
  gb_use_keyb_return=0;

  if (gb_rlen_uart>0)
  {
   while (contBuf < gb_rlen_uart)
   {
    switch (gb_buf_uart[contBuf])
    {
     case 0x01: //F2   01 62 4F 51      
      if ((contBuf+3) < gb_rlen_uart)
      {       
       gb_do_action_key_f1= 1;
       contBuf+= 3;
      }
      break;

     case 0x09: //TAB saco menu tambien
      gb_do_action_key_f1= 1;
      break;      

     case 0x08: case 0x7F: //Borrar
      //aux_Kmap[29]= 0;
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER 
      //keyboard[6]&= 0xFE; //ENTER
      //aux_Kmap[48]= 0; //SDLK_RETURN id en kmap 48
      gb_use_keyb_return=1;
      break;

     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: gb_use_keyb_up=1; break; //arriba 1B 5B 41
         case 0x42: gb_use_keyb_down=1; break; //abajo 1B 5B 42
         case 0x44: gb_use_keyb_left=1; break; //izquierda 1B 5B 44
         case 0x43: gb_use_keyb_right=1; break; //derecha 1B 5B 43
         default: break;
        }
       }
      }
      break;      

     //case 0x2D: aux_Kmap[53]= 0; break; //SHIFT LEFT -
     //case 0x2E: aux_Kmap[46]= 0; break; //.


     //La velocidad se hace desde OSD
     //case 0x61: case 0x41: gb_use_speed_fast=1; break; //A equivale F10 Rapido
     //case 0x62: case 0x42: gb_use_speed_fast=0; break; //B equivale F11 Lento

/*
     case 0x63: case 0x43: aux_Kmap[3]= 0; gb_Kmap[3]= 0; break; //C
     case 0x64: case 0x44: aux_Kmap[4]= 0; gb_Kmap[4]= 0; break; //D
     case 0x65: case 0x45: aux_Kmap[5]= 0; gb_Kmap[5]= 0; break; //E
     case 0x66: case 0x46: aux_Kmap[6]= 0; gb_Kmap[6]= 0; break; //F
     case 0x67: case 0x47: aux_Kmap[7]= 0; break; //G
     case 0x68: case 0x48: aux_Kmap[8]= 0; break; //H
     case 0x69: case 0x49: aux_Kmap[9]= 0; break; //I
     case 0x6A: case 0x4A: aux_Kmap[10]= 0; break; //J
     case 0x6B: case 0x4B: aux_Kmap[11]= 0; break; //K
     case 0x6C: case 0x4C: aux_Kmap[12]= 0; break; //L
     case 0x6D: case 0x4D: aux_Kmap[13]= 0; break; //M
     case 0x6E: case 0x4E: aux_Kmap[14]= 0; break; //N
     case 0x6F: case 0x4F: aux_Kmap[15]= 0; break; //O
     case 0x70: case 0x50: aux_Kmap[16]= 0; break; //P
     case 0x71: case 0x51: aux_Kmap[17]= 0; break; //Q
     case 0x72: case 0x52: aux_Kmap[18]= 0; break; //R
     case 0x73: case 0x53: aux_Kmap[19]= 0; break; //S
     case 0x74: case 0x54: aux_Kmap[20]= 0; break; //T
     case 0x75: case 0x55: aux_Kmap[21]= 0; break; //U
     case 0x76: case 0x56: aux_Kmap[22]= 0; break; //V
     case 0x77: case 0x57: aux_Kmap[23]= 0; break; //W
     case 0x78: case 0x58: aux_Kmap[24]= 0; break; //X
     case 0x79: case 0x59: aux_Kmap[25]= 0; break; //Y
     case 0x7A: case 0x5A: aux_Kmap[26]= 0; break; //Z
     */
     
     case 0x20: gb_use_keyb_space= 1; break; //SPACE    
      

     //Modos de video inicio provional
     case 0x30: gb_key_video[0]=0; break; //0
     case 0x31: gb_key_video[1]=0; break; //1
     case 0x32: gb_key_video[2]=0; break; //2
     case 0x33: gb_key_video[3]=0; break; //3
     case 0x34: gb_key_video[4]=0; break; //4
     case 0x35: gb_key_video[5]=0; break; //5               
     case 0x36: gb_key_video[6]=0; break; //6
     case 0x37: gb_key_video[7]=0; break; //7     
     case 0x38: gb_key_video[8]=0; break; //8     


     //La carga de nivel se hace por OSD
     //case 0x30: gb_use_game_part= 10; gb_use_send_game_part= 1; break; //0
     //case 0x31: gb_use_game_part= 1; gb_use_send_game_part= 1; break; //1
     //case 0x32: gb_use_game_part= 2; gb_use_send_game_part= 1; break; //2
     //case 0x33: gb_use_game_part= 3; gb_use_send_game_part= 1; break; //3
     //case 0x34: gb_use_game_part= 4; gb_use_send_game_part= 1; break; //4
     //case 0x35: gb_use_game_part= 5; gb_use_send_game_part= 1; break; //5               
     //case 0x36: gb_use_game_part= 6; gb_use_send_game_part= 1; break; //6
     //case 0x37: gb_use_game_part= 7; gb_use_send_game_part= 1; break; //7     
     //case 0x38: gb_use_game_part= 8; gb_use_send_game_part= 1; break; //8
     //case 0x39: gb_use_game_part= 9; gb_use_send_game_part= 1; break; //9


     default: break;
    }//fin switch gb_buf_uart
    contBuf++;
   }//fin while contBuf   
  }//fin if gb_rlen_uart

 }
#endif


#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    if (gb_use_debug==1){ Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart); }
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     if (gb_use_debug==1){ Serial.printf("%02X ",gb_buf_uart[i]); }
    }
    if (gb_use_debug==1){ Serial.printf("\r\n"); }
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }

 //***************************************************************************
 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44        
         default: break;
        }
       }
      }
      else
      {       
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC              
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      break;

     default: break;
    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
#endif


unsigned int read_keyboard_time_prev=0;
unsigned int read_keyboard_time_cur=0;
void read_keyboard()
{
 //gb_use_keyb_up=0;
 //gb_use_keyb_down=0;
 //gb_use_keyb_left=0;
 //gb_use_keyb_right=0;
 //gb_use_keyb_space=0;
 //gb_use_keyb_return=0;
 //unsigned char pulso;

 //if (checkKey(KEY_CURSOR_UP)==1) { Serial.printf("UP\r\n"); }
 //if (checkKey(KEY_CURSOR_DOWN)==1) {Serial.printf("DOWN\r\n"); }
 //if (checkKey(KEY_CURSOR_LEFT)==1) {Serial.printf("LEFT\r\n"); }
 //if (checkKey(KEY_CURSOR_RIGHT)==1) {Serial.printf("RIGHT\r\n"); }
 //if (checkKey(PS2_KC_SPACE)==1) {Serial.printf("SPACE\r\n"); }
 //if (checkKey(PS2_KC_ENTER)==1) {Serial.printf("ENTER\r\n"); }

 //if (checkKey(PS2_KC_0)==1) {Serial.printf("0\r\n"); }
 //if (checkKey(PS2_KC_1)==1) {Serial.printf("1\r\n"); }

 gb_use_keyb_up= (checkKey(KEY_CURSOR_UP)==1) ? 1:0; //arriba
 gb_use_keyb_down= (checkKey(KEY_CURSOR_DOWN)==1)? 1:0; //abajo
 gb_use_keyb_left= (checkKey(KEY_CURSOR_LEFT)==1)? 1:0; //izquierda
 gb_use_keyb_right= (checkKey(KEY_CURSOR_RIGHT)==1)? 1:0; //derecha 

 gb_use_keyb_space= (checkKey(PS2_KC_SPACE)==1)? 1:0; //SPACE
 gb_use_keyb_return= (checkKey(PS2_KC_ENTER)==1)? 1:0; //RETURN


 if (checkKey(PS2_KC_0)==1){ gb_key_video[0]=0; }  //0
 if (checkKey(PS2_KC_1)==1){ gb_key_video[1]=0; }  //1
 if (checkKey(PS2_KC_2)==1){ gb_key_video[2]=0; }  //2
 if (checkKey(PS2_KC_3)==1){ gb_key_video[3]=0; }  //3
 if (checkKey(PS2_KC_4)==1){ gb_key_video[4]=0; }  //4
 if (checkKey(PS2_KC_5)==1){ gb_key_video[5]=0; }  //5               
 if (checkKey(PS2_KC_6)==1){ gb_key_video[6]=0; }  //6
 if (checkKey(PS2_KC_7)==1){ gb_key_video[7]=0; }  //7     
 if (checkKey(PS2_KC_8)==1){ gb_key_video[8]=0; }  //8
 if (checkKey(PS2_KC_9)==1){ gb_key_video[9]=0; }  //9 

 unsigned char gb_use_keyb_f1= (checkKey(KEY_F1)==1)?1:0;
 if (gb_use_keyb_f1==1)
 {
  gb_do_action_key_f1= 1;
 }

 //unsigned char bitKeys= (gb_use_keyb_f1<<6)|(gb_use_keyb_up<<5)|(gb_use_keyb_down<<4)|(gb_use_keyb_left<<3)|(gb_use_keyb_right<<2)|(gb_use_keyb_space<<1)|gb_use_keyb_return;
 //read_keyboard_time_cur= millis();
 //unsigned int auxTime= read_keyboard_time_cur-read_keyboard_time_prev;  
 //Serial.printf("key:%02X keyb:%d%d%d%d%d%d%d cur:%d dur:%d\r\n",bitKeys,gb_use_keyb_f1,gb_use_keyb_up,gb_use_keyb_down,gb_use_keyb_left,gb_use_keyb_right,gb_use_keyb_space,gb_use_keyb_return, read_keyboard_time_cur,auxTime);
 //read_keyboard_time_prev= read_keyboard_time_cur;

 //pulso= checkKey(PS2_KEY_0);
 //if (pulso==1)
 //{
 // //     gb_use_game_part= 10; gb_use_send_game_part= 1; 
 // Serial.printf("0\r\n");
 //} //0

 //La carga de nivel se hace por OSD
 //if (checkKey(PS2_KC_0)==1){ gb_use_game_part= 10; gb_use_send_game_part= 1;}  //10
 //if (checkKey(PS2_KC_1)==1){ gb_use_game_part= 1; gb_use_send_game_part= 1; }  //1
 //if (checkKey(PS2_KC_2)==1){ gb_use_game_part= 2; gb_use_send_game_part= 1; }  //2
 //if (checkKey(PS2_KC_3)==1){ gb_use_game_part= 3; gb_use_send_game_part= 1; }  //3
 //if (checkKey(PS2_KC_4)==1){ gb_use_game_part= 4; gb_use_send_game_part= 1; }  //4
 //if (checkKey(PS2_KC_5)==1){ gb_use_game_part= 5; gb_use_send_game_part= 1; }  //5               
 //if (checkKey(PS2_KC_6)==1){ gb_use_game_part= 6; gb_use_send_game_part= 1; }  //6
 //if (checkKey(PS2_KC_7)==1){ gb_use_game_part= 7; gb_use_send_game_part= 1; }  //7     
 //if (checkKey(PS2_KC_8)==1){ gb_use_game_part= 8; gb_use_send_game_part= 1; }  //8
 //if (checkKey(PS2_KC_9)==1){ gb_use_game_part= 9; gb_use_send_game_part= 1; }  //9 

 //La velocidad se hace desde OSD
 //if (checkKey(PS2_KC_A)==1){ gb_use_speed_fast=1; } //A equivale F10 Rapido
 //if (checkKey(PS2_KC_B)==1){ gb_use_speed_fast=0; } //B equivale F11 Lento
 
}



#ifdef use_lib_esp32sound
 void IRAM_ATTR onTimerSoundDAC()
 {
  //Para DAC   
  if (gb_spk_data_r != gb_spk_data_r_before)
  {
   //dac_output_voltage(DAC_CHANNEL_1, gb_spk_data_r);
    
   //CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
   SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, gb_spk_data_r, RTC_IO_PDAC1_DAC_S);   //dac_output
    
   //    //En ESP32S2 es base + 0x0120 y en esp32 base+0x009C sens_reg.h y soc.h
   //    #define SENS_SAR_DAC_CTRL2_REG          (DR_REG_SENS_BASE + 0x0120)
   //    #define SENS_DAC_CW_EN1_M  (BIT(24))
   //    //CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
   //    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, gb_spk_data, RTC_IO_PDAC1_DAC_S);   //dac_output              

   gb_spk_data_r_before= gb_spk_data_r;
  }

  gb_spk_data_r= gb_dac_buf_r[gb_dac_read];
  gb_dac_read++;
  //gb_dac_read= gb_dac_read & 0x07FF; //2048
  if (gb_dac_read>2047)
  {
   gb_dac_read=0;
  }

  //gb_dac_read= gb_dac_read & 0xFF;
  //gb_dac_read= gb_dac_read & 0x7FFF; //32768
 
  //gb_dac_read= gb_dac_read & 0x7FF;
   
  //gb_dac_read++;
  //if(gb_dac_read>=max_dac_buffer)
  //{
  // gb_dac_read=0;
  //}   
  //   return;
 }


// #ifdef use_lib_esp32sound_mono_channel
 //Reproducir WAVE RAW
  void IRAM_ATTR GeneraRAW()
  {//Pendiente de optimizar
   unsigned char valor;
   unsigned int pos;
   int i0,i1,i2,i3,iSum;
   unsigned char a[4];

   if (gb_snd_shift==4)
   {//Silencio
    for (unsigned char i=0;i<64;i++)
    {
     gb_dac_buf_r[gb_dac_write++]= 0x80;
     gb_dac_write= gb_dac_write & 0x7FF;
    }
    return;
   }

   if (gb_sound_mono_channel==1)
   {//Un solo canal   
    for (unsigned char i=0;i<64;i++)
    {          
     if (gb_snd_play[0]==1) 
     {          
      pos= gb_snd_pos_cur[0];
      valor= gb_snd_data[0][pos];
      pos++;
   
      if (pos>=(gb_snd_len[0]-1))
      {
       pos=0;
       gb_snd_play[0]=0;
       valor=0x80; //silencio
      }
      gb_snd_pos_cur[0]= pos;     
     }
     else
     {     
      valor= 0x80;
     }    
        
     if (valor!=0x80)
     {
      valor= valor+0x80;
     }    

     valor= (valor>>gb_snd_shift);

     //gb_dac_buf_r[gb_dac_write++]= ((valor==0)||(valor==0x80))?0x80:(valor>>gb_snd_shift);
     gb_dac_buf_r[gb_dac_write++]= ((valor==0)||(valor==0x80))?0x80:valor;
     gb_dac_write= gb_dac_write & 0x7FF;
    }
   }
   else
   {
    //4 canales
    for (unsigned char i=0;i<64;i++)
    {
     a[0]=a[1]=a[2]=a[3]=0;
     for (unsigned char j=0;j<4;j++)
     {
      if (gb_snd_play[j]==1) 
      {          
       pos= gb_snd_pos_cur[j];
       a[j]= gb_snd_data[j][pos];
       pos++;

       if (pos>=(gb_snd_len[j]-1))
       {
        pos=0;
        gb_snd_play[j]=0;       
       }
       gb_snd_pos_cur[j]= pos;       
      }
     } //fin for j
    
     if (a[0]>0x80){ i0= (int)(a[0]-256); } else {i0= a[0];}
     if (a[1]>0x80){ i1= (int)(a[1]-256); } else {i1= a[1];}
     if (a[2]>0x80){ i2= (int)(a[2]-256); } else {i2= a[2];}
     if (a[3]>0x80){ i3= (int)(a[3]-256); } else {i3= a[3];}     
   
     iSum= i0+i1+i2+i3;
     if (iSum>127) {iSum=127;}
     else
     {
      if(iSum<-127) {iSum=-127;}
     }
          
     gb_dac_buf_r[gb_dac_write++]= ((iSum+0x80)>>gb_snd_shift);
     gb_dac_write= gb_dac_write & 0x7FF;
    }//fin for i
   }
  } 
/*  
 #else
 void IRAM_ATTR GeneraRAW()
 {
  //valor 0x00 - 0x7F --> 0x80+valor
  //valor 0x80 - 0xFF --> valor-0x80
  //255 127
  //251 123    
  unsigned char valor;
  unsigned int pos;
  int signo;
  int media;

 
  for (unsigned char i=0;i<64;i++)
  {
   media=0;
   for (unsigned char j=0;j<4;j++)
   {
    if (gb_snd_play[j]==1) 
    {          
     pos= gb_snd_pos_cur[j];
     valor= gb_snd_data[j][pos];
     signo= (valor<0x80)?(int)valor:-(int)((unsigned char)(valor-0x80));
     pos++;
   
     if (pos>=(gb_snd_len[j]-1))
     {
      pos=0;
      gb_snd_play[j]=0;
      signo=0;
     }
     gb_snd_pos_cur[j]= pos;     
    }
    else
    {
     signo=0;
    }
        
    if (signo!=0){
     media= (media+signo)>>1;
    }
   }//fin for j
      
   valor= (media>=0)? (0x80+media): 0x80;

   gb_dac_buf_r[gb_dac_write++]= valor;
   gb_dac_write= gb_dac_write & 0x7FF;
  }//fin for i
 }
 #endif
*/ 
#endif


void ForcePSRAMVideo()
{
 //El modo 512x384 requiere PSRAM
 unsigned char *ptrAux;
 unsigned char idPtr1=0;
 unsigned char idPtr2=0;
 unsigned char idPtr3=0;
 
 #ifdef use_lib_log_serial
  Serial.printf("ForcePSRAMVideo\r\n");
 #endif

 #ifdef use_lib_video4buffers_psram
  //Ya tenia los 4 buffer en psram no hago nada
 #else
  //Si ya he usado PSRAM, no hago nada
  if (gb_forcePSRAMused==0)
  {
   gb_forcePSRAMused= 1;

   //Busco punteros
   for (unsigned char i=0;i<4;i++)
   {
    if (_curPagePtr1 == _pages[i]){ idPtr1= i; }
    if (_curPagePtr2 == _pages[i]){ idPtr2= i; }
    if (_curPagePtr3 == _pages[i]){ idPtr3= i; }    
   }

   Serial.printf("id1:%d id2:%d id3:%d\r\n",idPtr1,idPtr2,idPtr3);

   //Creo memoria y copio de SRAM a PSRAM
   for (unsigned char i=0;i<4;i++)
   {  
    //Copio en psram
    ptrAux= (unsigned char *)ps_malloc(32000);
    if (ptrAux!=NULL)
    {
     memcpy(&ptrAux[0],&gb_vram[i][0],32000);
     Serial.printf("Copy psram %d\r\n",i);
     free(gb_vram[i]); //libero SRAM
     Serial.printf("Free psram %d\r\n",i);
     gb_vram[i]= ptrAux;

     _pages[i] = gb_vram[i];
    }
    else
    {
     Serial.printf("psram null %d\r\n",i);
    }
   }

   //Resturo punteros apuntando a PSRAM
   _curPagePtr1= _pages[idPtr1];
   _curPagePtr2= _pages[idPtr2];
   _curPagePtr3= _pages[idPtr3];
   Serial.printf("Assign id and pointer Pages\r\n");

  }
 #endif 
}



void setup() 
{
 gb_setup_end= 0;
 gb_ramfree_ini= ESP.getFreeHeap();

 memset(gb_key_video,0xFF,sizeof(gb_key_video));
 
 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);
  Serial.printf("BEGIN Setup\r\n");
  Serial.printf("HEAP BEGIN %d\r\n", gb_ramfree_ini);
 #endif 

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif 

 kb_begin(); //Cargo teclado antes seleccion modo video
 delay(100);
 Serial.printf("keyboard %d\r\n", ESP.getFreeHeap());

 
 #ifdef use_lib_esp32sound
  memset((void *)gb_snd_play,0,sizeof(gb_snd_play));  

  memset(gb_dac_buf_r,0x80,sizeof(gb_dac_buf_r));
  dac_output_enable(DAC_CHANNEL_1);
  //dac_output_voltage(DAC_CHANNEL_1, 0x7f); //Evita llamar CLEAR_PERI_REG_MASK
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
  SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 0x7f, RTC_IO_PDAC1_DAC_S);

  gb_timerSound= timerBegin(0, 80, true); 
  timerAttachInterrupt(gb_timerSound, &onTimerSoundDAC, true);
  gb_timerRellenoSonido= timerBegin(1, 80, true);
  timerAttachInterrupt(gb_timerRellenoSonido, &GeneraRAW, true);  

  timerAlarmWrite(gb_timerSound, 125, true); //1000000 1 segundo  125 es 8000 hz
  timerAlarmWrite(gb_timerRellenoSonido, 8000, true); //1000000 1 segundo  8 ms

  Serial.printf("Wait\r\n");
  GeneraRAW(); //Relleno 8 primeros ms
  timerAlarmEnable(gb_timerRellenoSonido); //Lo arranco despues de rellenar buffer
  delay(8);
  Serial.printf("Init Play WAV\r\n");
  timerAlarmEnable(gb_timerSound); //Lo arranco despues de rellenar buffer
 #endif

 if(psramInit())
 {
  #ifdef use_lib_video4buffers_psram
   for (unsigned char i=0;i<4;i++)
   {
    gb_vram[i]= (unsigned char *)ps_malloc(32000);
   }
  #else
   for (unsigned char i=0;i<4;i++)
   {       
    gb_vram[i]= (unsigned char *)malloc(32000);
   }
  #endif
  Serial.printf("vram 4 buffers %d\r\n", ESP.getFreeHeap());

  Serial.println("\nPSRAM is correctly initialized");
  Serial.printf("HEAP %d\r\n", ESP.getFreeHeap());

  gb_memory = (unsigned char *)ps_malloc(614400);

  Serial.printf("gb_memory %d\r\n", ESP.getFreeHeap());

  //e = new Engine(stub, ".", ".");
  //Serial.printf("new Engine %d\r\n", ESP.getFreeHeap());

  ActivarVideoPorTeclas();
  
  gb_setup_end= 1;
 }
 else
 {
  Serial.println("PSRAM not available");
  gb_setup_end= 0;
 }


 gb_ramfree_setupEnd= ESP.getFreeHeap();
 #ifdef use_lib_log_serial  
  if (gb_use_debug==1){ Serial.printf("END SETUP %d\r\n", gb_ramfree_setupEnd); }
 #endif 

 //Serial.printf("END Setup %d\r\n", ESP.getFreeHeap());
}

void loop() 
{
 if (gb_setup_end==1)
 {
  Serial.printf("BEGIN loop\r\n");
  
  main(0,NULL);

  //Engine* e = new Engine(stub, dataPath, savePath);
  //e = new Engine(stub, ".", ".");
  e->init();
  e->run();

  //delete e;

  Serial.printf("END loop\r\n");
 }
 delay(1);
}
