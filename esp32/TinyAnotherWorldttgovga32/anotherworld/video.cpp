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
#include "video.h"
#include "resource.h"
//JJ #include "serializer.h" //pendiente de portar (no quiero ficheros ni librerias)
#include "sys.h"
#include <Arduino.h>


//JJ void Polygon::readVertices(const uint8_t *p, uint16_t zoom)  //No quiero objetos
/*
void Polygon::readVertices(const unsigned char *p, unsigned short int zoom) 
{         
	bbw = (*p++) * zoom / 64;
	bbh = (*p++) * zoom / 64;
	numPoints = *p++;
	assert((numPoints & 1) == 0 && numPoints < MAX_POINTS);

	//Read all points, directly from bytecode segment
	for (int i = 0; i < numPoints; ++i) {
		Point *pt = &points[i];
		pt->x = (*p++) * zoom / 64;
		pt->y = (*p++) * zoom / 64;
	}	
}
*/

void Polygon_readVertices(const unsigned char *p, unsigned short int zoom, struct Polygon * ptr) 
{         
	ptr->bbw = (*p++) * zoom / 64;
	ptr->bbh = (*p++) * zoom / 64;
	ptr->numPoints = *p++;
	//JJ assert((ptr->numPoints & 1) == 0 && ptr->numPoints < MAX_POINTS);
	if (!((ptr->numPoints & 1) == 0 && ptr->numPoints < MAX_POINTS))
	{
     Serial.printf("ERROR Polygon_readVertices\r\n");
     //fflush(stdout);
     return;
    }
 
	//Read all points, directly from bytecode segment
	for (int i = 0; i < ptr->numPoints; ++i) {
		Point *pt = &(ptr->points[i]);
		pt->x = (*p++) * zoom / 64;
		pt->y = (*p++) * zoom / 64;
	}	
}

Video::Video(Resource *resParameter, System *stub) 
	: res(resParameter), sys(stub) {
}

void Video::init() 
{
 paletteIdRequested = NO_PALETTE_CHANGE_REQUESTED;

 //Usar memoria psram para video
 //JJ unsigned char* tmp = (unsigned char *)ps_malloc(4 * VID_PAGE_SIZE); //JJ uint8_t* tmp = (uint8_t *)malloc(4 * VID_PAGE_SIZE);
 //JJ memset(tmp,0,4 * VID_PAGE_SIZE);
	
 //JJ for (int i = 0; i < 4; ++i) 
 //JJ {
 //JJ  _pages[i] = tmp + i * VID_PAGE_SIZE;
 //JJ }

 //Usar memoria sram para video
 for (unsigned char i = 0; i < 4; ++i) 
 {
  //_pages[i] = (unsigned char *)malloc(32000);
  _pages[i] = gb_vram[i];
  memset(_pages[i],0,32000);
 }



 _curPagePtr3 = getPage(1);
 _curPagePtr2 = getPage(2);

 changePagePtr1(0xFE);

 _interpTable[0] = 0x4000;

 for (int i = 1; i < 0x400; ++i) 
 {
  _interpTable[i] = 0x4000 / i;
 }
}

/*
	This
*/
//JJ void Video::setDataBuffer(uint8_t *dataBuf, uint16_t offset) 
void Video::setDataBuffer(unsigned char *dataBuf, unsigned short int offset)
{
 _dataBuf = dataBuf;
 _pData.pc = dataBuf + offset;
}


/*  A shape can be given in two different ways:

	 - A list of screenspace vertices.
	 - A list of objectspace vertices, based on a delta from the first vertex.

	 This is a recursive function. */
//JJ void Video::readAndDrawPolygon(uint8_t color, uint16_t zoom, const Point &pt)
void Video::readAndDrawPolygon(unsigned char color, unsigned short int zoom, const Point &pt)
{
	unsigned char i = _pData.fetchByte(); //JJ uint8_t i = _pData.fetchByte();

	//This is 
	if (i >= 0xC0) {	// 0xc0 = 192

		// WTF ?
		if (color & 0x80) {   //0x80 = 128 (1000 0000)
			color = i & 0x3F; //0x3F =  63 (0011 1111)   
		}

		// pc is misleading here since we are not reading bytecode but only
		// vertices informations.
		Polygon_readVertices(_pData.pc, zoom, &polygon); //JJ polygon.readVertices(_pData.pc, zoom);

		fillPolygon(color, zoom, pt);



	} else {
		i &= 0x3F;  //0x3F = 63
		if (i == 1) {
			warning("Video::readAndDrawPolygon() ec=0x%X (i != 2)", 0xF80);
		} else if (i == 2) {
			readAndDrawPolygonHierarchy(zoom, pt);

		} else {
			warning("Video::readAndDrawPolygon() ec=0x%X (i != 2)", 0xFBB);
		}
	}



}

