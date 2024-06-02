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

//#include <SDL.h>
#include "gbConfig.h"
#include "gbGlobals.h"
#include "main.h"
#include <Arduino.h>

//#include <SDL2/SDL.h> //JJ #include <SDL/SDL.h>
//JJ #include <SDL/SDL.h> //SDL1
#include "sys.h"
#include "util.h"


struct SDLStub : System 
{
 typedef void (SDLStub::*ScaleProc)(unsigned short int *dst, unsigned short int dstPitch, const unsigned short int *src, unsigned short int srcPitch, unsigned short int w, unsigned short int h); //JJ typedef void (SDLStub::*ScaleProc)(uint16_t *dst, uint16_t dstPitch, const uint16_t *src, uint16_t srcPitch, uint16_t w, uint16_t h);

 enum 
 {
  SCREEN_W = 320,
  SCREEN_H = 200,
  SOUND_SAMPLE_RATE = 22050
 };

 int DEFAULT_SCALE; //JJ int DEFAULT_SCALE = 3;	

//JJ SDL_Surface * _screen; //JJ SDL_Surface *_screen = nullptr;
//JJ	SDL_Window * _window; //JJ SDL_Window * _window = nullptr; //convertir SDL1
//JJ	SDL_Renderer * _renderer; //JJ SDL_Renderer * _renderer = nullptr; //convertir SDL1
 unsigned char _scale; //uint8_t _scale; //JJ uint8_t _scale = DEFAULT_SCALE;

 SDLStub();
    
 virtual ~SDLStub() {}
 virtual void init(const char *title);
 virtual void destroy();
 virtual void setPalette(const unsigned char *buf); //JJ virtual void setPalette(const uint8_t *buf);
 
 virtual void updateDisplay(const unsigned char *src); //JJ virtual void updateDisplay(const uint8_t *src);
 #ifdef use_lib_bitlunivga
  void updateDisplayBitluni(const unsigned char *buf);
 #endif
 virtual void processEvents();
 virtual void sleep(unsigned int duration); //JJ virtual void sleep(uint32_t duration);
 virtual unsigned int getTimeStamp(); //JJ virtual uint32_t getTimeStamp();
 virtual void startAudio(AudioCallback callback, void *param);
 virtual void stopAudio();
 virtual unsigned int getOutputSampleRate(); //JJ virtual uint32_t getOutputSampleRate();
 //JJ virtual int addTimer(uint32_t delay, TimerCallback callback, void *param); //convertir SDL1
 virtual int addTimer(unsigned int delay, TimerCallback callback, void *param); //convertir SDL1
 //virtual int addTimer(uint32_t delay, SDL_NewTimerCallback callback, void *param); //convertir SDL1
 virtual void removeTimer(int timerId);
 virtual void *createMutex();
 virtual void destroyMutex(void *mutex);
 virtual void lockMutex(void *mutex);
 virtual void unlockMutex(void *mutex);

 void prepareGfxMode();
 void cleanupGfxMode();
 void switchGfxMode();
};



SDLStub::SDLStub()
{
 //DEFAULT_SCALE = 3;
 DEFAULT_SCALE = 1;
//JJ _screen = nullptr;
//JJ _window = nullptr; //convertir SDL1
//JJ _renderer = nullptr; //convertir SDL1
 _scale = DEFAULT_SCALE;
}


void SDLStub::init(const char *title) 
{
//JJ SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
//	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
//JJ SDL_ShowCursor(SDL_DISABLE);

//JJ SDL_ShowCursor( SDL_ENABLE );
//JJ 	SDL_CaptureMouse(SDL_TRUE); //Pendiente portar

 memset(&input, 0, sizeof(input));
 _scale = DEFAULT_SCALE;
 prepareGfxMode();
}

void SDLStub::destroy() 
{
 cleanupGfxMode();
//JJ SDL_Quit();
}


