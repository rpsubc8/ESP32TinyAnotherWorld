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

//#include <cstdarg> //No lo necesito
#include "util.h"
#include <Arduino.h>


//uint16_t g_debugMask=DBG_INFO; // DBG_VM | DBG_BANK | DBG_VIDEO | DBG_SER | DBG_SND;
unsigned short int g_debugMask= 0; //DBG_RES ; //JJ uint16_t g_debugMask= DBG_RES ;

//JJ void debug(uint16_t cm, const char *msg, ...) 
void debug(unsigned short int cm, const char *msg, ...)
{
 char buf[1024];
 if (cm & g_debugMask) 
 {
  va_list va;
  va_start(va, msg);
  vsprintf(buf, msg, va);
  va_end(va);
  Serial.printf("%s\r\n", buf);
  //fflush(stdout);
		
  //fprintf(gb_log,"%s\n", buf);
  //fflush(gb_log);
 }
}

void error(const char *msg, ...) 
{
 char buf[1024];
 va_list va;
 va_start(va, msg);
 vsprintf(buf, msg, va);
 va_end(va);
 //fprintf(stderr, "ERROR: %s!\n", buf);
 Serial.printf("ERROR: %s!\r\n", buf);
 //fflush(stdout);
 exit(-1);
}

void warning(const char *msg, ...) 
{
 char buf[1024];
 va_list va;
 va_start(va, msg);
 vsprintf(buf, msg, va);
 va_end(va);
 //fprintf(stderr, "WARNING: %s!\n", buf);
 Serial.printf("WARNING: %s!\r\n", buf);
 //fflush(stdout);
}

void string_lower(char *p) 
{
 for (; *p; ++p) 
 {
  if (*p >= 'A' && *p <= 'Z') 
  {
   *p += 'a' - 'A';
  }
 }
}

void string_upper(char *p) 
{
 for (; *p; ++p) 
 {
  if (*p >= 'a' && *p <= 'z') 
  {
   *p += 'A' - 'a';
  }
 }
}

