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
#include "resource.h"
#include "bank.h"
//JJ #include "file.h" (no quiero ficheros ni librerias)
//JJ #include "serializer.h" //pendiente de portar (no quiero ficheros ni librerias)
#include "video.h"
#include "util.h"
#include "parts.h"
#include <Arduino.h>

#ifdef use_lib_dataflash
 #include "datamemlistbin.h"
#endif 

Resource::Resource(Video *vid, const char *dataDir) 
	: video(vid), _dataDir(dataDir), currentPartId(0),requestedNextPart(0) {
}

//JJ void Resource::readBank(const MemEntry *me, uint8_t *dstBuf) 
void Resource::readBank(const MemEntry *me, unsigned char *dstBuf) 
{
 unsigned short int n = me - _memList; //JJ uint16_t n = me - _memList;
 debug(DBG_BANK, "Resource::readBank(%d)", n);

 Bank bk(_dataDir);
 #ifdef use_lib_dataflash
  if (!bk.readFlash(me, dstBuf)) 
  {
   error("Resource::readBank() unable to unpack entry %d\n", n);
  } 
 #else
  if (!bk.read(me, dstBuf)) 
  {
   error("Resource::readBank() unable to unpack entry %d\n", n);
  }
 #endif 
 
 //Serial.printf("Resource::readBank bank%d\r\n",n);
 //fflush(stdout);
}

static const char *resTypeToString(unsigned int type)
{
 static const char* resTypes[]=
 {
  "RT_SOUND",
  "RT_MUSIC",
  "RT_POLY_ANIM",
  "RT_PALETTE",
  "RT_BYTECODE",
  "RT_POLY_CINEMATIC"
 };
 if (type >= (sizeof(resTypes) / sizeof(const char *)))
 {
  return "RT_UNKNOWN";
 }
 return resTypes[type];
}

#define RES_SIZE 0
#define RES_COMPRESSED 1
int resourceSizeStats[7][2];
#define STATS_TOTAL_SIZE 6
int resourceUnitStats[7][2];