/*JJ
void ProcesaPaletaTTGOVGA32(SDL_Color * pal, unsigned char *ptrPal)
{
 unsigned char aux,b1,b0,r,g,b;

// printf("PAL BEGIN\n");
 for (unsigned short int i=0;i<16;i++)
 {
//  printf("%i %02X %02X %02X  %02X %02X %02X\n",i,pal[i].r,pal[i].g,pal[i].b, ptrPal[i*3],ptrPal[(i*3)+1],ptrPal[(i*3)+2]);
  
  r= ptrPal[(i*3)];
  g= ptrPal[(i*3)+1];
  b= ptrPal[(i*3)+2];
  
  pal[i].r= ((r>>4)&0x03)*85;
  pal[i].g= ((g>>4)&0x03)*85;
  pal[i].b= ((b>>4)&0x03)*85;
  
  
  //b1= ((pal[i].r>>7) & 0x01);
  //b0= ((pal[i].r>>6) & 0x01);
  //aux= ((b1<<1)|b0)*85;
  //pal[i].r= aux;
  
  //b1= ((pal[i].g>>7) & 0x01);
  //b0= ((pal[i].g>>6) & 0x01);
  //aux= ((b1<<1)|b0)*85;
  //pal[i].g= aux;
  
  //b1= ((pal[i].b>>7) & 0x01);
  //b0= ((pal[i].b>>6) & 0x01);
  //aux= ((b1<<1)|b0)*85;
  //pal[i].b= aux;  
  
  
  
  //pal[i].r= (pal[i].r>>6)<<6;
  //pal[i].g= (pal[i].g>>6)<<6;
  //pal[i].b= (pal[i].b>>6)<<6; 
  
 }
// printf("PAL END\n\n");
// fflush(stdout);
 
 for (unsigned short int i=16;i<256;i++)
 {
  pal[i].r= pal[i].g= pal[i].b=0;
 }
}
*/

//JJ static SDL_Color palette[NUM_COLORS];
//JJ void SDLStub::setPalette(const uint8_t *p) 
void SDLStub::setPalette(const unsigned char *p) 
{
 unsigned char jj_paleta[16*3];
 unsigned char jj_cont=0;
 unsigned char r,g,b; 
 
 // The incoming palette is in 565 format.
 for (int i = 0; i < NUM_COLORS; ++i)
 {
  unsigned char c1 = *(p + 0); //JJ uint8_t c1 = *(p + 0);
  unsigned char c2 = *(p + 1); //JJ uint8_t c2 = *(p + 1);
//JJ  palette[i].r = (((c1 & 0x0F) << 2) | ((c1 & 0x0F) >> 2)) << 2; // r
//JJ  palette[i].g = (((c2 & 0xF0) >> 2) | ((c2 & 0xF0) >> 6)) << 2; // g
//JJ  palette[i].b = (((c2 & 0x0F) >> 2) | ((c2 & 0x0F) << 2)) << 2; // b
  
  jj_paleta[jj_cont]= (((c1 & 0x0F) << 2) | ((c1 & 0x0F) >> 2));
  r=(jj_paleta[jj_cont]>>4)&0x03;
  jj_cont++;
  jj_paleta[jj_cont]= (((c2 & 0xF0) >> 2) | ((c2 & 0xF0) >> 6));
  g=(jj_paleta[jj_cont]>>4)&0x03; 
  jj_cont++;
  jj_paleta[jj_cont]= (((c2 & 0x0F) >> 2) | ((c2 & 0x0F) << 2));
  b= (jj_paleta[jj_cont]>>4)&0x03;
  jj_cont++;

    
  //gb_paleta16[i]= (b<<4)|(g<<2)|r; //indice  
  gb_paleta16[i]= gb_const_colorNormal[((b<<4)|(g<<2)|r)]; //color directo

//JJ     palette[i].a = 0xFF; // convertir SDL1
  p += 2;
 }
 //JJ SDL_SetPaletteColors(_screen->format->palette, palette, 0, NUM_COLORS);
 
//JJ ProcesaPaletaTTGOVGA32(palette, jj_paleta);
 
//JJ SDL_SetPalette(_screen, SDL_LOGPAL|SDL_PHYSPAL, palette, 0, NUM_COLORS); //convertir SDL1
}

