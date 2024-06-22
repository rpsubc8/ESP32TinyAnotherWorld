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

#include "gbConfig.h"
#include "gbGlobals.h"
#include "main.h"

#include "engine.h"
//JJ #include "file.h" //(no quiero ficheros ni librerias)
//JJ #include "serializer.h" //pendiente de portar (no quiero ficheros ni librerias)
#include "sys.h"
#include "parts.h"
#include <Arduino.h>
#include "osd.h"

Engine::Engine(System *paramSys, const char *dataDir, const char *saveDir)
	: sys(paramSys), vm(&mixer, &res, &player, &video, sys), mixer(sys), res(&video, dataDir), 
	player(&mixer, &res, sys), video(&res, sys), _dataDir(dataDir), _saveDir(saveDir), _stateSlot(0) {
}

void Engine::CargaParteJuego(unsigned char apart)
{
   vm.init();
  
   unsigned short int part= GAME_PART_FIRST;
   switch(apart)
   {
    case 0: part= GAME_PART_FIRST; break;
    case 1: part= GAME_PART1; break;
    case 2: part= GAME_PART2; break;
    case 3: part= GAME_PART3; break;
    case 4: part= GAME_PART4; break;
    case 5: part= GAME_PART5; break;
    case 6: part= GAME_PART6; break;
    case 7: part= GAME_PART7; break;
    case 8: part= GAME_PART8; break;
    case 9: part= GAME_PART9; break;
    case 10: part= GAME_PART10; break;
    case 11: part= GAME_PART_LAST; break;
   }
  
   vm.initForPart(part);     
}

void Engine::run() 
{

 Serial.printf("Engine RUN BEGIN\r\n");
 //fflush(stdout);
 
 while (!sys->input.quit) 
 {  
  if (gb_do_action_key_reset==1)
  {
   gb_do_action_key_reset= 0;
   //sys->input.quit= true;   
   #ifdef use_lib_ultra_mini_test
    gb_use_game_part= 1;
   #else   
    gb_use_game_part= 2;
   #endif
   gb_use_send_game_part= 1;
  }

  vm.checkThreadRequests();
  vm.inp_updatePlayer();
  processInput();
  
  //Procesar acciones
  if (gb_do_action_key_f1==1)
  {
   gb_do_action_key_f1=0;
   gb_show_osd_main_menu= 1;
   do_tinyOSD();     
  }

  vm.hostFrame();
  
  if (gb_use_send_game_part == 1)
  {
   gb_use_send_game_part=0;
  
   Serial.printf("Force load Part:%d\r\n",gb_use_game_part);
   //fflush(stdout);
   
   CargaParteJuego(gb_use_game_part);  
  }
 
 }
 Serial.printf("Engine RUN END\r\n");
 //fflush(stdout);
}

Engine::~Engine()
{
 finish();
 sys->destroy();
}


void Engine::init() 
{
 //Init system
 sys->init("Out Of This World");
	
 Serial.printf("Out Of This World\r\n");
 Serial.printf("ENGINE Init BEGIN\r\n");
 //fflush(stdout);

 video.init();

 res.allocMemBlock();

 #ifdef use_lib_dataflash
  res.readEntriesFlash();
 #else
  res.readEntries();
 #endif 

 vm.init();

 mixer.init();

 player.init();

 unsigned short int part = GAME_PART1; //JJ uint16_t part = GAME_PART1;  // This game part is the protection screen
#ifdef BYPASS_PROTECTION
  part = GAME_PART2;
#endif

  switch(gb_use_game_part)
  {
   case 0: part= GAME_PART_FIRST; break;
   case 1: part= GAME_PART1; break;
   case 2: part= GAME_PART2; break;
   case 3: part= GAME_PART3; break;
   case 4: part= GAME_PART4; break;
   case 5: part= GAME_PART5; break;
   case 6: part= GAME_PART6; break;
   case 7: part= GAME_PART7; break;
   case 8: part= GAME_PART8; break;
   case 9: part= GAME_PART9; break;
   case 10: part= GAME_PART10; break;
  }
  
  vm.initForPart(part);



  // Try to cheat here. You can jump anywhere but the VM crashes afterward.
	// Starting somewhere is probably not enough, the variables and calls return are probably missing.
	//vm.initForPart(GAME_PART2); // Skip protection screen and go directly to intro
	//vm.initForPart(GAME_PART3); // CRASH
	//vm.initForPart(GAME_PART4); // Start directly in jail but then crash
	//vm.initForPart(GAME_PART5);   //CRASH
	//vm.initForPart(GAME_PART6);   // Start in the battlechar but CRASH afteward
	//vm.initForPart(GAME_PART7); //CRASH
	//vm.initForPart(GAME_PART8); //CRASH
	//vm.initForPart(GAME_PART9); // Green screen not doing anything
	
 Serial.printf("ENGINE Init END\r\n");
 //fflush(stdout);
}