#ifdef use_lib_dataflash
 unsigned char Resource::readByteFlash(const unsigned char *ptr,unsigned int * addr,unsigned int tope)
 {  
  unsigned char aux=0;
  
  if ((*addr)<tope)
  {
   aux= gb_memlist_bin[(*addr)];  
   *addr= (*addr)+1;
  }  
  return aux;
 }
 
 unsigned short int Resource::readUint16BEFlash(const unsigned char *ptr,unsigned int * addr,unsigned int tope)
 {
  unsigned char hi=0;
  unsigned char lo=0;

  if ((*addr)<tope)
  {
   hi = readByteFlash(ptr,addr,tope); //JJ uint8_t hi = readByte();
   lo = readByteFlash(ptr,addr,tope); //JJ uint8_t lo = readByte();
  }
  return ((hi << 8) | lo);
 }
 
 unsigned int Resource::readUint32BEFlash(const unsigned char *ptr,unsigned int * addr,unsigned int tope)
 {
  unsigned short int hi=0;
  unsigned short int lo=0;
            
  if ((*addr)<tope)
  {
   hi = readUint16BEFlash(ptr,addr,tope); //JJ uint16_t hi = readUint16BE();
   lo = readUint16BEFlash(ptr,addr,tope); //JJ uint16_t lo = readUint16BE();
  }
  return ((hi << 16) | lo);
 }
 
 void Resource::readEntriesFlash()
 {
  int resourceCounter = 0;
  unsigned int addrFile=0;
  
  #ifdef use_log_readEntriesFlash
   Serial.printf("readEntriesFlash BEGIN\r\n");
  #endif 
  //fflush(stdout);
  
  //Prepare stats array
  memset(resourceSizeStats,0,sizeof(resourceSizeStats));
  memset(resourceUnitStats,0,sizeof(resourceUnitStats));

  _numMemList = 0;
  MemEntry *memEntry = _memList;
  #ifdef use_log_readEntriesFlash
   Serial.printf("_numMemList:%d ARRAYSIZE(_memList):%d Size memlist:%d\n",_numMemList,ARRAYSIZE(_memList),gb_memlist_bin_size);
  #endif 

  while (1)
  {
   memEntry->state = readByteFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->type = readByteFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->bufPtr = 0; readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->unk4 = readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->rankNum = readByteFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->bankId = readByteFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->bankOffset = readUint32BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->unkC = readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->packedSize = readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->unk10 = readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   memEntry->size = readUint16BEFlash(gb_memlist_bin,&addrFile,gb_memlist_bin_size);
   
   //printf("addrFile:%d\n",addrFile);
   //fflush(stdout);
   
   if (memEntry->state == MEMENTRY_STATE_END_OF_MEMLIST) 
   {
    #ifdef use_log_readEntriesFlash
     Serial.printf("break MEMENTRY_STATE_END_OF_MEMLIST\r\n");
    #endif 
    //fflush(stdout);
    break;
   }
  
   if (addrFile>=gb_memlist_bin_size)
   {
    #ifdef use_log_readEntriesFlash     
     Serial.printf("break end file\r\n");
    #endif 
    //fflush(stdout);                                     
    break;
   }
   
   //Memory tracking
   if (memEntry->packedSize==memEntry->size)
   {
    resourceUnitStats[memEntry->type][RES_SIZE] ++;
    resourceUnitStats[STATS_TOTAL_SIZE][RES_SIZE] ++;
   }
   else
   {
    resourceUnitStats[memEntry->type][RES_COMPRESSED] ++;
    resourceUnitStats[STATS_TOTAL_SIZE][RES_COMPRESSED] ++;
   }

   resourceSizeStats[memEntry->type][RES_SIZE] += memEntry->size;
   resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE] += memEntry->size;
   resourceSizeStats[memEntry->type][RES_COMPRESSED] += memEntry->packedSize;
   resourceSizeStats[STATS_TOTAL_SIZE][RES_COMPRESSED] += memEntry->packedSize;

   #ifdef use_log_readEntriesFlash   				
    Serial.printf("R:0x%02X, %-17s size=%5d (compacted gain=%2.0f%%)\r\n",
     resourceCounter,
     resTypeToString(memEntry->type),
     memEntry->size,
     memEntry->size ? (memEntry->size-memEntry->packedSize) / (float)memEntry->size * 100.0f : 0.0f);
    //fflush(stdout);
   #endif 

   resourceCounter++;

   _numMemList++;
   memEntry++;   
  }
  
  
  #ifdef use_log_readEntriesFlash
   Serial.printf("readEntriesFlash END\r\n");   
   //fflush(stdout);
  #endif 
 }