void SDLStub::prepareGfxMode() 
{
 int w = SCREEN_W;
 int h = SCREEN_H;

 //JJ _window = SDL_CreateWindow("Another World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w * _scale, h * _scale, SDL_WINDOW_SHOWN); //convertir SDL1
 //JJ _renderer = SDL_CreateRenderer(_window, -1, 0); //convertir SDL1
 //JJ _screen = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0); //convertir SDL1
  
//JJ SDL_putenv("SDL_VIDEO_WINDOW_POS=2,22");
//JJ _screen = SDL_SetVideoMode ((w * _scale), (h * _scale), 8, SDL_SWSURFACE | SDL_DOUBLEBUF);
//JJ gb_screen= _screen; 
//JJ SDL_WM_SetCaption ("ESP32 Another World", NULL);  
  
//JJ if (!_screen) 
//JJ {
//JJ  error("SDLStub::prepareGfxMode() unable to allocate _screen buffer");
//JJ  printf("SDLStub::prepareGfxMode() unable to allocate _screen buffer\n");
//JJ  fflush(stdout);
//JJ }
 // Upon resize during gameplay, the screen surface is re-created and a new palette is allocated.
 // This will result in an all-white surface palette displaying a window full of white until a
 // a palette is set by the VM.
 // To avoid this issue, we save the last palette locally and re-upload it each time. On game start-up this
 // is not requested.
 //JJ SDL_SetPaletteColors(_screen->format->palette, palette, 0, NUM_COLORS); //convertir SDL1
//JJ SDL_SetPalette(_screen, SDL_LOGPAL|SDL_PHYSPAL, palette, 0, NUM_COLORS);
}

#ifdef use_lib_bitlunivga
 //****************************************************************
 void SDLStub::updateDisplayBitluni(const unsigned char *src)
 {
  //El que marca el tiempo es VirtualMachine::op_blitFramebuffer() en vm.cpp
  //Llama video->updateDisplay(pageId); con tiempos
  
  unsigned short int height = SCREEN_H;
  unsigned short int y=0;
  unsigned char a0,a1,a2,a3,aux;
  unsigned int a32;
  unsigned int contOrig=0;
   
  //printf("updateDisplayBitluni\n");
  //fflush(stdout);
   
  //For each line
  while (height--) 
  {
   //One byte gives us two pixels, we only need to iterate w/2 times.
   //for (int i = 0; i < (SCREEN_W>>1); ++i)
   //{//Extract two palette indices from upper byte and lower byte.            
   // //gb_buffer_vga[y][((i * 2) + 0)^2]= gb_const_colorNormal[gb_paleta16[(*(src + i) >> 4)]]; //con indices
   // //gb_buffer_vga[y][((i * 2) + 1)^2]= gb_const_colorNormal[gb_paleta16[(*(src + i) & 0xF)]];
   //
   // gb_buffer_vga[y][((i * 2) + 0)^2]= gb_paleta16[(*(src + i) >> 4)]; //color directo
   // gb_buffer_vga[y][((i * 2) + 1)^2]= gb_paleta16[(*(src + i) & 0xF)];    
   //}

   for (unsigned int i = 0; i<80; i++)
   {
    aux= src[contOrig];
    contOrig++;
    a0= gb_paleta16[(aux>>4)]; //color directo
    a1= gb_paleta16[(aux & 0x0F)]; //color directo
    aux= src[contOrig];
    contOrig++;    
    a2= gb_paleta16[(aux>>4)]; //color directo
    a3= gb_paleta16[(aux & 0x0F)]; //color directo
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24); //ESP32
    gb_buffer_vga32[y][i]= a32;
   }

   y++;
   //src += SCREEN_W>>1; //Ya no lo necesito   
  }        
   
   
   
  //bitluni_dump_vga();
//JJ  SDL_Flip(_screen); //convertir SDL1
 } 
#endif

//****************************************************************
//JJ void SDLStub::updateDisplay(const uint8_t *src)
void SDLStub::updateDisplay(const unsigned char *src)
{
//JJ unsigned short int height = SCREEN_H; //JJ uint16_t height = SCREEN_H;
//JJ unsigned char* p = (unsigned char*)_screen->pixels; //JJ uint8_t* p = (uint8_t*)_screen->pixels;
 
 #ifdef use_lib_bitlunivga
  updateDisplayBitluni(src);
  return;
 #endif

/*JJ
 //For each line
 while (height--) 
 {
  //One byte gives us two pixels, we only need to iterate w/2 times.
  for (int i = 0; i < SCREEN_W / 2; ++i) 
  {
   //Extract two palette indices from upper byte and lower byte.
   p[i * 2 + 0] = *(src + i) >> 4;
   p[i * 2 + 1] = *(src + i) & 0xF;
  }
  p += _screen->pitch;
  src += SCREEN_W/2;
 }

 //JJ SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, _screen); //convertir SDL1
 //JJ SDL_RenderCopy(_renderer, texture, nullptr, nullptr); //convertir SDL1
 //JJ SDL_RenderPresent(_renderer); //convertir SDL1
 //JJ SDL_DestroyTexture(texture); //convertir SDL1
  
//JJ SDL_Flip(_screen); //convertir SDL1
*/
}