//JJ void Video::fillPolygon(uint16_t color, uint16_t zoom, const Point &pt) 
void Video::fillPolygon(unsigned short int color, unsigned short int zoom, const Point &pt) 
{

	if (polygon.bbw == 0 && polygon.bbh == 1 && polygon.numPoints == 4) {
		drawPoint(color, pt.x, pt.y);

		return;
	}
	
	short int x1 = pt.x - polygon.bbw / 2; //JJ int16_t x1 = pt.x - polygon.bbw / 2;
	short int x2 = pt.x + polygon.bbw / 2; //JJ int16_t x2 = pt.x + polygon.bbw / 2;
	short int y1 = pt.y - polygon.bbh / 2; //JJ int16_t y1 = pt.y - polygon.bbh / 2;
	short int y2 = pt.y + polygon.bbh / 2; //JJ int16_t y2 = pt.y + polygon.bbh / 2;

	if (x1 > 319 || x2 < 0 || y1 > 199 || y2 < 0)
	{
	 return;
    }

	_hliney = y1;
	
	unsigned short int i, j; //JJ uint16_t i, j;
	i = 0;
	j = polygon.numPoints - 1;
	
	x2 = polygon.points[i].x + x1;
	x1 = polygon.points[j].x + x1;

	++i;
	--j;

	drawLine drawFct;
	if (color < 0x10) {
		drawFct = &Video::drawLineN;
	} else if (color > 0x10) {
		drawFct = &Video::drawLineP;
	} else {
		drawFct = &Video::drawLineBlend;
	}

	unsigned int cpt1 = x1 << 16; //JJ uint32_t cpt1 = x1 << 16;
	unsigned int cpt2 = x2 << 16; //JJ uint32_t cpt2 = x2 << 16;

	while (1) 
    {
		polygon.numPoints -= 2;
		if (polygon.numPoints == 0) 
        {
			break;
		}
		unsigned short int h; //JJ uint16_t h;
		int step1 = calcStep(polygon.points[j + 1], polygon.points[j], h); //JJ int32_t step1 = calcStep(polygon.points[j + 1], polygon.points[j], h);
		int step2 = calcStep(polygon.points[i - 1], polygon.points[i], h); //JJ int32_t step2 = calcStep(polygon.points[i - 1], polygon.points[i], h);

		++i;
		--j;

		cpt1 = (cpt1 & 0xFFFF0000) | 0x7FFF;
		cpt2 = (cpt2 & 0xFFFF0000) | 0x8000;

		if (h == 0) {	
			cpt1 += step1;
			cpt2 += step2;
		} else {
			for (; h != 0; --h) {
				if (_hliney >= 0) {
					x1 = cpt1 >> 16;
					x2 = cpt2 >> 16;
					if (x1 <= 319 && x2 >= 0) {
						if (x1 < 0) x1 = 0;
						if (x2 > 319) x2 = 319;
						(this->*drawFct)(x1, x2, color);
					}
				}
				cpt1 += step1;
				cpt2 += step2;
				++_hliney;					
				if (_hliney > 199)
                {
                 return;
                }
			}
		}
	}





}