#else
/*
	Read all entries from memlist.bin. Do not load anything in memory,
	this is just a fast way to access the data later based on their id.
*/
void Resource::readEntries()
{	
 File f;
 int resourceCounter = 0;

 Serial.printf("Resource readEntries BEGIN\r\n");	
 //fflush(stdout);

 if (!f.open("memlist.bin", _dataDir)) 
 {
  error("Resource::readEntries() unable to open 'memlist.bin' file\n");
  //Error will exit() no need to return or do anything else.
	
  Serial.printf("unable to open 'memlist.bin' file\n");
  //fflush(stdout);
 }

 //Prepare stats array
 memset(resourceSizeStats,0,sizeof(resourceSizeStats));
 memset(resourceUnitStats,0,sizeof(resourceUnitStats));

 _numMemList = 0;
 MemEntry *memEntry = _memList;
 while (1) 
 {
  assert(_numMemList < ARRAYSIZE(_memList));
  memEntry->state = f.readByte();
  memEntry->type = f.readByte();
  memEntry->bufPtr = 0; f.readUint16BE();
  memEntry->unk4 = f.readUint16BE();
  memEntry->rankNum = f.readByte();
  memEntry->bankId = f.readByte();
  memEntry->bankOffset = f.readUint32BE();
  memEntry->unkC = f.readUint16BE();
  memEntry->packedSize = f.readUint16BE();
  memEntry->unk10 = f.readUint16BE();
  memEntry->size = f.readUint16BE();

  if (memEntry->state == MEMENTRY_STATE_END_OF_MEMLIST) 
  {
   break;
  }

  //Memory tracking
  if (memEntry->packedSize==memEntry->size)
  {
   resourceUnitStats[memEntry->type][RES_SIZE] ++;
   resourceUnitStats[STATS_TOTAL_SIZE][RES_SIZE] ++;
  }
  else
  {
   resourceUnitStats[memEntry->type][RES_COMPRESSED] ++;
   resourceUnitStats[STATS_TOTAL_SIZE][RES_COMPRESSED] ++;
  }

  resourceSizeStats[memEntry->type][RES_SIZE] += memEntry->size;
  resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE] += memEntry->size;
  resourceSizeStats[memEntry->type][RES_COMPRESSED] += memEntry->packedSize;
  resourceSizeStats[STATS_TOTAL_SIZE][RES_COMPRESSED] += memEntry->packedSize;

  debug(DBG_RES, "R:0x%02X, %-17s size=%5d (compacted gain=%2.0f%%)",
  resourceCounter,
  resTypeToString(memEntry->type),
  memEntry->size,
  memEntry->size ? (memEntry->size-memEntry->packedSize) / (float)memEntry->size * 100.0f : 0.0f);
				
  Serial.printf("R:0x%02X, %-17s size=%5d (compacted gain=%2.0f%%)\r\n",
   resourceCounter,
   resTypeToString(memEntry->type),
   memEntry->size,
   memEntry->size ? (memEntry->size-memEntry->packedSize) / (float)memEntry->size * 100.0f : 0.0f);
  //fflush(stdout);

  resourceCounter++;

  _numMemList++;
  memEntry++;
 }

 debug(DBG_RES,"\n");
 debug(DBG_RES,"Total # resources: %d",resourceCounter);
 debug(DBG_RES,"Compressed       : %d",resourceUnitStats[STATS_TOTAL_SIZE][RES_COMPRESSED]);
 debug(DBG_RES,"Uncompressed     : %d",resourceUnitStats[STATS_TOTAL_SIZE][RES_SIZE]);
 debug(DBG_RES,"Note: %2.0f%% of resources are compressed.",100*resourceUnitStats[STATS_TOTAL_SIZE][RES_COMPRESSED]/(float)resourceCounter);

 debug(DBG_RES,"\n");
 debug(DBG_RES,"Total size (uncompressed) : %7d bytes.",resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE]);
 debug(DBG_RES,"Total size (compressed)   : %7d bytes.",resourceSizeStats[STATS_TOTAL_SIZE][RES_COMPRESSED]);
 debug(DBG_RES,"Note: Overall compression gain is : %2.0f%%.",
	(resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE] - resourceSizeStats[STATS_TOTAL_SIZE][RES_COMPRESSED])/(float)resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE]*100);

 debug(DBG_RES,"\n");
 for(int i=0 ; i < 6 ; i++)
 {
  debug(DBG_RES,"Total %-17s unpacked size: %7d (%2.0f%% of total unpacked size) packedSize %7d (%2.0f%% of floppy space) gain:(%2.0f%%)",
   resTypeToString(i),
   resourceSizeStats[i][RES_SIZE],
   resourceSizeStats[i][RES_SIZE] / (float)resourceSizeStats[STATS_TOTAL_SIZE][RES_SIZE] * 100.0f,
   resourceSizeStats[i][RES_COMPRESSED],
   resourceSizeStats[i][RES_COMPRESSED] / (float)resourceSizeStats[STATS_TOTAL_SIZE][RES_COMPRESSED] * 100.0f,
   (resourceSizeStats[i][RES_SIZE] - resourceSizeStats[i][RES_COMPRESSED]) / (float)resourceSizeStats[i][RES_SIZE] * 100.0f);
 }
 debug(DBG_RES,"Note: Damn you sound compression rate!");

 debug(DBG_RES,"\nTotal bank files:              %d",resourceUnitStats[STATS_TOTAL_SIZE][RES_SIZE]+resourceUnitStats[STATS_TOTAL_SIZE][RES_COMPRESSED]);
 for(int i=0 ; i < 6 ; i++)
 {
  debug(DBG_RES,"Total %-17s files: %3d",resTypeToString(i),resourceUnitStats[i][RES_SIZE]+resourceUnitStats[i][RES_COMPRESSED]);
  Serial.printf("Total %-17s files: %3d\r\n",resTypeToString(i),resourceUnitStats[i][RES_SIZE]+resourceUnitStats[i][RES_COMPRESSED]);	
  //fflush(stdout);
 }


 Serial.printf("Resource readEntries END\r\n");
 //fflush(stdout);
}
#endif