void SDLStub::processEvents() 
{   
 #ifdef use_lib_keyboard_uart
  keyboard_uart_poll();
  ///read_keyboard_uart();
  do_keyboard_uart();
 #else
  read_keyboard();
 #endif
 

 if (gb_use_keyb_left==1){ input.dirMask |= PlayerInput::DIR_LEFT; }
 else { input.dirMask &= ~PlayerInput::DIR_LEFT; }

 if (gb_use_keyb_right==1){ input.dirMask |= PlayerInput::DIR_RIGHT; }
 else { input.dirMask &= ~PlayerInput::DIR_RIGHT; }

 if (gb_use_keyb_up==1){ input.dirMask |= PlayerInput::DIR_UP; }
 else { input.dirMask &= ~PlayerInput::DIR_UP; }

 if (gb_use_keyb_down==1){ input.dirMask |= PlayerInput::DIR_DOWN; }
 else { input.dirMask &= ~PlayerInput::DIR_DOWN; }  

 if ((gb_use_keyb_space==1)||(gb_use_keyb_return==1)) {input.button = true;}
 else {input.button = false;}


 /*JJ   
 SDL_Event ev;
 while(SDL_PollEvent(&ev)) 
 {
  switch (ev.type) 
  {
   case SDL_QUIT:
	input.quit = true;
	break;
   case SDL_KEYUP:
	switch(ev.key.keysym.sym) 
    {
     case SDLK_1: gb_use_game_part= 1; gb_use_send_game_part= 1; break;
     case SDLK_2: gb_use_game_part= 2; gb_use_send_game_part= 1; break;
     case SDLK_3: gb_use_game_part= 3; gb_use_send_game_part= 1; break;
     case SDLK_4: gb_use_game_part= 4; gb_use_send_game_part= 1; break;
     case SDLK_5: gb_use_game_part= 5; gb_use_send_game_part= 1; break;
     case SDLK_6: gb_use_game_part= 6; gb_use_send_game_part= 1; break;
     case SDLK_7: gb_use_game_part= 7; gb_use_send_game_part= 1; break;
     case SDLK_8: gb_use_game_part= 8; gb_use_send_game_part= 1; break;
     case SDLK_9: gb_use_game_part= 9; gb_use_send_game_part= 1; break;
     case SDLK_0: gb_use_game_part= 10; gb_use_send_game_part= 1; break;
     
     case SDLK_F10: gb_use_speed_fast=1; break;
     case SDLK_F11: gb_use_speed_fast=0; break;
                              
	 case SDLK_LEFT:
	  input.dirMask &= ~PlayerInput::DIR_LEFT;
	  break;
	 case SDLK_RIGHT:
	  input.dirMask &= ~PlayerInput::DIR_RIGHT;
	  break;
	 case SDLK_UP:
	  input.dirMask &= ~PlayerInput::DIR_UP;
	  break;
	 case SDLK_DOWN:
	  input.dirMask &= ~PlayerInput::DIR_DOWN;
	  break;
	 case SDLK_SPACE:
	 case SDLK_RETURN:
	  input.button = false;
	  break;
	 case SDLK_ESCAPE:
      input.quit = true;
	  break;
	}
	break;
   case SDL_KEYDOWN:
	if (ev.key.keysym.mod & KMOD_CTRL) 
    {
     if (ev.key.keysym.sym == SDLK_x) 
     {
      input.quit = true;
     }
     else 
      if (ev.key.keysym.sym == SDLK_s) 
      {
	   input.save = true;
	  }
      else 
       if (ev.key.keysym.sym == SDLK_l) 
       {
		input.load = true;
	   }
       else 
        if (ev.key.keysym.sym == SDLK_KP_PLUS) 
        {
		 input.stateSlot = 1;
		}
        else 
         if (ev.key.keysym.sym == SDLK_KP_MINUS) 
         {
		  input.stateSlot = -1;
		 }
    break;
  }
  
  input.lastChar = ev.key.keysym.sym;
  switch(ev.key.keysym.sym) 
  {
   case SDLK_LEFT:
    input.dirMask |= PlayerInput::DIR_LEFT;
    break;
   case SDLK_RIGHT:
    input.dirMask |= PlayerInput::DIR_RIGHT;
    break;
   case SDLK_UP:
	input.dirMask |= PlayerInput::DIR_UP;
	break;
   case SDLK_DOWN:
    input.dirMask |= PlayerInput::DIR_DOWN;
    break;
   case SDLK_SPACE:
   case SDLK_RETURN:
    input.button = true;
    break;
   case SDLK_c:
	input.code = true;
	break;
   case SDLK_p:
    input.pause = true;
    break;
   case SDLK_TAB :
    _scale = _scale + 1;
    if (_scale > 4) 
    { 
     _scale = 1; 
    }      
    switchGfxMode();
    break;
   default:
    break;
  }
  break;
 default:
  break;
}
}
*/
}