/*
    What is read from the bytecode is not a pure screnspace polygon but a polygonspace polygon.

*/
//JJ void Video::readAndDrawPolygonHierarchy(uint16_t zoom, const Point &pgc) 
void Video::readAndDrawPolygonHierarchy(unsigned short int zoom, const Point &pgc) 
{

	Point pt(pgc);
	pt.x -= _pData.fetchByte() * zoom / 64;
	pt.y -= _pData.fetchByte() * zoom / 64;

	short int childs = _pData.fetchByte(); //JJ int16_t childs = _pData.fetchByte();
	//JJ debug(DBG_VIDEO, "Video::readAndDrawPolygonHierarchy childs=%d", childs);

	for ( ; childs >= 0; --childs) 
    {
		unsigned short int off = _pData.fetchWord(); //JJ uint16_t off = _pData.fetchWord();

		Point po(pt);
		po.x += _pData.fetchByte() * zoom / 64;
		po.y += _pData.fetchByte() * zoom / 64;

		unsigned short int color = 0xFF; //JJ uint16_t color = 0xFF;
		unsigned short int _bp = off; //JJ uint16_t _bp = off;
		off &= 0x7FFF;

		if (_bp & 0x8000) {
			color = *_pData.pc & 0x7F;
			_pData.pc += 2;
		}

		unsigned char *bak = _pData.pc; //JJ uint8_t *bak = _pData.pc;
		_pData.pc = _dataBuf + off * 2;


		readAndDrawPolygon(color, zoom, po);


		_pData.pc = bak;
	}

	
}

//JJ int32_t Video::calcStep(const Point &p1, const Point &p2, uint16_t &dy) 
int Video::calcStep(const Point &p1, const Point &p2, unsigned short int &dy) 
{
	dy = p2.y - p1.y;
	return (p2.x - p1.x) * _interpTable[dy] * 4;
}

//JJ void Video::drawString(uint8_t color, uint16_t x, uint16_t y, uint16_t stringId) 
void Video::drawString(unsigned char color, unsigned short int x, unsigned short int y, unsigned short int stringId)
{
	const StrEntry *se = _stringsTableEng;

	//Search for the location where the string is located.
	while (se->id != END_OF_STRING_DICTIONARY && se->id != stringId)
	{
	 ++se;
    }
	
	//JJ debug(DBG_VIDEO, "drawString(%d, %d, %d, '%s')", color, x, y, se->str);

	//Not found
	if (se->id == END_OF_STRING_DICTIONARY)
	{
	 return;
    }
	

    //Used if the string contains a return carriage.
	unsigned short int xOrigin = x; //JJ uint16_t xOrigin = x;
	int len = strlen(se->str);
	for (int i = 0; i < len; ++i) {

		if (se->str[i] == '\n') {
			y += 8;
			x = xOrigin;
			continue;
		} 
		
		drawChar(se->str[i], x, y, color, _curPagePtr1);
		x++;
		
	}
}

//JJ void Video::drawChar(uint8_t character, uint16_t x, uint16_t y, uint8_t color, uint8_t *buf) 
void Video::drawChar(unsigned char character, unsigned short int x, unsigned short int y, unsigned char color, unsigned char *buf)
{
	if (x <= 39 && y <= 192) 
    {		
		const unsigned char *ft = _font + (character - ' ') * 8; //JJ const uint8_t *ft = _font + (character - ' ') * 8;

		unsigned char *p = buf + x * 4 + y * 160; //JJ uint8_t *p = buf + x * 4 + y * 160;

		for (int j = 0; j < 8; ++j) 
        {
			unsigned char ch = *(ft + j); //JJ uint8_t ch = *(ft + j);
			for (int i = 0; i < 4; ++i) 
            {
				unsigned char b = *(p + i); //JJ uint8_t b = *(p + i);
				unsigned char cmask = 0xFF; //JJ uint8_t cmask = 0xFF;
				unsigned char colb = 0; //JJ uint8_t colb = 0;
				if (ch & 0x80) {
					colb |= color << 4;
					cmask &= 0x0F;
				}
				ch <<= 1;
				if (ch & 0x80) {
					colb |= color;
					cmask &= 0xF0;
				}
				ch <<= 1;
				*(p + i) = (b & cmask) | colb;
			}
			p += 160;
		}
	}
}

//JJ void Video::drawPoint(uint8_t color, int16_t x, int16_t y) 
void Video::drawPoint(unsigned char color, short int x, short int y) 
{
	//JJ debug(DBG_VIDEO, "drawPoint(%d, %d, %d)", color, x, y);
	if (x >= 0 && x <= 319 && y >= 0 && y <= 199) 
    {
		unsigned short int off = y * 160 + x / 2; //JJ uint16_t off = y * 160 + x / 2;
	
		unsigned char cmasko, cmaskn; //JJ uint8_t cmasko, cmaskn;
		if (x & 1) {
			cmaskn = 0x0F;
			cmasko = 0xF0;
		} else {
			cmaskn = 0xF0;
			cmasko = 0x0F;
		}

		unsigned char colb = (color << 4) | color; //JJ uint8_t colb = (color << 4) | color;
		if (color == 0x10) {
			cmaskn &= 0x88;
			cmasko = ~cmaskn;
			colb = 0x88;		
		} else if (color == 0x11) {
			colb = *(_pages[0] + off);
		}
		unsigned char b = *(_curPagePtr1 + off); //JJ uint8_t b = *(_curPagePtr1 + off);
		*(_curPagePtr1 + off) = (b & cmasko) | (colb & cmaskn);
	}
}