/*
	Go over every resource and check if they are marked at "MEMENTRY_STATE_LOAD_ME".
	Load them in memory and mark them are MEMENTRY_STATE_LOADED
*/
void Resource::loadMarkedAsNeeded() 
{
 while (1) 
 {
  MemEntry *me = NULL;

  // get resource with max rankNum
  unsigned char maxNum = 0; //JJ uint8_t maxNum = 0;
  unsigned short int i = _numMemList; //JJ uint16_t i = _numMemList;
  MemEntry *it = _memList;
  while (i--) 
  {
   if (it->state == MEMENTRY_STATE_LOAD_ME && maxNum <= it->rankNum) 
   {
 	maxNum = it->rankNum;
	me = it;
   }
   it++;
  }
		
  if (me == NULL) 
  {
   break; // no entry found
  }

		
  // At this point the resource descriptor should be pointed to "me"
  // "That's what she said"

  unsigned char *loadDestination = NULL; //JJ uint8_t *loadDestination = NULL;
  if (me->type == RT_POLY_ANIM) 
  {
   loadDestination = _vidCurPtr;
  }
  else 
  {
   loadDestination = _scriptCurPtr;
   if (me->size > _vidBakPtr - _scriptCurPtr) 
   {
 	warning("Resource::load() not enough memory");
	me->state = MEMENTRY_STATE_NOT_NEEDED;
    continue;
   }
  }


  if (me->bankId == 0) 
  {
   warning("Resource::load() ec=0x%X (me->bankId == 0)", 0xF00);
   me->state = MEMENTRY_STATE_NOT_NEEDED;
  }
  else 
  {
   debug(DBG_BANK, "Resource::load() bufPos=%X size=%X type=%X pos=%X bankId=%X", loadDestination - _memPtrStart, me->packedSize, me->type, me->bankOffset, me->bankId);
   readBank(me, loadDestination);
   if(me->type == RT_POLY_ANIM) 
   {
    video->copyPage(_vidCurPtr);
	me->state = MEMENTRY_STATE_NOT_NEEDED;
   }
   else 
   {
 	me->bufPtr = loadDestination;
	me->state = MEMENTRY_STATE_LOADED;
	_scriptCurPtr += me->size;
   }
  }
 }
}

void Resource::invalidateRes()
{
 MemEntry *me = _memList;
 unsigned short int i = _numMemList; //JJ uint16_t i = _numMemList;
 while (i--)
 {
  if (me->type <= RT_POLY_ANIM || me->type > 6)
  {  // 6 WTF ?!?! ResType goes up to 5 !!
   me->state = MEMENTRY_STATE_NOT_NEEDED;
  }
  ++me;
 }
 _scriptCurPtr = _scriptBakPtr;
}

void Resource::invalidateAll()
{
 MemEntry *me = _memList;
 unsigned short int i = _numMemList; //JJ uint16_t i = _numMemList;
 while (i--) 
 {
  me->state = MEMENTRY_STATE_NOT_NEEDED;
  ++me;
 }
 _scriptCurPtr = _memPtrStart;
}

