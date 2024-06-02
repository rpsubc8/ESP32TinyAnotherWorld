#include "gbConfig.h"
#include "gbGlobals.h"
//#include "testvga.h" //Ya no se necesita
#include "hardware.h"
//#include "keys.h" //Ya no se necesita
#include "PS2Kbd.h"
#include <Arduino.h>

//volatile unsigned char keymap[256];
//volatile unsigned char oldKeymap[256];

//static unsigned char keymap[256];
//static unsigned char oldKeymap[256];
//static unsigned char keymap[32]; //256 DIV 8 = 32 bytes packet bit
volatile unsigned char gb_keymap[32]; //256 DIV 8 = 32 bytes packet bit

//unsigned int shift = 0; //Ya no se necesita
//byte lastcode = 0; //Ya no se necesita
unsigned char keyup = 0;
//boolean shift_presed = false;  //Ya no se necesita
//boolean symbol_pressed = false;  //Ya no se necesita
//byte rc = 0;  //Ya no se necesita

// #define DEBUG_LOG_KEYSTROKES 1

//volatile unsigned char gb_kb_bitcount = 0;
//volatile unsigned char gb_kb_incoming = 0;
//volatile unsigned int gb_kb_prev_ms = 0;

static unsigned char gb_kb_bitcount = 0;
static unsigned char gb_kb_incoming = 0;
static unsigned int gb_kb_prev_ms = 0; 

void IRAM_ATTR kb_interruptHandler()
{
 //static uint8_t bitcount = 0;
 //static uint8_t incoming = 0;
 //static uint32_t prev_ms = 0;
 unsigned int now_ms;
 unsigned char n, val;
 //unsigned char auxId;
 //unsigned char auxOffs;

 //int clock = digitalRead(KEYBOARD_CLK);
 //if (clock == 1)
 if (digitalRead(KEYBOARD_CLK) == 1)
 {
  return;
 }

 val = digitalRead(KEYBOARD_DATA);
 now_ms = millis(); 
 if ((now_ms - gb_kb_prev_ms) > 250)
 {
  gb_kb_bitcount = 0;
  gb_kb_incoming = 0;
 }
 gb_kb_prev_ms = now_ms;
 n = gb_kb_bitcount - 1;
 if (n <= 7)
 {
  gb_kb_incoming |= (val << n);
 }
 gb_kb_bitcount++;
 if (gb_kb_bitcount == 11)
 {
  //if (1) //revisar no se necesita
  //{
   if (keyup == 1) 
   {//Se libera tecla pasa a 1
    if (gb_keymap[gb_kb_incoming] == 0) 
    {
     //auxId= gb_kb_incoming>>3; //DIV 8
     //auxOffs= gb_kb_incoming & 0x07; //MOD 8
     //unsigned char antes= keymap[auxId];
     //antes= antes | (1<<auxOffs);
     gb_keymap[(gb_kb_incoming>>3)] |= (1<<(gb_kb_incoming & 0x07));

     //keymap[gb_kb_incoming]= 1;
    }
    else
    {
     //Serial.println("WARNING: Keyboard cleaned");
     //for (unsigned short int gg = 0; gg < 256; gg++)
     //{
     // keymap[gg]= 1;
     //}
     
     memset((void *)gb_keymap,0xFF,sizeof(gb_keymap));

     //memset((void *)keymap,1,sizeof(keymap));
    }
    keyup = 0;
   }
   else
   {//Se pulsa tecla pasa a 0
    //auxId= gb_kb_incoming>>3; //DIV 8
    //auxOffs= gb_kb_incoming & 0x07; //MOD 8
    //unsigned char antes= keymap[auxId];
    //antes= antes & ((~(1<<auxOffs)) & 0xFF);
    gb_keymap[(gb_kb_incoming>>3)] &= ((~(1<<(gb_kb_incoming & 0x07))) & 0xFF);
    //9 div 8 = 1   resto 1

    //keymap[gb_kb_incoming] = 0;
   }

   //#ifdef DEBUG_LOG_KEYSTROKES
   // #ifdef use_lib_log_serial
   //  Serial.printf("PS2Kbd[%s]: %02X\r\n", keyup ? " up " : "down", gb_kb_incoming);
   // #endif    
   //#endif

   //if (gb_kb_incoming == 240)
   //{
   // keyup = 1;
   //}
   //else
   //{
   // keyup = 0;
   //}
   keyup= (gb_kb_incoming == 240) ? 1 : 0;
  //}
  gb_kb_bitcount = 0;
  gb_kb_incoming = 0;
 }
}

////#define FIX_PERIBOARD_NOT_INITING
//#ifdef FIX_PERIBOARD_NOT_INITING
// #include "PS2KeyAdvanced.h"
// PS2KeyAdvanced ps2boot;
//#endif

void kb_begin()
{
// #ifdef FIX_PERIBOARD_NOT_INITING
//  //Configure the keyboard library
//  ps2boot.begin( KEYBOARD_DATA, KEYBOARD_CLK );
//  ps2boot.echo( );              // ping keyboard to see if there
//  delay( 6 );
//  ps2boot.read( );
//  delay( 6 );
//  ps2boot.terminate();
// #endif

    pinMode(KEYBOARD_DATA, INPUT_PULLUP);
    pinMode(KEYBOARD_CLK, INPUT_PULLUP);
    digitalWrite(KEYBOARD_DATA, true);
    digitalWrite(KEYBOARD_CLK, true);

    memset((void *)gb_keymap, 0xFF, sizeof(gb_keymap));
    
    //memset((void *)keymap, 1, sizeof(keymap));

    //memset((void *)oldKeymap, 1, sizeof(oldKeymap));

    attachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK), kb_interruptHandler, FALLING);
}

// Check if keymatrix is changed
//unsigned char isKeymapChanged()
//{ 
// return (keymap != oldKeymap);
//}

// Check if key is pressed and clean it
unsigned char checkAndCleanKey(unsigned char scancode)
{
 unsigned char auxId= scancode>>3; //DIV 8
 unsigned char auxOffs= scancode & 0x07; //MOD 8
 //unsigned char tecla= keymap[auxId];
 //unsigned char valor= (tecla>>auxOffs)&0x01;

 unsigned char valor= ((gb_keymap[auxId])>>auxOffs)&0x01;

 if (valor == 0)
 {
  //keymap[scancode] = 1;  
  //tecla= tecla | (1<<auxOffs);
  //keymap[auxId] = tecla;
  gb_keymap[auxId] |= (1<<auxOffs);

  return 1;     
 }
 return 0;
 
 //if (keymap[scancode] == 0)
 //{
 // keymap[scancode] = 1;
 // return 1;
 //}
 //return 0; 
}

//*****************************************
unsigned char checkKey(unsigned char scancode)
{
 unsigned char auxId= scancode>>3; //DIV 8
 unsigned char auxOffs= scancode & 0x07; //MOD 8

 unsigned char valor= (~((gb_keymap[auxId])>>auxOffs))&0x01;
  
 return valor;
// if (keymap[scancode] == 0)
// {
//  return true;
// }
// return false;
}

//void emulateKeyChange(unsigned char scancode, unsigned char isdown)
//{
// keymap[scancode] = isdown ? 0 : 1;
//}