/* Blend a line in the current framebuffer (_curPagePtr1)
*/
//JJ void Video::drawLineBlend(int16_t x1, int16_t x2, uint8_t color)
void Video::drawLineBlend(short int x1, short int x2, unsigned char color)
{
 //JJ debug(DBG_VIDEO, "drawLineBlend(%d, %d, %d)", x1, x2, color);
 short int xmax = MAX(x1, x2); //JJ int16_t xmax = MAX(x1, x2);
 short int xmin = MIN(x1, x2); //JJ int16_t xmin = MIN(x1, x2);
 unsigned char *p = _curPagePtr1 + _hliney * 160 + xmin / 2; //JJ uint8_t *p = _curPagePtr1 + _hliney * 160 + xmin / 2;

 unsigned short int w = xmax / 2 - xmin / 2 + 1; //JJ uint16_t w = xmax / 2 - xmin / 2 + 1;
 unsigned char cmaske = 0; //JJ uint8_t cmaske = 0;
 unsigned char cmasks = 0; //JJ uint8_t cmasks = 0;
 if (xmin & 1) 
 {
  --w;
  cmasks = 0xF7;
 }
 if (!(xmax & 1)) 
 {
  --w;
  cmaske = 0x7F;
 }

 if (cmasks != 0)
 {
  *p = (*p & cmasks) | 0x08;
  ++p;
 }
 while (w--)
 {
  *p = (*p & 0x77) | 0x88;
  ++p;
 }
 if (cmaske != 0)
 {
  *p = (*p & cmaske) | 0x80;
  ++p;
 }
}

//JJ void Video::drawLineN(int16_t x1, int16_t x2, uint8_t color) 
void Video::drawLineN(short int x1, short int x2, unsigned char color)
{
 //JJ debug(DBG_VIDEO, "drawLineN(%d, %d, %d)", x1, x2, color);
 short int xmax = MAX(x1, x2); //JJ int16_t xmax = MAX(x1, x2);
 short int xmin = MIN(x1, x2); //JJ int16_t xmin = MIN(x1, x2);
 unsigned char *p = _curPagePtr1 + _hliney * 160 + xmin / 2; //JJ uint8_t *p = _curPagePtr1 + _hliney * 160 + xmin / 2;

 unsigned short int w = xmax / 2 - xmin / 2 + 1; //JJ uint16_t w = xmax / 2 - xmin / 2 + 1;
 unsigned char cmaske = 0; //JJ uint8_t cmaske = 0;
 unsigned char cmasks = 0; //JJ uint8_t cmasks = 0;	
 if (xmin & 1) 
 {
  --w;
  cmasks = 0xF0;
 }
 if (!(xmax & 1)) 
 {
  --w;
  cmaske = 0x0F;
 }

 unsigned char colb = ((color & 0xF) << 4) | (color & 0xF); //JJ uint8_t colb = ((color & 0xF) << 4) | (color & 0xF);
 if (cmasks != 0) 
 {
  *p = (*p & cmasks) | (colb & 0x0F);
  ++p;
 }
 while (w--) 
 {
  *p++ = colb;
 }
 if (cmaske != 0) 
 {
  *p = (*p & cmaske) | (colb & 0xF0);
  ++p;		
 }	
}

