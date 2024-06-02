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

#ifndef __SFXPLAYER_H__
 #define __SFXPLAYER_H__

 #include "intern.h"

 struct SfxInstrument 
 {
  unsigned char *data; //JJ uint8_t *data;
  unsigned short int volume; //JJ uint16_t volume;
 };

 struct SfxModule 
 {
  const unsigned char *data; //JJ const uint8_t *data;
  unsigned short int curPos; //JJ uint16_t curPos;
  unsigned char curOrder; //JJ uint8_t curOrder;
  unsigned char numOrder; //JJ uint8_t numOrder;
  unsigned char orderTable[0x80]; //JJ uint8_t orderTable[0x80];
  SfxInstrument samples[15];
 };

 struct SfxPattern 
 {
  unsigned short int note_1; //JJ uint16_t note_1;
  unsigned short int note_2; //JJ uint16_t note_2;
  unsigned short int sampleStart; //JJ uint16_t sampleStart;
  unsigned char *sampleBuffer; //JJ uint8_t *sampleBuffer;
  unsigned short int sampleLen; //JJ uint16_t sampleLen;
  unsigned short int loopPos; //JJ uint16_t loopPos;
  unsigned char *loopData; //JJ uint8_t *loopData;
  unsigned short int loopLen; //JJ uint16_t loopLen;
  unsigned short int sampleVolume; //JJ uint16_t sampleVolume;
 };

 struct Mixer;
 struct Resource;
 //JJ struct Serializer; //pendiente de portar (no quiero ficheros ni librerias)
 struct System;

 struct SfxPlayer 
 {
  Mixer *mixer;
  Resource *res;
  System *sys;

  void *_mutex;
  int _timerId;
  unsigned short int _delay; //JJ uint16_t _delay;
  unsigned short int _resNum; //JJ uint16_t _resNum;
  SfxModule _sfxMod;
  short int *_markVar; //JJ int16_t *_markVar;

  SfxPlayer(Mixer *mix, Resource *res, System *stub);
  void init();
  void free();

  void setEventsDelay(unsigned short int delay); //JJ void setEventsDelay(uint16_t delay);
  void loadSfxModule(unsigned short int resNum, unsigned short int delay, unsigned char pos); //JJ void loadSfxModule(uint16_t resNum, uint16_t delay, uint8_t pos);
  void prepareInstruments(const unsigned char *p); //JJ void prepareInstruments(const uint8_t *p);
  void start();
  void stop();
  void handleEvents();
  void handlePattern(unsigned char channel, const unsigned char *patternData); //JJ void handlePattern(uint8_t channel, const uint8_t *patternData);

  static unsigned int eventsCallback(unsigned int interval, void *param); //JJ static uint32_t eventsCallback(uint32_t interval, void *param);

  //JJ void saveOrLoad(Serializer &ser); //pendiente de portar (no quiero ficheros ni librerias)
 };

#endif

