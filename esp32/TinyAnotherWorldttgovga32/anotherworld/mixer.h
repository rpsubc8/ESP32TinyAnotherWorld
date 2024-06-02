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

#ifndef __MIXER_H__
 #define __MIXER_H__

 #include "intern.h"

 struct MixerChunk 
 {
  const unsigned char *data; //JJ const uint8_t *data;
  unsigned short int len; //JJ uint16_t len;
  unsigned short int loopPos; //JJ uint16_t loopPos;
  unsigned short int loopLen; //JJ uint16_t loopLen;
 };

 struct MixerChannel 
 {
  unsigned char active; //JJ uint8_t active;
  unsigned char volume; //JJ uint8_t volume;
  MixerChunk chunk;
  unsigned int chunkPos; //JJ uint32_t chunkPos;
  unsigned int chunkInc; //JJ uint32_t chunkInc;
 };

 //struct Serializer; //pendiente de portar (no quiero ficheros ni librerias)
 struct System;

 #define AUDIO_NUM_CHANNELS 4

 struct Mixer 
 {
  void *_mutex;
  System *sys;

  // Since the virtal machine and SDL are running simultaneously in two different threads
  // any read or write to an elements of the sound channels MUST be synchronized with a 
  // mutex.
  MixerChannel _channels[AUDIO_NUM_CHANNELS];

  Mixer(System *stub);
  void init();
  void free();

  void playChannel(unsigned char channel, const MixerChunk *mc, unsigned short int freq, unsigned char volume); //JJ void playChannel(uint8_t channel, const MixerChunk *mc, uint16_t freq, uint8_t volume);
  void stopChannel(unsigned char channel); //JJ void stopChannel(uint8_t channel);
  void setChannelVolume(unsigned char channel, unsigned char volume); //JJ void setChannelVolume(uint8_t channel, uint8_t volume);
  void stopAll();
  void mix(signed char *buf, int len); //JJ void mix(int8_t *buf, int len);

  static void mixCallback(void *param, unsigned char *buf, int len); //JJ static void mixCallback(void *param, uint8_t *buf, int len);

  //JJ void saveOrLoad(Serializer &ser); //pendiente de portar (no quiero ficheros ni librerias)
 };

#endif