/* This method serves two purpose: 
    - Load parts in memory segments (palette,code,video1,video2)
	           or
    - Load a resource in memory

	This is decided based on the resourceId. If it does not match a mementry id it is supposed to 
	be a part id. */
//JJ void Resource::loadPartsOrMemoryEntry(uint16_t resourceId)
void Resource::loadPartsOrMemoryEntry(unsigned short int resourceId)
{
 if (resourceId > _numMemList) 
 {
  requestedNextPart = resourceId;
 }
 else 
 {
  MemEntry *me = &_memList[resourceId];

  if (me->state == MEMENTRY_STATE_NOT_NEEDED) 
  {
   me->state = MEMENTRY_STATE_LOAD_ME;
   loadMarkedAsNeeded();
  }
 }
}

/* Protection screen and cinematic don't need the player and enemies polygon data
   so _memList[video2Index] is never loaded for those parts of the game. When 
   needed (for action phrases) _memList[video2Index] is always loaded with 0x11 
   (as seen in memListParts). */
//JJ void Resource::setupPart(uint16_t partId) 
void Resource::setupPart(unsigned short int partId) 
{
 if (partId == currentPartId)
 {
  return;
 }

 if (partId < GAME_PART_FIRST || partId > GAME_PART_LAST)
 {
  error("Resource::setupPart() ec=0x%X invalid partId", partId);
 }

 unsigned short int memListPartIndex = partId - GAME_PART_FIRST; //JJ uint16_t memListPartIndex = partId - GAME_PART_FIRST;

 unsigned char paletteIndex = memListParts[memListPartIndex][MEMLIST_PART_PALETTE]; //JJ uint8_t paletteIndex = memListParts[memListPartIndex][MEMLIST_PART_PALETTE];
 unsigned char codeIndex = memListParts[memListPartIndex][MEMLIST_PART_CODE];	//JJ uint8_t codeIndex    = memListParts[memListPartIndex][MEMLIST_PART_CODE];
 unsigned char videoCinematicIndex = memListParts[memListPartIndex][MEMLIST_PART_POLY_CINEMATIC]; //JJ uint8_t videoCinematicIndex  = memListParts[memListPartIndex][MEMLIST_PART_POLY_CINEMATIC];
 unsigned char video2Index = memListParts[memListPartIndex][MEMLIST_PART_VIDEO2]; //JJ uint8_t video2Index  = memListParts[memListPartIndex][MEMLIST_PART_VIDEO2];

 // Mark all resources as located on harddrive.
 invalidateAll();

 _memList[paletteIndex].state = MEMENTRY_STATE_LOAD_ME;
 _memList[codeIndex].state = MEMENTRY_STATE_LOAD_ME;
 _memList[videoCinematicIndex].state = MEMENTRY_STATE_LOAD_ME;

 // This is probably a cinematic or a non interactive part of the game.
 // Player and enemy polygons are not needed.
 if (video2Index != MEMLIST_PART_NONE)
 {
  _memList[video2Index].state = MEMENTRY_STATE_LOAD_ME;
 }
	

 loadMarkedAsNeeded();

 segPalettes = _memList[paletteIndex].bufPtr;
 segBytecode = _memList[codeIndex].bufPtr;
 segCinematic = _memList[videoCinematicIndex].bufPtr;


 // This is probably a cinematic or a non interactive part of the game.
 // Player and enemy polygons are not needed.
 if (video2Index != MEMLIST_PART_NONE)
 {
  _segVideo2 = _memList[video2Index].bufPtr;
 }
	
 debug(DBG_RES,"");
 debug(DBG_RES,"setupPart(%d)",partId-GAME_PART_FIRST);
 debug(DBG_RES,"Loaded resource %d (%s) in segPalettes.",paletteIndex,resTypeToString(_memList[paletteIndex].type));
 debug(DBG_RES,"Loaded resource %d (%s) in segBytecode.",codeIndex,resTypeToString(_memList[codeIndex].type));
 debug(DBG_RES,"Loaded resource %d (%s) in segCinematic.",videoCinematicIndex,resTypeToString(_memList[videoCinematicIndex].type));

 if (video2Index != MEMLIST_PART_NONE) 
 {
  debug(DBG_RES,"Loaded resource %d (%s) in _segVideo2.",video2Index,resTypeToString(_memList[video2Index].type));
 }

 currentPartId = partId;
	
 // _scriptCurPtr is changed in this->load();
 _scriptBakPtr = _scriptCurPtr;
}