void Engine::finish() 
{
 player.free();
 mixer.free();
 //JJ res.freeMemBlock();
}

void Engine::processInput() 
{
 if (sys->input.load) 
 {
  loadGameState(_stateSlot);
  sys->input.load = false;
 }
 if (sys->input.save) 
 {
  saveGameState(_stateSlot, "quicksave");
  sys->input.save = false;
 }
 if (sys->input.stateSlot != 0) 
 {
  signed char slot = _stateSlot + sys->input.stateSlot; //JJ int8_t slot = _stateSlot + sys->input.stateSlot;
  if (slot >= 0 && slot < MAX_SAVE_SLOTS) 
  {
   _stateSlot = slot;
   debug(DBG_INFO, "Current game state slot is %d", _stateSlot);
  }
  sys->input.stateSlot = 0;
 }
}

//JJ void Engine::makeGameStateName(uint8_t slot, char *buf) 
void Engine::makeGameStateName(unsigned char slot, char *buf) 
{
 sprintf(buf, "raw.s%02d", slot);
}

//JJ void Engine::saveGameState(uint8_t slot, const char *desc) 
void Engine::saveGameState(unsigned char slot, const char *desc) 
{
 /*JJ pendiente de portar (no quiero ficheros ni librerias)
 char stateFile[20];
 makeGameStateName(slot, stateFile);
 File f(true);
 if (!f.open(stateFile, _saveDir, "wb")) 
 {
  warning("Unable to save state file '%s'", stateFile);
 }
 else 
 {
  // header
  f.writeUint32BE('AWSV');
  f.writeUint16BE(Serializer::CUR_VER);
  f.writeUint16BE(0);
  char hdrdesc[32];
  strncpy(hdrdesc, desc, sizeof(hdrdesc) - 1);
  f.write(hdrdesc, sizeof(hdrdesc));
  // contents
  Serializer s(&f, Serializer::SM_SAVE, res._memPtrStart);
  vm.saveOrLoad(s);
  res.saveOrLoad(s);
  video.saveOrLoad(s);
  player.saveOrLoad(s);
  mixer.saveOrLoad(s);
  if (f.ioErr()) 
  {
   warning("I/O error when saving game state");
  }
  else 
  {
   debug(DBG_INFO, "Saved state to slot %d", _stateSlot);
  }
 }
 */
}

//JJ void Engine::loadGameState(uint8_t slot)
void Engine::loadGameState(unsigned char slot)
{
 /*JJ pendiente de portar (no quiero ficheros ni librerias) 
 char stateFile[20];
 makeGameStateName(slot, stateFile);
 File f(true);
 if (!f.open(stateFile, _saveDir, "rb")) 
 {
  warning("Unable to open state file '%s'", stateFile);
 }
 else 
 {
  unsigned int id = f.readUint32BE(); //JJ uint32_t id = f.readUint32BE();
  if (id != 'AWSV') 
  {
   warning("Bad savegame format");
  }
  else 
  {
   // mute
   player.stop();
   mixer.stopAll();
   // header
   unsigned short int ver = f.readUint16BE(); //JJ uint16_t ver = f.readUint16BE();
   f.readUint16BE();
   char hdrdesc[32];
   f.read(hdrdesc, sizeof(hdrdesc));
   // contents
   Serializer s(&f, Serializer::SM_LOAD, res._memPtrStart, ver);
   vm.saveOrLoad(s);
   res.saveOrLoad(s);
   video.saveOrLoad(s);
   player.saveOrLoad(s);
   mixer.saveOrLoad(s);
  }
  if (f.ioErr()) 
  {
   warning("I/O error when loading game state");
  }
  else 
  {
   debug(DBG_INFO, "Loaded state from slot %d", _stateSlot);
  }
 }
 */
}