//JJ void SDLStub::sleep(uint32_t duration) 
void SDLStub::sleep(unsigned int duration) 
{
 delay(duration); //JJ SDL_Delay(duration);
}

//JJ uint32_t SDLStub::getTimeStamp() 
unsigned int SDLStub::getTimeStamp() 
{
 return millis(); //JJ SDL_GetTicks();	
}

void SDLStub::startAudio(AudioCallback callback, void *param) 
{
 /*JJ   
 SDL_AudioSpec desired;
 memset(&desired, 0, sizeof(desired));

 desired.freq = SOUND_SAMPLE_RATE;
 desired.format = AUDIO_U8;
 desired.channels = 1;
 desired.samples = 2048;
 desired.callback = callback;
 desired.userdata = param;
 if (SDL_OpenAudio(&desired, NULL) == 0) 
 {
  SDL_PauseAudio(0);
 }
 else 
 {
  error("SDLStub::startAudio() unable to open sound device");
 }
 */
}

void SDLStub::stopAudio() 
{
 //JJ SDL_CloseAudio();
}

//JJ uint32_t SDLStub::getOutputSampleRate() 
unsigned int SDLStub::getOutputSampleRate() 
{
 return SOUND_SAMPLE_RATE;
}

//JJ int SDLStub::addTimer(uint32_t delay, TimerCallback callback, void *param) //convertir SDL1
int SDLStub::addTimer(unsigned int delay, TimerCallback callback, void *param) //convertir SDL1
{
 //JJ return SDL_AddTimer(delay, (SDL_TimerCallback)callback, param); //convertir SDL1
//JJ return SDL_AddTimer(delay, callback, param); //convertir SDL1
 return 0;
}

void SDLStub::removeTimer(int timerId) 
{
//JJ	SDL_RemoveTimer(timerId); //convertir SDL1
}

void *SDLStub::createMutex() 
{
 //JJ return SDL_CreateMutex();
 return NULL; //revisar ESP32
}

void SDLStub::destroyMutex(void *mutex) 
{
 //JJ SDL_DestroyMutex((SDL_mutex *)mutex);
}

void SDLStub::lockMutex(void *mutex) 
{
 //JJ SDL_mutexP((SDL_mutex *)mutex);
}

void SDLStub::unlockMutex(void *mutex) 
{
 //JJ SDL_mutexV((SDL_mutex *)mutex);
}



void SDLStub::cleanupGfxMode() 
{
//JJ if (_screen) 
//JJ {
//JJ  SDL_FreeSurface(_screen);
//JJ  _screen = 0;
//JJ }

//JJ	if (_window) //convertir SDL1
//JJ    {//convertir SDL1
	  //JJ SDL_DestroyWindow(_window); //convertir SDL1
	  //JJ _window = nullptr; //convertir SDL1
//JJ	}//convertir SDL1

//JJ if (_screen) 
//JJ {
//JJ  SDL_FreeSurface(_screen);
//JJ  _screen = nullptr;
//JJ }
}

void SDLStub::switchGfxMode() 
{
 cleanupGfxMode();
 prepareGfxMode();
}

SDLStub sysImplementation;
System *stub = &sysImplementation;

