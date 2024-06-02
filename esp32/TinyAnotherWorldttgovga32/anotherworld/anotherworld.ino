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

unsigned char gb_paleta16[16];

const unsigned int *gb_ptrVideo_cur= VgaMode_vga_mode_320x200;
unsigned char gb_vga_videomode_cur= video_mode_vga320x200x70hz_bitluni; //2

unsigned char gb_vga_8colors=0; //1 8 colores, 0 64 colores

unsigned char gb_setup_end=0;

unsigned char gb_use_send_game_part=0;
#ifdef use_lib_mini_test
 unsigned char gb_use_game_part= 1; //proteccion
#else
 unsigned char gb_use_game_part= 2; //2 bypass protection
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
 memset(gb_paleta16,gb_const_colorNormal[0],16); //color directo
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

 unsigned char video=10;

/*
 unsigned int tiempo_ahora;
 unsigned int tiempo_antes;
 unsigned char video=255;
 #ifdef use_lib_ram_8KB
  unsigned short int auxAddr= 0x2000-8192;
 #else
  unsigned short int auxAddr= 0x2000;
 #endif
 
 tiempo_antes= tiempo_ahora= millis();
 
 while ((tiempo_ahora-tiempo_antes)<use_lib_boot_time_select_vga)
 {
  tiempo_ahora= millis();

  read_keyboard();
  #ifdef use_lib_keyboard_uart
   keyboard_uart_poll();
   ///read_keyboard_uart();
   do_keyboard_uart();
  #endif

  if (gb_Kmap[32]== 0) {video=0;} //0
  if (gb_Kmap[33]== 0) {video=1;} //1
  if (gb_Kmap[34]== 0) {video=2;} //2
  if (gb_Kmap[35]== 0) {video=3;} //3
  if (gb_Kmap[36]== 0) {video=4;} //4
  if (gb_Kmap[37]== 0) {video=5;} //5               
  if (gb_Kmap[38]== 0) {video=6;} //6
  if (gb_Kmap[39]== 0) {video=7;} //7 //fin modos 3 bpp

  if (gb_Kmap[40]== 0) {video=8;} //8
  if (gb_Kmap[41]== 0) {video=9;} //9
  if (gb_Kmap[1]== 0) {video=10;} //A
  if (gb_Kmap[2]== 0) {video=11;} //B
  if (gb_Kmap[3]== 0) {video=12;} //C
  if (gb_Kmap[4]== 0) {video=13;} //D
  if (gb_Kmap[5]== 0) {video=14;} //E
  if (gb_Kmap[6]== 0) {video=15;} //F
 }
 */

 if (video!=255)
 {//Cambio modo video
  if (gb_use_debug==1){ Serial.printf("Video mode change\r\n"); }
  switch (video)
  {
   case 0: gb_vga_videomode_cur= 0; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; break; //360x200x70hz bitluni(3bpp)
   case 1: gb_vga_videomode_cur= 1; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; break; //360x200x70hz bitluni apll(3bpp)
   case 2: gb_vga_videomode_cur= 2; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; break; //320x200x70hz bitluni(3bpp)
   case 3: gb_vga_videomode_cur= 3; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; break; //320x200x70hz fabgl(3bpp)
   case 4: gb_vga_videomode_cur= 4; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; break; //320x200x70hz bitluni apll(3bpp)
   case 5: gb_vga_videomode_cur= 5; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; break; //320x240x60hz bitluni(3bpp)
   case 6: gb_vga_videomode_cur= 6; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; break; //320x240x60hz fabgl(3bpp)
   case 7: gb_vga_videomode_cur= 7; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; break; //320x240x60hz bitluni apll(3bpp)

   case 8: gb_vga_videomode_cur= 0; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; break;
   case 9: gb_vga_videomode_cur= 1; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; break;
   case 10:gb_vga_videomode_cur= 2; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; break;
   case 11:gb_vga_videomode_cur= 3; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; break;
   case 12:gb_vga_videomode_cur= 4; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; break;
   case 13:gb_vga_videomode_cur= 5; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; break;
   case 14:gb_vga_videomode_cur= 6; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; break;
   case 15:gb_vga_videomode_cur= 7; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; break;   
   default: break;
  }

 }
 


 if (gb_use_debug==1){ Serial.printf("Video mode set %d\r\n",video); }

