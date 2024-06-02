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

#ifndef __BANK_H__
 #define __BANK_H__

 #include "gbConfig.h"
 #include "main.h"

 #include "intern.h"

 struct MemEntry;

 struct UnpackContext
 {
  unsigned short int size;	//JJ uint16_t size;
  unsigned int crc;	//JJ uint32_t crc;
  unsigned int chk;  //JJ	uint32_t chk;
  int datasize; //JJ int32_t datasize;
 };

 struct Bank 
 {
  UnpackContext _unpCtx;
  const char *_dataDir;
  unsigned char *_iBuf, *_oBuf, *_startBuf; //JJ uint8_t *_iBuf, *_oBuf, *_startBuf;

  Bank(const char *dataDir);
  #ifdef use_lib_dataflash
   bool readFlash(const MemEntry *me, unsigned char *buf);
  #else 
   bool read(const MemEntry *me, unsigned char *buf); //JJ bool read(const MemEntry *me, uint8_t *buf);
  #endif 
  void decUnk1(unsigned char numChunks, unsigned char addCount); //JJ void decUnk1(uint8_t numChunks, uint8_t addCount);
  void decUnk2(unsigned char numChunks); //JJ void decUnk2(uint8_t numChunks);
  bool unpack();
  unsigned short int getCode(unsigned char numChunks); //JJ uint16_t getCode(uint8_t numChunks);
  bool nextChunk();
  bool rcr(bool CF);
 };

#endif