void Resource::allocMemBlock()
{
 //JJ _memPtrStart = (unsigned char *)ps_malloc(MEM_BLOCK_SIZE); //JJ _memPtrStart = (uint8_t *)malloc(MEM_BLOCK_SIZE);
 _memPtrStart = gb_memory; //614400 bytes global memory engine

 _scriptBakPtr = _scriptCurPtr = _memPtrStart;
 _vidBakPtr = _vidCurPtr = _memPtrStart + MEM_BLOCK_SIZE - 0x800 * 16; //0x800 = 2048, so we have 32KB free for vidBack and vidCur
 _useSegVideo2 = false;
}

//JJ void Resource::freeMemBlock() 
//JJ {
//JJ  free(_memPtrStart);
//JJ }

//JJ pendiente de portar (no quiero ficheros ni librerias)
//void Resource::saveOrLoad(Serializer &ser) 
//{
// unsigned char loadedList[64]; //JJ uint8_t loadedList[64];
// if (ser._mode == Serializer::SM_SAVE) 
// {
//  memset(loadedList, 0, sizeof(loadedList));
//  unsigned char *p = loadedList; //JJ uint8_t *p = loadedList;
//  unsigned char *q = _memPtrStart; //JJ uint8_t *q = _memPtrStart;
//  while (1) 
//  {
//   MemEntry *it = _memList;
//   MemEntry *me = 0;
//   unsigned short int num = _numMemList; //JJ uint16_t num = _numMemList;
//   while (num--) 
//   {
//	if (it->state == MEMENTRY_STATE_LOADED && it->bufPtr == q) 
//    {
//	 me = it;
//	}
//	++it;
//   }
//   if (me == 0) 
//   {
// 	break;
//   }
//   else
//   {
// 	//JJ assert(p < loadedList + 64);
// 	if (!(p < loadedList + 64))
// 	{
//     Serial.printf("ERROR loadedList saveOrLoad\r\n");
//     //fflush(stdout);
//     break;
//    }
// 	
//	*p++ = me - _memList;
//	q += me->size;
//   }
//  }
// }
//
// Serializer::Entry entries[] = {
//  SE_ARRAY(loadedList, 64, Serializer::SES_INT8, VER(1)),
//  SE_INT(&currentPartId, Serializer::SES_INT16, VER(1)),
//  SE_PTR(&_scriptBakPtr, VER(1)),
//  SE_PTR(&_scriptCurPtr, VER(1)),
//  SE_PTR(&_vidBakPtr, VER(1)),
//  SE_PTR(&_vidCurPtr, VER(1)),
//  SE_INT(&_useSegVideo2, Serializer::SES_BOOL, VER(1)),
//  SE_PTR(&segPalettes, VER(1)),
//  SE_PTR(&segBytecode, VER(1)),
//  SE_PTR(&segCinematic, VER(1)),
//  SE_PTR(&_segVideo2, VER(1)),
//  SE_END()
// };
//
// ser.saveOrLoadEntries(entries);
// if (ser._mode == Serializer::SM_LOAD) 
// {
//  unsigned char *p = loadedList; //JJ uint8_t *p = loadedList;
//  unsigned char *q = _memPtrStart; //JJ uint8_t *q = _memPtrStart;
//  while (*p) 
//  {
//   MemEntry *me = &_memList[*p++];
//   readBank(me, q);
//   me->bufPtr = q;
//   me->state = MEMENTRY_STATE_LOADED;
//   q += me->size;
//  }
// }	
//}