//JJ void Video::drawLineP(int16_t x1, int16_t x2, uint8_t color) 
void Video::drawLineP(short int x1, short int x2, unsigned char color) 
{
 //JJ debug(DBG_VIDEO, "drawLineP(%d, %d, %d)", x1, x2, color);
 short int xmax = MAX(x1, x2); //JJ int16_t xmax = MAX(x1, x2);
 short int xmin = MIN(x1, x2); //JJ int16_t xmin = MIN(x1, x2);
 unsigned short int off = _hliney * 160 + xmin / 2; //JJ uint16_t off = _hliney * 160 + xmin / 2;
 unsigned char *p = _curPagePtr1 + off; //JJ uint8_t *p = _curPagePtr1 + off;
 unsigned char *q = _pages[0] + off; //JJ uint8_t *q = _pages[0] + off;

 unsigned char w = xmax / 2 - xmin / 2 + 1; //JJ uint8_t w = xmax / 2 - xmin / 2 + 1;
 unsigned char cmaske = 0; //JJ uint8_t cmaske = 0;
 unsigned char cmasks = 0; //JJ uint8_t cmasks = 0;	
 if (xmin & 1) 
 {
  --w;
  cmasks = 0xF0;
 }
 if (!(xmax & 1)) 
 {
  --w;
  cmaske = 0x0F;
 }

 if (cmasks != 0) 
 {
  *p = (*p & cmasks) | (*q & 0x0F);
  ++p;
  ++q;
 }
 while (w--) 
 {
  *p++ = *q++;			
 }
 if (cmaske != 0) 
 {
  *p = (*p & cmaske) | (*q & 0xF0);
  ++p;
  ++q;
 }
}

//JJ uint8_t *Video::getPage(uint8_t page) 
unsigned char *Video::getPage(unsigned char page) 
{
 unsigned char *p; //JJ uint8_t *p;
 if (page <= 3) 
 {
  p = _pages[page];
 } 
 else 
 {
  switch (page) 
  {
   case 0xFF:
 	p = _curPagePtr3;
	break;
   case 0xFE:
	p = _curPagePtr2;
	break;
   default:
	p = _pages[0]; // XXX check
	warning("Video::getPage() p != [0,1,2,3,0xFF,0xFE] == 0x%X", page);
	break;
  }
 }
 return p;
}



//JJ void Video::changePagePtr1(uint8_t pageID) 
void Video::changePagePtr1(unsigned char pageID) 
{
 //JJ debug(DBG_VIDEO, "Video::changePagePtr1(%d)", pageID);
 _curPagePtr1 = getPage(pageID);
}



//JJ void Video::fillPage(uint8_t pageId, uint8_t color) 
void Video::fillPage(unsigned char pageId, unsigned char color) 
{
 //JJ debug(DBG_VIDEO, "Video::fillPage(%d, %d)", pageId, color);
 unsigned char *p = getPage(pageId); //JJ uint8_t *p = getPage(pageId);

 // Since a palette indice is coded on 4 bits, we need to duplicate the
 // clearing color to the upper part of the byte.
 unsigned char c = (color << 4) | color; //JJ uint8_t c = (color << 4) | color;

 memset(p, c, VID_PAGE_SIZE);
}

/*  This opcode is used once the background of a scene has been drawn in one of the framebuffer:
	   it is copied in the current framebuffer at the start of a new frame in order to improve performances. */
//JJ void Video::copyPage(uint8_t srcPageId, uint8_t dstPageId, int16_t vscroll) 
void Video::copyPage(unsigned char srcPageId, unsigned char dstPageId, short int vscroll) 
{
 //JJ debug(DBG_VIDEO, "Video::copyPage(%d, %d)", srcPageId, dstPageId);

 if (srcPageId == dstPageId)
 {
  return;
 }

 unsigned char *p; //JJ uint8_t *p;
 unsigned char *q; //JJ uint8_t *q;

 if (srcPageId >= 0xFE || !((srcPageId &= 0xBF) & 0x80)) 
 {
  p = getPage(srcPageId);
  q = getPage(dstPageId);
  memcpy(q, p, VID_PAGE_SIZE);			
 }
 else
 {
  p = getPage(srcPageId & 3);
  q = getPage(dstPageId);
  if (vscroll >= -199 && vscroll <= 199) 
  {
   uint16_t h = 200;
   if (vscroll < 0) 
   {
    h += vscroll;
	p += -vscroll * 160;
   }
   else
   {
	h -= vscroll;
	q += vscroll * 160;
   }
   memcpy(q, p, h * 160);
  }
 }
}