//   OSDUpdateAnchoAltoVGA(gb_vga_videomode_cur,is8colors);//Set is8colors gb_vga_8colors

   //vga_init(pin_config,VgaMode_vga_mode_320x240,false,0,0,0,0,0,0);
   vga_init((is8colors==1)?pin_config8:pin_config64,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3,usecustompll,is8colors, gb_vga_videomode_cur);
   gb_sync_bits= vga_get_sync_bits();
   gb_buffer_vga = vga_get_framebuffer();
   gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
   PrepareColorsBitluniVGA(); //Llamar despues de tener gb_sync_bits  
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


     case 0x61: case 0x41: gb_use_speed_fast=1; break; //A equivale F10 Rapido
     case 0x62: case 0x42: gb_use_speed_fast=0; break; //B equivale F11 Lento

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
      

     case 0x30: gb_use_game_part= 10; gb_use_send_game_part= 1; break; //0
     case 0x31: gb_use_game_part= 1; gb_use_send_game_part= 1; break; //1
     case 0x32: gb_use_game_part= 2; gb_use_send_game_part= 1; break; //2
     case 0x33: gb_use_game_part= 3; gb_use_send_game_part= 1; break; //3
     case 0x34: gb_use_game_part= 4; gb_use_send_game_part= 1; break; //4
     case 0x35: gb_use_game_part= 5; gb_use_send_game_part= 1; break; //5               
     case 0x36: gb_use_game_part= 6; gb_use_send_game_part= 1; break; //6
     case 0x37: gb_use_game_part= 7; gb_use_send_game_part= 1; break; //7     
     case 0x38: gb_use_game_part= 8; gb_use_send_game_part= 1; break; //8
     case 0x39: gb_use_game_part= 9; gb_use_send_game_part= 1; break; //9


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

 gb_use_keyb_up= checkKey(KEY_CURSOR_UP); //arriba
 gb_use_keyb_down= checkKey(KEY_CURSOR_DOWN); //abajo
 gb_use_keyb_left= checkKey(KEY_CURSOR_LEFT); //izquierda
 gb_use_keyb_right= checkKey(KEY_CURSOR_RIGHT); //derecha 

 gb_use_keyb_space= checkKey(PS2_KC_SPACE); //SPACE
 gb_use_keyb_return= checkKey(PS2_KC_ENTER); //RETURN

 //pulso= checkKey(PS2_KEY_0);
 //if (pulso==1)
 //{
 // //     gb_use_game_part= 10; gb_use_send_game_part= 1; 
 // Serial.printf("0\r\n");
 //} //0

 if (checkKey(PS2_KC_0)==1){ gb_use_game_part= 10; gb_use_send_game_part= 1;}  //10
 if (checkKey(PS2_KC_1)==1){ gb_use_game_part= 1; gb_use_send_game_part= 1; }  //1
 if (checkKey(PS2_KC_2)==1){ gb_use_game_part= 2; gb_use_send_game_part= 1; }  //2
 if (checkKey(PS2_KC_3)==1){ gb_use_game_part= 3; gb_use_send_game_part= 1; }  //3
 if (checkKey(PS2_KC_4)==1){ gb_use_game_part= 4; gb_use_send_game_part= 1; }  //4
 if (checkKey(PS2_KC_5)==1){ gb_use_game_part= 5; gb_use_send_game_part= 1; }  //5               
 if (checkKey(PS2_KC_6)==1){ gb_use_game_part= 6; gb_use_send_game_part= 1; }  //6
 if (checkKey(PS2_KC_7)==1){ gb_use_game_part= 7; gb_use_send_game_part= 1; }  //7     
 if (checkKey(PS2_KC_8)==1){ gb_use_game_part= 8; gb_use_send_game_part= 1; }  //8
 if (checkKey(PS2_KC_9)==1){ gb_use_game_part= 9; gb_use_send_game_part= 1; }  //9 

 if (checkKey(PS2_KC_A)==1){ gb_use_speed_fast=1; } //A equivale F10 Rapido
 if (checkKey(PS2_KC_B)==1){ gb_use_speed_fast=0; } //B equivale F11 Lento
 
}





void setup() 
{
 gb_setup_end= 0;
 Serial.begin(115200);
 Serial.printf("BEGIN Setup\r\n");
 Serial.printf("HEAP BEGIN %d\r\n", ESP.getFreeHeap());

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif 

 kb_begin(); //Cargo teclado antes seleccion modo video
 delay(100);
 Serial.printf("keyboard %d\r\n", ESP.getFreeHeap());

 for (unsigned char i=0;i<4;i++)
 {
  gb_vram[i]= (unsigned char *)malloc(32000);
 }
 Serial.printf("vram 4 buffers %d\r\n", ESP.getFreeHeap());

 if(psramInit())
 {
  Serial.println("\nPSRAM is correctly initialized");
 }
 else
 {
  Serial.println("PSRAM not available");
 }

 Serial.printf("HEAP %d\r\n", ESP.getFreeHeap());

 gb_memory = (unsigned char *)ps_malloc(614400);

 Serial.printf("gb_memory %d\r\n", ESP.getFreeHeap());


 //e = new Engine(stub, ".", ".");
 //Serial.printf("new Engine %d\r\n", ESP.getFreeHeap());


 ActivarVideoPorTeclas();

 
 Serial.printf("END Setup %d\r\n", ESP.getFreeHeap());
 gb_setup_end= 1;
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
}
