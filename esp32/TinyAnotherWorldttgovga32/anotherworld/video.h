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

#ifndef __VIDEO_H__
 #define __VIDEO_H__

 #include "intern.h"

 struct StrEntry 
 {
  unsigned short int id; //JJ uint16_t id;
  const char *str;
 };

 //Esto esta muy poco optimizado

 #define MAX_POINTS 50
 struct Polygon 
 {
  //Lo quiero fuera. No objetos, separacion de datos y codigo
  //JJ enum {
  //JJ MAX_POINTS = 50
  //JJ};

  unsigned short int bbw, bbh; //JJ uint16_t bbw, bbh;
  unsigned char numPoints; //JJ uint8_t numPoints;
  Point points[MAX_POINTS];

  //JJ void readVertices(const unsigned char *p, unsigned short int zoom); //void readVertices(const uint8_t *p, uint16_t zoom); //No quiero objetos
 };

 void Polygon_readVertices(const unsigned char *p, unsigned short int zoom, struct Polygon *ptr); //void readVertices(const uint8_t *p, uint16_t zoom);

 struct Resource;
 //JJ struct Serializer; //pendiente de portar (no quiero ficheros ni librerias)
 struct System;

 // This is used to detect the end of  _stringsTableEng and _stringsTableDemo
 #define END_OF_STRING_DICTIONARY 0xFFFF 

 // Special value when no palette change is necessary
 #define NO_PALETTE_CHANGE_REQUESTED 0xFF 



 struct Video 
 {
  typedef void (Video::*drawLine)(short int x1, short int x2, unsigned char col); //JJ typedef void (Video::*drawLine)(int16_t x1, int16_t x2, uint8_t col);

  enum 
  {
   VID_PAGE_SIZE  = 320 * 200 / 2
  };

  static const unsigned char _font[]; //JJ static const uint8_t _font[];
  static const StrEntry _stringsTableEng[];
  static const StrEntry _stringsTableDemo[];

  Resource *res;
  System *sys;
	
  unsigned char paletteIdRequested, currentPaletteId; //JJ uint8_t paletteIdRequested, currentPaletteId;
  unsigned char *_pages[4]; //JJ uint8_t *_pages[4];

  // I am almost sure that:
  // _curPagePtr1 is the work buffer
  // _curPagePtr2 is the background buffer1
  // _curPagePtr3 is the background buffer2
  unsigned char *_curPagePtr1, *_curPagePtr2, *_curPagePtr3; //JJ uint8_t *_curPagePtr1, *_curPagePtr2, *_curPagePtr3;

  Polygon polygon;
  short int _hliney; //JJ int16_t _hliney;

  //Precomputer division lookup table
  unsigned short int _interpTable[0x400]; //JJ uint16_t _interpTable[0x400];

  Ptr _pData;
  unsigned char *_dataBuf; //JJ uint8_t *_dataBuf;

  Video(Resource *res, System *stub);
  void init();

  void setDataBuffer(unsigned char *dataBuf, unsigned short int offset); //JJ void setDataBuffer(uint8_t *dataBuf, uint16_t offset);
  void readAndDrawPolygon(unsigned char color, unsigned short int zoom, const Point &pt); //JJ void readAndDrawPolygon(uint8_t color, uint16_t zoom, const Point &pt);
  void fillPolygon(unsigned short int color, unsigned short int zoom, const Point &pt); //JJ void fillPolygon(uint16_t color, uint16_t zoom, const Point &pt);
  void readAndDrawPolygonHierarchy(unsigned short int zoom, const Point &pt); //JJ void readAndDrawPolygonHierarchy(uint16_t zoom, const Point &pt);
  int calcStep(const Point &p1, const Point &p2, unsigned short int &dy); //JJ int32_t calcStep(const Point &p1, const Point &p2, uint16_t &dy);

  void drawString(unsigned char color, unsigned short int x, unsigned short int y, unsigned short int strId); //JJ void drawString(uint8_t color, uint16_t x, uint16_t y, uint16_t strId);
  void drawChar(unsigned char c, unsigned short int x, unsigned short int y, unsigned char color, unsigned char *buf); //JJ void drawChar(uint8_t c, uint16_t x, uint16_t y, uint8_t color, uint8_t *buf);
  void drawPoint(unsigned char color, short int x, short int y); //JJ void drawPoint(uint8_t color, int16_t x, int16_t y);
  void drawLineBlend(short int x1, short int x2, unsigned char color); //JJ void drawLineBlend(int16_t x1, int16_t x2, uint8_t color);
  void drawLineN(short int x1, short int x2, unsigned char color); //JJ void drawLineN(int16_t x1, int16_t x2, uint8_t color);
  void drawLineP(short int x1, short int x2, unsigned char color); //JJ void drawLineP(int16_t x1, int16_t x2, uint8_t color);
  unsigned char *getPage(unsigned char page); //JJ uint8_t *getPage(uint8_t page);
  void changePagePtr1(unsigned char page); //JJ void changePagePtr1(uint8_t page);
  void fillPage(unsigned char page, unsigned char color); //JJ void fillPage(uint8_t page, uint8_t color);
  void copyPage(unsigned char src, unsigned char dst, short int vscroll); //JJ void copyPage(uint8_t src, uint8_t dst, int16_t vscroll);
  void copyPage(const unsigned char *src); //JJ void copyPage(const uint8_t *src);
  void changePal(unsigned char pal); //JJ void changePal(uint8_t pal);
  void updateDisplay(unsigned char page); //JJ void updateDisplay(uint8_t page);
	
  //JJ void saveOrLoad(Serializer &ser); //pendiente de portar (no quiero ficheros ni librerias)
 };

#endif

