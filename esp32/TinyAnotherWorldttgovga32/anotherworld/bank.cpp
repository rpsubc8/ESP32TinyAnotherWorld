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
#include "bank.h"
//JJ #include "file.h" //(no quiero ficheros ni librerias)
#include "resource.h"
#include <Arduino.h> //Para serial

#ifdef use_lib_dataflash
 #ifdef use_lib_mini_test
  //Necesitamos banco 1 proteccion
  //banco 1,2 intro
  //13,12,6 para nivel piscina 
  #include "databank01.h"
  //#include "databank02.h"
  //#include "databank06.h"
  //#include "databank0c.h"
  //#include "databank0d.h"
 #else
  #include "databank01.h"
  #include "databank02.h"
  #include "databank03.h"
  #include "databank04.h"
  #include "databank05.h"
  #include "databank06.h"
  #include "databank07.h"
  #include "databank08.h"
  #include "databank09.h"
  #include "databank0a.h"
  #include "databank0b.h"
  #include "databank0c.h"
  #include "databank0d.h"
 #endif 
#endif 


Bank::Bank(const char *dataDir)
	: _dataDir(dataDir) {
}

#ifdef use_lib_dataflash
 bool Bank::readFlash(const MemEntry *me, unsigned char *buf)
 {
  bool ret = false;
  unsigned int offset= me->bankOffset;
  const unsigned char *ptr;
  unsigned char banco;
 
  banco= me->bankId;
  //Serial.printf("Bank::readFlash %d\n",banco);
  //fflush(stdout);

 #ifdef use_lib_mini_test   
  switch (banco)
  {
   case 1: ptr= gb_bank01_bin; break;
   //case 2: ptr= gb_bank02_bin; break;
   //case 6: ptr= gb_bank06_bin; break;
   //case 0x0C: ptr= gb_bank0c_bin; break;
   //case 0x0D: ptr= gb_bank0d_bin; break;   
   default: 
    Serial.printf("banco no reconocido %d\r\n",banco);
    //fflush(stdout);
    break;   
  } 
 #else
  switch (banco)
  {
   case 1: ptr= gb_bank01_bin; break;
   case 2: ptr= gb_bank02_bin; break;
   case 3: ptr= gb_bank03_bin; break;
   case 4: ptr= gb_bank04_bin; break;
   case 5: ptr= gb_bank05_bin; break;
   case 6: ptr= gb_bank06_bin; break;   
   case 7: ptr= gb_bank07_bin; break;   
   case 8: ptr= gb_bank08_bin; break;
   case 9: ptr= gb_bank09_bin; break;
   case 0x0A: ptr= gb_bank0a_bin; break;   
   case 0x0B: ptr= gb_bank0b_bin; break;   
   case 0x0C: ptr= gb_bank0c_bin; break;   
   case 0x0D: ptr= gb_bank0d_bin; break;   
   default: 
    Serial.printf("banco no reconocido %d\n",banco);
    //fflush(stdout);
    break;   
  }
 #endif 
 
  if (me->packedSize == me->size) 
  {
   memcpy(buf, &ptr[offset], me->packedSize);
   ret = true;
  }
  else 
  {
   memcpy(buf, &ptr[offset], me->packedSize);
   _startBuf = buf;
   _iBuf = buf + me->packedSize - 4;
   ret = unpack();
  }
 
  return ret;
 }
#else
//JJ bool Bank::read(const MemEntry *me, uint8_t *buf) 
bool Bank::read(const MemEntry *me, unsigned char *buf)
{
 bool ret = false;
 char bankName[10];
 sprintf(bankName, "bank%02x", me->bankId);
 File f;

 if (!f.open(bankName, _dataDir))
 {
  error("Bank::read() unable to open '%s'", bankName);
 }

 Serial.printf("Bank::read %s\r\n",bankName);
 //fflush(stdout);
	
 f.seek(me->bankOffset);

 // Depending if the resource is packed or not we
 // can read directly or unpack it.
 if (me->packedSize == me->size) 
 {
  f.read(buf, me->packedSize);
  ret = true;
 }
 else 
 {
  f.read(buf, me->packedSize);
  _startBuf = buf;
  _iBuf = buf + me->packedSize - 4;
  ret = unpack();
 }
	
 return ret;
}
#endif