//JJ void Video::copyPage(const uint8_t *src) 
void Video::copyPage(const unsigned char *src) 
{
 //JJ debug(DBG_VIDEO, "Video::copyPage()");
 unsigned char *dst = _pages[0]; //JJ uint8_t *dst = _pages[0];
 int h = 200;
 while (h--) 
 {
  int w = 40;
  while (w--) 
  {
   //JJ uint8_t p[] = {
   unsigned char p[] = {
    *(src + 8000 * 3),
    *(src + 8000 * 2),
    *(src + 8000 * 1),
    *(src + 8000 * 0)
   };
   for(int j = 0; j < 4; ++j) 
   {
    unsigned char acc = 0; //JJ uint8_t acc = 0;
    for (int i = 0; i < 8; ++i) 
    {
  	 acc <<= 1;
  	 acc |= (p[i & 3] & 0x80) ? 1 : 0;
	 p[i & 3] <<= 1;
    }
    *dst++ = acc;
   }
   ++src;
  }
 }
}

/*
Note: The palettes set used to be allocated on the stack but I moved it to
      the heap so I could dump the four framebuffer and follow how
	  frames are generated.
*/
//JJ void Video::changePal(uint8_t palNum) 
void Video::changePal(unsigned char palNum) 
{
 if (palNum >= 32)
 {
  return;
 }
	
 unsigned char *p = res->segPalettes + palNum * 32; //JJ uint8_t *p = res->segPalettes + palNum * 32; //colors are coded on 2bytes (565) for 16 colors = 32
 sys->setPalette(p);
 currentPaletteId = palNum;
}

//JJ void Video::updateDisplay(uint8_t pageId) 
void Video::updateDisplay(unsigned char pageId) 
{
 unsigned char *ptrTemp;
//JJ 	debug(DBG_VIDEO, "Video::updateDisplay(%d)", pageId);

 if (pageId != 0xFE) 
 {
  if (pageId == 0xFF) 
  {
   //JJ SWAP(_curPagePtr2, _curPagePtr3); //no tiene sentido algo tan simple
   ptrTemp= _curPagePtr2;
   _curPagePtr2= _curPagePtr3;
   _curPagePtr3= ptrTemp;		 
  }
  else 
  {
	_curPagePtr2 = getPage(pageId);
  }
 }

 //Check if we need to change the palette
 if (paletteIdRequested != NO_PALETTE_CHANGE_REQUESTED) 
 {
  changePal(paletteIdRequested);
  paletteIdRequested = NO_PALETTE_CHANGE_REQUESTED;
 }

 //Q: Why 160 ?
 //A: Because one byte gives two palette indices so
 //   we only need to move 320/2 per line.
 sys->updateDisplay(_curPagePtr2);
}


//JJ pendiente de portar (no quiero ficheros ni librerias)
//void Video::saveOrLoad(Serializer &ser) 
//{
// unsigned char mask = 0; //JJ uint8_t mask = 0;
// if (ser._mode == Serializer::SM_SAVE) 
// {
//  for (int i = 0; i < 4; ++i) 
//  {
//   if (_pages[i] == _curPagePtr1)
//   {
//	mask |= i << 4;
//   }
//   if (_pages[i] == _curPagePtr2)
//   {
//	mask |= i << 2;
//   }
//   if (_pages[i] == _curPagePtr3)
//   {
//	mask |= i << 0;
//   }
//  }		
// }
// Serializer::Entry entries[] = {
//  SE_INT(&currentPaletteId, Serializer::SES_INT8, VER(1)),
//  SE_INT(&paletteIdRequested, Serializer::SES_INT8, VER(1)),
//  SE_INT(&mask, Serializer::SES_INT8, VER(1)),
//  SE_ARRAY(_pages[0], Video::VID_PAGE_SIZE, Serializer::SES_INT8, VER(1)),
//  SE_ARRAY(_pages[1], Video::VID_PAGE_SIZE, Serializer::SES_INT8, VER(1)),
//  SE_ARRAY(_pages[2], Video::VID_PAGE_SIZE, Serializer::SES_INT8, VER(1)),
//  SE_ARRAY(_pages[3], Video::VID_PAGE_SIZE, Serializer::SES_INT8, VER(1)),
//  SE_END()
// };
// ser.saveOrLoadEntries(entries);
//
// if (ser._mode == Serializer::SM_LOAD) 
// {
//  _curPagePtr1 = _pages[(mask >> 4) & 0x3];
//  _curPagePtr2 = _pages[(mask >> 2) & 0x3];
//  _curPagePtr3 = _pages[(mask >> 0) & 0x3];
//  changePal(currentPaletteId);
// }
//}