//JJ void Bank::decUnk1(uint8_t numChunks, uint8_t addCount) 
void Bank::decUnk1(unsigned char numChunks, unsigned char addCount) 
{
 unsigned short int count = getCode(numChunks) + addCount + 1; //JJ uint16_t count = getCode(numChunks) + addCount + 1;
 debug(DBG_BANK, "Bank::decUnk1(%d, %d) count=%d", numChunks, addCount, count);
 _unpCtx.datasize -= count;
 while (count--) 
 {
  //JJ assert(_oBuf >= _iBuf && _oBuf >= _startBuf);
  if (!(_oBuf >= _iBuf && _oBuf >= _startBuf))
  {
   Serial.printf("ERROR decUnk1\r\n");
   //fflush(stdout);
   break;
  }
  
  *_oBuf = (uint8_t)getCode(8);
  --_oBuf;
 }
}

/*
   Note from fab: This look like run-length encoding.
*/
//JJ void Bank::decUnk2(uint8_t numChunks) 
void Bank::decUnk2(unsigned char numChunks) 
{
 unsigned short int i = getCode(numChunks); //JJ uint16_t i = getCode(numChunks);
 unsigned short int count = _unpCtx.size + 1; //JJ uint16_t count = _unpCtx.size + 1;
 debug(DBG_BANK, "Bank::decUnk2(%d) i=%d count=%d", numChunks, i, count);
 _unpCtx.datasize -= count;
 while (count--) 
 {
  //JJ assert(_oBuf >= _iBuf && _oBuf >= _startBuf);
  if (!(_oBuf >= _iBuf && _oBuf >= _startBuf))
  {
   Serial.printf("ERROR decUnk2\r\n");
   //fflush(stdout);            
   break;
  }
  
  *_oBuf = *(_oBuf + i);
  --_oBuf;
 }
}

/*
	Most resource in the banks are compacted.
*/
bool Bank::unpack() 
{
 _unpCtx.size = 0;
 _unpCtx.datasize = READ_BE_UINT32(_iBuf); _iBuf -= 4;
 _oBuf = _startBuf + _unpCtx.datasize - 1;
 _unpCtx.crc = READ_BE_UINT32(_iBuf); _iBuf -= 4;
 _unpCtx.chk = READ_BE_UINT32(_iBuf); _iBuf -= 4;
 _unpCtx.crc ^= _unpCtx.chk;
 do 
 {
  if (!nextChunk()) 
  {
   _unpCtx.size = 1;
   if (!nextChunk()) 
   {
 	decUnk1(3, 0);
   } 
   else 
   {
	decUnk2(8);
   }
  }
  else 
  {
   unsigned short int c = getCode(2); //JJ uint16_t c = getCode(2);
   if (c == 3) 
   {
	decUnk1(8, 8);
   }
   else 
   {
	if (c < 2) 
    {
	 _unpCtx.size = c + 2;
	 decUnk2(c + 9);
	}
    else 
    {
	 _unpCtx.size = getCode(8);
	 decUnk2(12);
	}
   }
  }
 } while (_unpCtx.datasize > 0);
 return (_unpCtx.crc == 0);
}

///JJ uint16_t Bank::getCode(uint8_t numChunks) 
unsigned short int Bank::getCode(unsigned char numChunks) 
{
 unsigned short int c = 0; //JJ uint16_t c = 0;
 while (numChunks--) 
 {
  c <<= 1;
  if (nextChunk()) 
  {
   c |= 1;
  }			
 }
 return c;
}

bool Bank::nextChunk() 
{
 bool CF = rcr(false);
 if (_unpCtx.chk == 0) 
 {
  //JJ assert(_iBuf >= _startBuf);
  if (!(_iBuf >= _startBuf))
  {   
   Serial.printf("ERROR nextChunk\r\n");
   //fflush(stdout);
   return false;
  }
   
  _unpCtx.chk = READ_BE_UINT32(_iBuf); _iBuf -= 4;
  _unpCtx.crc ^= _unpCtx.chk;
  CF = rcr(true);
 }
 return CF;
}

bool Bank::rcr(bool CF) 
{
 bool rCF = (_unpCtx.chk & 1);
 _unpCtx.chk >>= 1;
 if (CF)
 {
  _unpCtx.chk |= 0x80000000;
 }
 return rCF;
}

