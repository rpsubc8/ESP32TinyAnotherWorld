#include "gbConfig.h"
#include "osd.h"
//#include "gbsdlfont6x8.h"
#include <string.h> //para strelen, revisar quitar
#include "gbGlobals.h"

#include "PS2Kbd.h"
#include <Arduino.h> //millis
#include "vga6bit.h"
#include "main.h"

unsigned char ESP32_WHITE=63;
unsigned char ESP32_BLACK=0;


//********************************************************************************************
#ifdef use_lib_keyboard_uart
/*
 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    if (gb_use_debug==1){ Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart); }
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     if (gb_use_debug==1){ Serial.printf("%02X ",gb_buf_uart[i]); }
    }
    if (gb_use_debug==1){ Serial.printf("\r\n"); }
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }
 */

 //***************************************************************************
 /*
 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44        
         default: break;
        }
       }
      }
      else
      {       
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC              
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      break;

     default: break;
    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
 */
#endif



//********************************************************************************************
void OSDClear()
{
 unsigned char a0;
 unsigned int aux;
 unsigned short int topeX32= gb_vga_ancho>>2;
 a0= gb_const_colorNormal[0];
 
 aux= (a0<<24)|(a0<<16)|(a0<<8)|a0;

 //for (unsigned short int y=0;y<200;y++)
 for (unsigned short int y=0;y<gb_vga_alto;y++)
 {
  //for (unsigned short int x=0;x<320;x++)
  //for (unsigned short int x=0;x<80;x++) //320 div 4
  for (unsigned short int x=0;x<topeX32;x++) //320 div 4
  {
   //gb_buffer_vga[y][x^2]= gb_const_colorNormal[0];
   gb_buffer_vga32[y][x]= aux;
  }
 }
 
 //bitluni_dump_vga();
}

//********************************************************************************************
void OSDLoadGALFlash(unsigned short int id)
{
 if (gb_use_debug==1){ Serial.printf("Process load OSDLoadGALFlash id:%d\r\n",id); }
 //LoadGALFlash(gb_list_gal_data[id]);
}

//********************************************************************************************
void OSDLoadGTPFlash(unsigned short int id)
{
/*   
 //char yn[2] = {'Y', '\0'};
 unsigned char warmstart = 0;

 //warmstart = (yn[0] == 'Y');
 //printf("warmstart %d\n",warmstart);
 //fflush(stdout);        
 
 //LoadGTPFlash(gb_list_gtp_data[id], gb_list_gtp_size[id]);
 
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("WARM START",((gb_language_en==1)?(char **)gb_bool_menu_en:(char **)gb_bool_menu_sp),0,max_gb_bool_menu);
 
 if (aSelNum==255)
 {
  gb_show_osd_main_menu= 0;
  return;
 }
 
 warmstart= (aSelNum==0)? 1: 0; 
 if (gb_use_debug==1){ Serial.printf("warmstart %d\r\n",warmstart); }
 //fflush(stdout);         
  
 if (warmstart==1)
 {
  //printf("Jump to basic warmstart ...\n");
  if (gb_use_debug==1){ Serial.printf("Jump to basic warmstart ...\r\n"); }
  //fflush(stdout);
  R.PC.W = 0x0317; // BASIC WARM START
 } 
 

 gb_cont_pul_vkeys=0;
 gb_cont_vkeys=0;
 gb_send_vkeys=1;
 */
}

//********************************************************************************************
void OSDprintChar(char car,unsigned short int x,unsigned short int y,unsigned char color,unsigned char backcolor,unsigned char isDouble,unsigned char isVertical)
{
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 unsigned int auxId = car << 3; //x8
 unsigned char aux;
 unsigned char auxBit,auxColor;
 unsigned short int xDest=0;
 unsigned short int yDest=0;
 if (isVertical==0)
 {//Caracter en horizontal
  for (unsigned char j=0;j<8;j++)
  {
   aux = gb_sdl_font_6x8[auxId + j];
   for (int i=0;i<8;i++)
   {
    auxColor= ((aux>>i) & 0x01);    
    //jj_fast_putpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
    gb_buffer_vga[(y+j)][(x+(6-i))^2]= (auxColor==1)?gb_const_colorNormal[color]:gb_const_colorNormal[backcolor];
   }
  }
 } //00001111
 else
 {//Caracter en vertical
  if (isDouble==1)
  {
   unsigned short int aux_j=0;
   for (unsigned char j=0;j<16;j++)
   {
    aux_j= j>>1; //DIV 2
    aux = gb_sdl_font_6x8[auxId + aux_j];
    for (int i=0;i<8;i++)
    {
     auxColor= ((aux>>i) & 0x01);
     xDest= x+j;
     yDest= y+i;
     //jj_fast_putpixel(surface,xDest,yDest,(auxColor==1)?color:backcolor);
     gb_buffer_vga[yDest][xDest^2]= (auxColor==1)?gb_const_colorNormal[color]:gb_const_colorNormal[backcolor];
    }                  
   }
  }
  else
  {
   for (unsigned char j=0;j<8;j++)
   {
    aux = gb_sdl_font_6x8[auxId + j];
    for (int i=0;i<8;i++)
    {
     auxColor= ((aux>>i) & 0x01);
     xDest= x+j;
     yDest= y+i;
     //jj_fast_putpixel(surface,xDest,yDest,(auxColor==1)?color:backcolor);
     gb_buffer_vga[yDest][xDest^2]= (auxColor==1)?gb_const_colorNormal[color]:gb_const_colorNormal[backcolor];
    }
   }//fin cuando es normal
  }     
 }     
}

//********************************************************************************************
void OSDprintTextFPS(char *cad,unsigned char x, unsigned char y, unsigned char color,unsigned char backcolor)
{//Coordenadas en 6x8
 unsigned short int xDest= ((unsigned short int)x)<<3; 
 unsigned short int yDest= ((unsigned short int)y)<<3;
 
 unsigned int auxId;
 unsigned char aux;
 unsigned char auxBit,auxColor;
 unsigned short int xDest2=0;
 unsigned short int yDest2=0; 
 //unsigned short int x2,y2;
 unsigned char aCol[2]= {gb_const_colorNormal[backcolor],gb_const_colorNormal[color]};
 
 int auxLen= strlen(cad);
 if (auxLen>50)
 {
  auxLen=50;
 }
 
 for (int i=0;i<auxLen;i++)
 {
//  OSDprintChar(cad[i],xDest,yDest,color,backcolor,0,1);
    
   auxId = cad[i] << 3; //x8
   //x2= xDest;
   //y2= yDest;
   for (unsigned char j=0;j<8;j++)
   {
    aux = gb_sdl_font_6x8[auxId + j];
    for (int k=0;k<7;k++)
    {
     auxColor= ((aux>>k) & 0x01);
     xDest2= xDest+j; //xDest2= x2+j;
     yDest2= yDest+k; //yDest2= y2+k;
     //jj_fast_putpixel(surface,xDest,yDest,(auxColor==1)?color:backcolor);
     //gb_buffer_vga[yDest2][xDest2]= (auxColor==1)?color:backcolor;
     //gb_buffer_vga[yDest2][xDest2]= aCol[auxColor];
     gb_buffer_vga[yDest2][xDest2^2]= aCol[auxColor];
    }
   }//fin cuando es normal  
  
  
  yDest-=6;
 } 
}

//********************************************************************************************
void OSDprintText(char *cad,unsigned char x, unsigned char y, unsigned char color,unsigned char backcolor,unsigned char isDouble, unsigned char isVertical)
{//Coordenadas en 8x8
 unsigned short int xDest= ((unsigned short int)x)<<3; 
 unsigned short int yDest= ((unsigned short int)y)<<3;
 
 int auxLen= strlen(cad);
 if ((auxLen>50)&&(isDouble==1))
 {
  auxLen=50;
 }
 
 for (int i=0;i<auxLen;i++)
 {
  OSDprintChar(cad[i],xDest,yDest,color,backcolor,isDouble,isVertical);
  if (isVertical==0)
  {
   xDest+=6; //xDest+=8;
  }
  else
  {
   yDest-=6;//yDest-=8;
  }  
 }
}

//***************************************
void OSDSubir(unsigned char idSel)
{//Le doy a la tecla de subir
 unsigned char idPag= idSel>>4; //idSel/16;
 if (idPag>0)
 {
  gb_osd_ventana_ini--;
  if (gb_osd_ventana_ini<0)
  {
   gb_osd_ventana_ini= 0;
  }
  gb_osd_ventana_fin= gb_osd_ventana_ini+15;
 }
 else
 {
  gb_osd_ventana_ini=0;
  gb_osd_ventana_fin= gb_osd_ventana_ini+15;
 }
}

//***********************************************************
void OSDBajar(unsigned char idSel)
{//Le doy a la tecla de bajar
 unsigned char idPag= idSel>>4;//idSel/16;
 if (idPag>0)
 {
  gb_osd_ventana_ini++;
  gb_osd_ventana_fin= gb_osd_ventana_ini+15;
 }
}

//********************************************************************************************
//void DrawNewOSDMenu(const char **ptr_cad, unsigned char idSel, unsigned char maximo)
void DrawNewOSDMenu(char **ptr_cad, unsigned char idSel, unsigned char maximo)
{//OSD 32x16  Muestra 16 filas
 unsigned char idForeCol= ESP32_WHITE;
 unsigned char idBGCol= ESP32_BLACK;
 
 unsigned short int x_text_ini=0;
 unsigned short int xTextUpDown=0;
 unsigned short int yTextUp=0;
 unsigned short int yTextDown=0;
 unsigned short int y_text_ini=0;

 if ((gb_vga_ancho==320)&&((gb_vga_alto==240)||(gb_vga_alto==200)))
 {
  x_text_ini= 5; //34;
  xTextUpDown= 2;
  yTextUp= 4; //32/8
  yTextDown= 19; //152/8
  y_text_ini= 36; //Luego div 8                                  
 }
 else
 {
  if ((gb_vga_ancho==360)&&(gb_vga_alto==200))
  {
   x_text_ini= 5+3; //34;
   xTextUpDown= 2+3;
   yTextUp= 4;
   yTextDown= 19;                                          
   y_text_ini= 36; //Luego div 8
  }
  else
  {
   if ((gb_vga_ancho==512)&&(gb_vga_alto==384))
   {
    x_text_ini= 5 + ((gb_use_lib_offset_x<<2)>>3);
    xTextUpDown= 2 + ((gb_use_lib_offset_x<<2)>>3);
    yTextUp= 4 + (gb_use_lib_offset_y>>3) + 1;
    yTextDown= 19 + (gb_use_lib_offset_y>>3); 
    y_text_ini= 36 + gb_use_lib_offset_y; //Luego div 8
   }     
  }
 }

 //unsigned short int y_text_ini= 36 + gb_use_lib_offset_y; //Luego div 8
 char auxCad[64];

 short int contRow=gb_osd_ventana_ini;
 
 unsigned char scr_up= 0;
 unsigned char scr_down= 0;


 scr_up= (gb_osd_ventana_ini>0)? 1:0;
 scr_down= (gb_osd_ventana_fin<(maximo-1))? 1:0;
 
 if (scr_up==1) { OSDprintText(">",xTextUpDown,yTextUp,ESP32_BLACK,ESP32_WHITE,1,1); }
 else { OSDprintText(" ",xTextUpDown,yTextUp,ESP32_BLACK,ESP32_WHITE,1,1); } 
 if (scr_down==1){ OSDprintText("<",xTextUpDown,yTextDown,ESP32_BLACK,ESP32_WHITE,1,1); }
 else { OSDprintText(" ",xTextUpDown,yTextDown,ESP32_BLACK,ESP32_WHITE,1,1); } 
 
 short int contRowDest=0;
 //unsigned int contRow=0;
 for (unsigned short int i=gb_osd_ventana_ini;i<=gb_osd_ventana_fin;i++)
 {
  idForeCol= (contRow==idSel)? ESP32_BLACK : ESP32_WHITE;
  idBGCol= (contRow==idSel)? ESP32_WHITE : ESP32_BLACK;     
  if (i<maximo)
  {
   strcpy(auxCad," ");
   strcat(auxCad,ptr_cad[contRow]);
   unsigned char len= strlen(auxCad);
   for (unsigned char j=len;j<44;j++)
   {
    auxCad[j]=' ';   
   }
   auxCad[44]='\0'; //Maximo 44 caracteres
   OSDprintText(auxCad,x_text_ini,(y_text_ini+(contRowDest<<3))>>3,idForeCol,idBGCol,0,0);
  }
  else
  {
   memset(auxCad,' ',44); //Lineas vacias sin rellenar
   auxCad[44]='\0';
   OSDprintText(auxCad,x_text_ini,(y_text_ini+(contRowDest<<3))>>3,ESP32_WHITE,ESP32_BLACK,0,0);            
  }
   
  contRowDest++;
  contRow++;
 } 

/*
 unsigned char idForeCol= ESP32_WHITE;
 unsigned char idBGCol= ESP32_BLACK;
 unsigned short int x_text_ini= 34;
 unsigned short int y_text_ini= 36;
 char auxCad[64];
 unsigned char idPag= (idSel/16); //Pagina
 unsigned char id_ini_window= (idPag>0)? (idPag+(idSel%16)): idPag;
 unsigned char id_end_window= id_ini_window+16;
 unsigned char contRow=id_ini_window;
 
 unsigned char scr_up= 0;
 unsigned char scr_down= 0;


 if (idPag==0)
 { 
  scr_down= (maximo>=16) ? 1: 0;
 }
 else
 { 
  scr_up= 1;
  scr_down= (idSel<(maximo-1)) ? 1: 0;
 }
  
 if (scr_up==1) { OSDprintText(">",15/8,42/8,ESP32_BLACK,ESP32_WHITE,1,1); }
 else { OSDprintText(" ",15/8,42/8,ESP32_BLACK,ESP32_WHITE,1,1); } 
 if (scr_down==1){ OSDprintText("<",15/8,152/8,ESP32_BLACK,ESP32_WHITE,1,1); }
 else { OSDprintText(" ",15/8,152/8,ESP32_BLACK,ESP32_WHITE,1,1); }

 for (unsigned char i=0;i<16;i++)
 {
  if (contRow < maximo)
  {
   idForeCol= (contRow==idSel)? ESP32_BLACK : ESP32_WHITE;
   idBGCol= (contRow==idSel)? ESP32_WHITE : ESP32_BLACK;
   //sprintf(auxCad,"%d %d ",i,contRow);
   strcpy(auxCad," ");
   strcat(auxCad,ptr_cad[contRow]);
   unsigned char len= strlen(auxCad);
   for (unsigned char j=len;j<34;j++)
   {
    auxCad[j]=' ';   
   }
   auxCad[32]='\0';
   OSDprintText(auxCad,x_text_ini/8,(y_text_ini+(i*8))/8,idForeCol,idBGCol,0,0);
  }
  else
  {
   memset(auxCad,' ',32);
   auxCad[32]='\0';
   OSDprintText(auxCad,x_text_ini/8,(y_text_ini+(i*8))/8,ESP32_WHITE,ESP32_BLACK,0,0);      
  }
  contRow++;
 }      
 */
}

//********************************************************************************************
void OSDdrawRectFill(unsigned short int x,unsigned short int y,unsigned short int aWidth, unsigned short int aHeight, unsigned char aColor)
{
 for (unsigned short int j=0;j<aHeight;j++)
 {
  for (unsigned short int i=0;i<aWidth;i++)
  {
   //jj_fast_putpixel(gb_osd_sdl_surface,(x+i),(y+j),aColor);
   gb_buffer_vga[(y+j)][(x+i)^2]= gb_const_colorNormal[aColor];
  }
 }
}



unsigned gb_aRand[16]={3,0,1,3,0,2,0,3,1,0,2,0,0,1,0,3};
unsigned char gb_contRand=0;
unsigned short int gb_line_shift=0;
unsigned char gb_cont_baja=0;
unsigned char gb_baja=1;

unsigned short int gb_alto=200;
unsigned short int gb_ancho=320;

static unsigned int g_seed=0;

//****************************************
void fast_srand(unsigned int seed)
{
 g_seed = seed;     
}

//****************************************
inline unsigned int fast_rand()
{
 g_seed = ((214013 * g_seed) + 2531011);
 return (g_seed>>16)&0x7FFF;
}


//*************************************************************************************
void drawTVeffect()
{    
 unsigned short int gb_alto= gb_vga_alto;
 unsigned short int gb_ancho= gb_vga_ancho;
 unsigned short int filtroY_0[2];
 unsigned short int filtroX_0[2];
 unsigned short int filtroY_1[2];
 unsigned short int filtroX_1[2];
 unsigned short int aux_alto;
 unsigned short int aux_ancho;
 unsigned short int desplaza_x=0;
 unsigned short int desplaza_y=0;


 if ((gb_vga_ancho==320)&&((gb_vga_alto==240)||(gb_vga_alto==200)))
 {
  filtroY_0[0]= 6; 
  filtroY_0[1]= 17;
  filtroX_0[0]= 12;
  filtroX_0[1]= 302;
  filtroY_1[0]= 32;
  filtroY_1[1]= 161;
  filtroX_1[0]= 12;
  filtroX_1[1]= 302;
  aux_alto= gb_alto;
  aux_ancho= gb_ancho;
 }
 else
 {
  if ((gb_vga_ancho==360)&&(gb_vga_alto==200))
  {
   filtroY_0[0]= 6; 
   filtroY_0[1]= 17;
   filtroX_0[0]= 12+(3*8);
   filtroX_0[1]= 302+(3*8);
   filtroY_1[0]= 32;
   filtroY_1[1]= 161;
   filtroX_1[0]= 12+(3*8);
   filtroX_1[1]= 302+(3*8);                                  
   aux_alto= gb_alto;
   aux_ancho= gb_ancho;
  }
  else
  {
   if ((gb_vga_ancho==512)&&(gb_vga_alto==384))
   {
    filtroY_0[0]= 6 -4; 
    filtroY_0[1]= 17 -4;
    filtroX_0[0]= 12;
    filtroX_0[1]= 302;
    filtroY_1[0]= 32; 
    filtroY_1[1]= 161;
    filtroX_1[0]= 12;
    filtroX_1[1]= 302;
    aux_alto= 200;
    aux_ancho= 320;
    desplaza_x= 96; //((512-320)DIV 2)
    desplaza_y= 92; //((384-200)DIV 2)
   }
  }
 }

 //000000
 //010101
 //101010
 //111111 
 unsigned char aSnow[4];
 if (gb_vga_8colors==1)
 {//azul en colores
  aSnow[0]=gb_const_colorNormal[0];
  aSnow[1]=gb_const_colorNormal[6];
  aSnow[2]=gb_const_colorNormal[4];  
  aSnow[3]=gb_const_colorNormal[7];  
 }
 else
 {//grises en 64 colores
  aSnow[0]=gb_const_colorNormal[0];
  aSnow[1]=gb_const_colorNormal[21];
  aSnow[2]=gb_const_colorNormal[42];  
  aSnow[3]=gb_const_colorNormal[63];     
 }

// unsigned char aSnow[4]={
//  /*
//  gb_const_colorNormal[0] //negro
//  ,gb_const_colorNormal[1] //rojo
//  ,gb_const_colorNormal[2] //verde
//  ,gb_const_colorNormal[3] //amarillo
//  //,gb_const_colorNormal[4] //azul
//  */
//
//  gb_const_colorNormal[0] //negro
//  ,gb_const_colorNormal[6] //cian
//  ,gb_const_colorNormal[4] //azul oscuro
//  ,gb_const_colorNormal[7] //blanco
// }; //unsigned char aSnow[4]={0,21,42,63};
 
 unsigned char aByte;
 unsigned char distancia;
 unsigned char shiftBit;

 unsigned char a0,a1,a2,a3;
 unsigned int a32;
 //unsigned char a[4];
 //unsigned char cont4pixels=0;
 //for (unsigned int y=0;y<200;y++)
 //for (unsigned int y=0;y<gb_alto;y++)
 for (unsigned int y=0;y<aux_alto;y++)
 {
  //for (unsigned int x=0;x<320;x++)  
  
  shiftBit=0;
  distancia= y&15; //y%16;
  if ((y>=gb_line_shift)&&(y<(gb_line_shift+16)))
  {   
   switch (distancia)
   {
    case 0: case 1: case 2: case 13: case 14: case 15: shiftBit= 1; break;
    case 3: case 4: case 5: case 10: case 11: case 12: shiftBit= 2; break;
    case 6: case 7: case 8: case 9: shiftBit= 3; break;     
   }
  }
  //printf("%d %d %d\n",y,distancia,shiftBit);
  
  
  //for (unsigned int x=0;x<gb_ancho;x++)
  for (unsigned int x=0;x<aux_ancho;x++)
  {
   //40,10,200,10   
//   if (
//       ((y>8)&&(y<(8+12)) && (x>16)&&(x<(16+280)))
//       ||
//       ((y>34)&&(y<(34+132)) && (x>16)&&(x<(16+280)))
//      )
//    continue;

   if (
       ((y>=filtroY_0[0])&&(y<=filtroY_0[1]) && (x>=filtroX_0[0])&&(x<=filtroX_0[1]))
       ||
       ((y>=filtroY_1[0])&&(y<=filtroY_1[1]) && (x>=filtroX_1[0])&&(x<=filtroX_1[1]))
      )
   {
    continue;
   }
   
   //gb_contRand= rand()%4;
   aByte= (shiftBit==0)? aSnow[gb_aRand[gb_contRand]] : aSnow[(gb_aRand[gb_contRand]-shiftBit) & 0x03];

   //jj_fast_putpixel(gb_osd_sdl_surface,x,y,aByte);
   //gb_buffer_vga[y][x^2]= aByte;   
   gb_buffer_vga[y+desplaza_y][(x+desplaza_x)^2]= aByte;
   
   //a[cont4pixels]= aByte;
   //cont4pixels++;
   //if (cont4pixels>=3)
   //{
   // cont4pixels=0;
   // a0=a[0];
   // a1=a[1];
   // a2=a[2];
   // a3=a[3];
   // a32= a2 | (a3<<8) | (a0<<16) | (a1<<24); //ESP32
   // gb_buffer_vga32[y][x>>2]= a32;
   //}

   gb_contRand++;
   if (gb_contRand>15)
   {
    //gb_contRand= gb_contRand + rand()%16;//gb_aRand[gb_contRand];
    //gb_contRand= gb_contRand + fast_rand()%16;//gb_aRand[gb_contRand];
    gb_contRand= gb_contRand + fast_rand() & 0x0F;
    gb_contRand= gb_contRand & 0x0F;
   }
  } 
 }


 gb_cont_baja++;
 //if (gb_cont_baja>=60)
 if (gb_cont_baja>=240)
 {
  gb_baja= ((fast_rand() & 0x03)>0)? 1 : 0;
  gb_cont_baja=0;
 }
 //printf("gb_baja %d\n",gb_baja);
 
 gb_line_shift= (gb_baja==1)? (gb_line_shift+1) : (gb_line_shift-1);
 if (gb_line_shift >= (gb_alto-1))
 {
  gb_line_shift=0;
 }
 
 //printf("SNOW\n");
 //fflush(stdout); 
}



//********************************************************************************************
void WaitFreeKeys(unsigned char isEffect)
{//Espera a que se liberen todas las teclas

 unsigned char auxKey= 1;
 unsigned int timeIni= millis(); //revisar millis(); //SDL_GetTicks();
 unsigned int timeCur= timeIni;
   
 while (auxKey!=0)
 {
  auxKey=0;
  #ifdef use_lib_keyboard_uart
   keyboard_uart_poll();     
   //if (checkKey_uart(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;} else {auxKey= auxKey & 0xFE; }
   //if (checkKey_uart(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; } else {auxKey= auxKey & 0xFD; }   
   //if (checkKey_uart(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; } else {auxKey= auxKey & 0xFB; }   
   //if (checkKey_uart(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; } else {auxKey= auxKey & 0xF7; }
   //if (checkKey_uart(KEY_ENTER)==1){ auxKey= auxKey | 16; } else {auxKey= auxKey & 0xEF; }
   //if (checkKey_uart(KEY_ESC)==1){ auxKey= auxKey | 32; } else {auxKey= auxKey & 0xDF; }
   if (checkKey_uart(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;}
   if (checkKey_uart(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; }
   if (checkKey_uart(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; }
   if (checkKey_uart(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; }
   if (checkKey_uart(KEY_ENTER)==1){ auxKey= auxKey | 16; }
   if (checkKey_uart(KEY_ESC)==1){ auxKey= auxKey | 32; }
  #endif

  if (checkKey(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;}
  if (checkKey(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; }
  if (checkKey(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; }
  if (checkKey(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; }
  if (checkKey(KEY_ENTER)==1){ auxKey= auxKey | 16; }
  if (checkKey(KEY_ESC)==1){ auxKey= auxKey | 32; }


  //revisar
  //SDL_PollEvent (&gb_event);
  //Uint8 *keystates = SDL_GetKeyState(NULL);
  //
  //auxKey=0;
  //
  //auxKey= auxKey | keystates[SDLK_LEFT];
  //auxKey= auxKey | (keystates[SDLK_RIGHT]<<1);
  //auxKey= auxKey | (keystates[SDLK_UP]<<2);
  //auxKey= auxKey | (keystates[SDLK_DOWN]<<3);
  //auxKey= auxKey | ((keystates[SDLK_KP_ENTER]<<4)|(keystates[SDLK_RETURN]<<4));
  //auxKey= auxKey | (keystates[SDLK_ESCAPE]<<5);       
    
  timeCur= millis(); //revisar millis(); //SDL_GetTicks();
  if ((timeCur-timeIni)>=20)
  {
   timeIni= timeCur;
   if (isEffect==1)
   {
    drawTVeffect();
   }
  }
 }
}


//********************************************************************************************
//unsigned char ShowTinyMenu(char *cadTitle,const char **ptrValue,unsigned char idSel,unsigned char aMax)
unsigned char ShowTinyMenu(char *cadTitle,char **ptrValue,unsigned char idSel,unsigned char aMax)
{
//OSD 32x16
 unsigned char contKeyEq=0;
 unsigned char auxKey=0;
 unsigned char auxKey_before=0;
 unsigned char aReturn=0;
 unsigned char salir=0;
 unsigned char auxChange=1;

 gb_osd_ventana_ini= (idSel<16)? 0: (idSel-15);//idSel;
 gb_osd_ventana_fin= gb_osd_ventana_ini+15; 
 
 unsigned short int xRectTitle= 0;
 unsigned short int yRectTitle= 0;
 unsigned short int xTextTitle= 0;
 unsigned short int yTextTitle= 0;

 unsigned short int xRectLeftMenu= 0;
 unsigned short int yRectLeftMenu= 0;
 unsigned short int xTitleLeftMenu= 0;
 unsigned short int yTitleLeftMenu= 0;

 //Seccion Titulo
 if ((gb_vga_ancho==320)&&((gb_vga_alto==240)||(gb_vga_alto==200)))
 {
  xRectTitle= 12;
  yRectTitle= 6;
  xTextTitle= 6;
  yTextTitle= 1;
                  
  xRectLeftMenu= 12;
  yRectLeftMenu= 32;
  xTitleLeftMenu= 2;
  yTitleLeftMenu= 16;
 }
 else
 {
  if ((gb_vga_ancho==360)&&(gb_vga_alto==200))
  {
   xRectTitle= 12+(3*8);    
   yRectTitle= 6;    
   xTextTitle= 6+3; 
   yTextTitle= 1;    
                  
   xRectLeftMenu= 12+(3*8); 
   yRectLeftMenu= 32; 
   xTitleLeftMenu= 2+3;
   yTitleLeftMenu= 16;                                              
  }
  else
  {
   if ((gb_vga_ancho==512)&&(gb_vga_alto==384))
   {
    xRectTitle= 12 + (gb_use_lib_offset_x<<2);    
    yRectTitle= 6 + gb_use_lib_offset_y - 4;
    xTextTitle= 6 + ((gb_use_lib_offset_x<<2)>>3); 
    yTextTitle= 1 + (gb_use_lib_offset_y>>3);
                  
    xRectLeftMenu= 12 + (gb_use_lib_offset_x<<2); 
    yRectLeftMenu= 32 + gb_use_lib_offset_y; 
    xTitleLeftMenu= 2 + ((gb_use_lib_offset_x<<2)>>3);
    yTitleLeftMenu= 16 + (gb_use_lib_offset_y>>3);
   }
  }
 }

 //OSDdrawRectFill(4,6,2+264+16+1+8,12,ESP32_WHITE); //rectangulo Titulo
 //OSDprintText("ESP32 GALAKSIJA(ackerman)",50/8,10/8,ESP32_BLACK,ESP32_WHITE,0,0); //Titulo          
 OSDdrawRectFill(xRectTitle,yRectTitle,291,12,ESP32_WHITE); //rectangulo Titulo
 OSDprintText("ESP32 ANOTHER WORLD (port by ackerman)",xTextTitle,yTextTitle,ESP32_BLACK,ESP32_WHITE,0,0); //Titulo
 
 //Titulo lateral izquierdo menu selector
 //OSDdrawRectFill(4,32,26,130,ESP32_WHITE);  
 //OSDprintText(cadTitle,15/8,132/8,ESP32_BLACK,ESP32_WHITE,1,1);
 OSDdrawRectFill(xRectLeftMenu,yRectLeftMenu,26,130,ESP32_WHITE);   //OSDdrawRectFill(4,32,26,130,ESP32_WHITE);  
 OSDprintText(cadTitle,xTitleLeftMenu,yTitleLeftMenu,ESP32_BLACK,ESP32_WHITE,1,1);
 
 DrawNewOSDMenu(ptrValue, idSel, aMax);
 
  
 //bitluni_dump_vga();
// SDL_Flip(screen); 
 
 
 
 unsigned int timeKeyboard= millis(); //revisar millis(); //SDL_GetTicks();
 unsigned int timeKeyboard_before= timeKeyboard;
 unsigned int time_cur= millis(); //revisar millis(); //SDL_GetTicks();
 unsigned int time_prev= time_cur;
 while (salir == 0)
 {
//  delay(20); //revisar delay(20); //SDL_Delay(20);
  time_cur= millis(); //revisar millis(); //SDL_GetTicks();
  if ((time_cur-time_prev)<19)
  {   
   continue;
  }
  else
  {
   time_prev= time_cur;
  }  
  //unsigned int segundos= (time_cur/1000)%60;
  //unsigned int min= ((time_cur/1000)/60)%60;
  //unsigned int hour= ((time_cur/1000)/3600)%24;
  
  if (auxChange==1)
  {
   auxChange= 0;
   DrawNewOSDMenu(ptrValue, idSel, aMax);
  } 
  //bitluni_dump_vga();
  //SDL_Flip(screen);
  
  drawTVeffect();

  auxKey=0;
  #ifdef use_lib_keyboard_uart
   keyboard_uart_poll();   
   //if (checkKey_uart(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;} else {auxKey= auxKey & 0xFE; }
   //if (checkKey_uart(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; } else {auxKey= auxKey & 0xFD; }   
   //if (checkKey_uart(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; } else {auxKey= auxKey & 0xFB; }   
   //if (checkKey_uart(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; } else {auxKey= auxKey & 0xF7; }
   //if (checkKey_uart(KEY_ENTER)==1){ auxKey= auxKey | 16; } else {auxKey= auxKey & 0xEF; }
   //if (checkKey_uart(KEY_ESC)==1){ auxKey= auxKey | 32; } else {auxKey= auxKey & 0xDF; }
   if (checkKey_uart(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;}
   if (checkKey_uart(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; }
   if (checkKey_uart(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; }
   if (checkKey_uart(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; }
   if (checkKey_uart(KEY_ENTER)==1){ auxKey= auxKey | 16; }
   if (checkKey_uart(KEY_ESC)==1){ auxKey= auxKey | 32; }
  #endif

  if (checkKey(KEY_CURSOR_LEFT)==1){auxKey= auxKey | 1;}
  if (checkKey(KEY_CURSOR_RIGHT)==1){auxKey= auxKey | 2; }
  if (checkKey(KEY_CURSOR_UP)==1){auxKey= auxKey | 4; }
  if (checkKey(KEY_CURSOR_DOWN)==1){auxKey= auxKey | 8; }
  if (checkKey(KEY_ENTER)==1){ auxKey= auxKey | 16; }
  if (checkKey(KEY_ESC)==1){ auxKey= auxKey | 32; }


  //revisar  
  //SDL_PollEvent (&gb_event);
  //Uint8 *keystates = SDL_GetKeyState(NULL);
  //auxKey= 0;
  //
  //auxKey= auxKey | keystates[SDLK_LEFT];
  //auxKey= auxKey | (keystates[SDLK_RIGHT]<<1);
  //auxKey= auxKey | (keystates[SDLK_UP]<<2);
  //auxKey= auxKey | (keystates[SDLK_DOWN]<<3);
  //auxKey= auxKey | ((keystates[SDLK_KP_ENTER]<<4)|(keystates[SDLK_RETURN]<<4));
  //auxKey= auxKey | (keystates[SDLK_ESCAPE]<<5);
  
    
  if (auxKey_before != auxKey)
  {
   if ((auxKey & 0x04)== 0x04) { if (idSel>0) {idSel--; OSDSubir(idSel); } }//UP
   if ((auxKey & 0x08)== 0x08) { if (idSel<(aMax-1)) {idSel++; OSDBajar(idSel); } }//DOWN
   if ((auxKey & 0x10)== 0x10) { salir= 1; aReturn= idSel; } //ENTER
   if ((auxKey & 0x20)== 0x20) { salir=1; aReturn= 255; } //ESC
                 
   auxKey_before= auxKey;
   contKeyEq=0;

   auxChange= 1;
  }
  else
  {
   contKeyEq++;
   if (contKeyEq>20)
   {
    if ((auxKey & 0x04)== 0x04) { if (idSel>0){ idSel--; OSDSubir(idSel); } }//UP
    if ((auxKey & 0x08)== 0x08) { if (idSel<(aMax-1)){ idSel++; OSDBajar(idSel); } }//DOWN

    if (auxKey!=0)
    {
     auxChange= 1;
    }
   }
  }           
 }
 
 WaitFreeKeys(1); //Espera a que se liberen todas las teclas

 //gb_show_osd_main_menu= 0;
 return aReturn; 
}


//***********************************
void ShowTinyMainMenu()
{
 unsigned char aSelNum;
 if(gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("  MAIN MENU", (char **)gb_main_menu_en, 0 ,max_gb_main_menu);                       
 }
 else
 {
  aSelNum = ShowTinyMenu("MENU INICIO", (char **)gb_main_menu_sp, 0 ,max_gb_main_menu);                            
 }
 
 if (aSelNum==255)
 {
  gb_show_osd_main_menu= 0;
  return;
 }
                  
 switch (aSelNum)
 {
  case 0: gb_id_menu_cur= id_menu_load_level; break;
  case 1: gb_id_menu_cur= id_menu_protection; break;
  case 2: gb_id_menu_cur= id_menu_language; break;
  case 3: gb_id_menu_cur= id_menu_video_options; break;
  case 4: gb_id_menu_cur= id_menu_keyb_options; break;
  case 5: gb_id_menu_cur= id_menu_cpu_options; break;
  case 6: gb_id_menu_cur= id_menu_osd; break;
  case 7: gb_id_menu_cur= id_menu_debug; break;
  case 8: gb_id_menu_cur= id_menu_help; break;
  case 9: gb_id_menu_cur= id_menu_reset; break;
  case 10: gb_id_menu_cur= id_menu_sound_options; break;
  case 11: gb_id_menu_cur= id_menu_ram; break;
  case 12: gb_id_menu_cur= id_menu_about; break;
  default: break;
  
  /*        
  case 2: gb_id_menu_cur= id_menu_sound_options; break;    
  
  case 7: gb_id_menu_cur= id_menu_help; break;
  case 8: gb_id_menu_cur= id_menu_reset; break;
  case 9: gb_id_menu_cur= id_menu_about; break;
  */
 }

}

//********************************************************************************************
void ShowTinyLoadLevelMenu()
{
 unsigned char aSelNum;

 aSelNum = ShowTinyMenu("LOAD LEVEL ",(char **)gb_loadlevel_menu,0,max_gb_loadlevel_menu);
 
 if (aSelNum==255)
 {
  //gb_show_osd_main_menu= 0;
  gb_id_menu_cur= id_menu_main;
  gb_show_osd_main_menu= 0;
  return;
 }

 switch (aSelNum)
 {
  case 0: gb_use_game_part= 0; gb_use_send_game_part= 1; break;
  case 1: gb_use_game_part= 1; gb_use_send_game_part= 1; break;
  case 2: gb_use_game_part= 2; gb_use_send_game_part= 1; break;
  case 3: gb_use_game_part= 3; gb_use_send_game_part= 1; break;
  case 4: gb_use_game_part= 4; gb_use_send_game_part= 1; break;
  case 5: gb_use_game_part= 5; gb_use_send_game_part= 1; break;
  case 6: gb_use_game_part= 6; gb_use_send_game_part= 1; break;
  case 7: gb_use_game_part= 7; gb_use_send_game_part= 1; break;
  case 8: gb_use_game_part= 8; gb_use_send_game_part= 1; break;
  case 9: gb_use_game_part= 9; gb_use_send_game_part= 1; break;
  case 10: gb_use_game_part= 10; gb_use_send_game_part= 1; break;
  case 11: gb_use_game_part= 11; gb_use_send_game_part= 1; break;
 }

 gb_show_osd_main_menu= 0;
}

//********************************************************************************************
void ShowTinyProtectionMenu()
{
 //unsigned char aSelNum;
 gb_show_osd_main_menu=0;          
}

//********************************************************************************************
void ShowTinyLanguageMenu()
{
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("Language", (char **)gb_language_menu_en, 0 ,max_gb_language_menu);                       
 }
 else
 {
  aSelNum = ShowTinyMenu("Idioma", (char **)gb_language_menu_sp, 1, max_gb_language_menu);
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior principal
  //gb_show_osd_main_menu= 0;
  gb_id_menu_cur= id_menu_main;
  return; 
 }
 switch (aSelNum)
 {
  case 0: gb_language_en= 1; break;
  case 1: gb_language_en= 0; break;
  default: break;
 }     
}

//********************************************************************************************
void ShowTinyVideoOptionsMenu()
{
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("   VIDEO", (char **)gb_video_options_menu_en, 0 ,max_gb_video_options_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("   VIDEO", (char **)gb_video_options_menu_sp, 0 ,max_gb_video_options_menu);
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior principal
  //gb_show_osd_main_menu= 0;
  gb_id_menu_cur= id_menu_main;
  return; 
 }
 switch (aSelNum)
 {
  case 0: gb_id_menu_cur= id_menu_video_mode; break;
  case 1: gb_id_menu_cur= id_menu_video_poll; break;
  case 2: gb_id_menu_cur= id_menu_video_filter; break;  
  default: break;
 } 
}


//********************************************************************************************
void OSDUpdateAnchoAltoVGA(unsigned char modeVideo, unsigned char is8colors)
{   
 gb_vga_8colors= is8colors;
     
 switch(modeVideo)
 {
  case video_mode_360x200x70hz_bitluni: case video_mode_360x200x70hz_bitluni_apll_fix:
   gb_vga_ancho= 360;
   gb_vga_alto= 200;
   gb_use_lib_offset_x= 5;
   gb_use_lib_offset_y= 0;
   //gb_use_lib_video= (modeVideo==video_mode_360x200x70hz_bitluni)?8:9;
   gb_use_lib_width= 360;
   gb_use_lib_height= 200;
   //gb_vga_yOffset= 2;
   //gb_vga_alto_caracter= 12;
   break;
  
  case video_mode_vga320x200x70hz_bitluni: case video_mode_vga320x200x70hz_fabgl: case video_mode_vga320x200x70hz_bitluni_apll_fix:
   gb_vga_ancho= 320;
   gb_vga_alto= 200;
   gb_use_lib_offset_x= 0;
   gb_use_lib_offset_y= 0;
   //gb_use_lib_video= (modeVideo==video_mode_vga320x200x70hz_bitluni)?10:((modeVideo==video_mode_vga320x200x70hz_fabgl)?11:12);
   gb_use_lib_width= 320;
   gb_use_lib_height= 200;   
   //gb_vga_yOffset= 2;
   //gb_vga_alto_caracter= 12;
   break;
   
  case video_mode_vga320x240x60hz_bitluni: case video_mode_vga320x240x60hz_fabgl: case video_mode_vga320x240x60hz_bitluni_apll_fix:
   gb_vga_ancho= 320;
   gb_vga_alto= 240; 
   gb_use_lib_offset_x= 0;
   gb_use_lib_offset_y= 20;
   //gb_use_lib_video= (modeVideo==video_mode_vga320x240x60hz_bitluni)?13:((modeVideo==video_mode_vga320x240x60hz_fabgl)?14:15);
   gb_use_lib_width= 320;
   gb_use_lib_height= 240;   
   //gb_vga_yOffset= 16;
   //gb_vga_alto_caracter= 13;
   break;

  case video_mode_vga512x384x60hz_fabgl:
   gb_vga_ancho= 512;
   gb_vga_alto= 384;
   gb_use_lib_offset_x= 24;
   gb_use_lib_offset_y= 92;
   //gb_use_lib_video= 16;
   gb_use_lib_width= 512;
   gb_use_lib_height= 384; 
   break;  
   
  default: break;
 }

 gb_use_lib_video= modeVideo;
 gb_vga_videomode_cur_menu= modeVideo;
 
 //gb_vga_display_offset_left=((gb_vga_ancho-256)/2);
 
 
 //0 "360x200 bitluni(3bpp)",
 //1 "360x200 bitluni apll(3bpp)",
 //2 "320x200 bitluni(3bpp)",
 //3 "320x200 fabgl(3bpp)",
 //4 "320x200 bitluni apll(3bpp)",
 //5 "320x240x60hz bitluni(3bpp)",
 //6 "320x240x60hz fabgl(3bpp)",
 //7 "320x240x60hz bitluni apll(3bpp)",
 
 //8 "360x200 bitluni(6bpp)",
 //9 "360x200 bitluni apll(6bpp)",
 //10 "320x200 bitluni(6bpp)",
 //11 "320x200 fabgl(6bpp)",
 //12 "320x200 bitluni apll(6bpp)",
 //13 "320x240x60hz bitluni(6bpp)",
 //14 "320x240x60hz fabgl(6bpp)",
 //15 "320x240x60hz bitluni apll(6bpp)" 
 
 /*
 switch (modeVideo)
 {
  case video_mode_360x200x70hz_bitluni: gb_vga_videomode_cur_menu= 0; break;
  case video_mode_360x200x70hz_bitluni_apll_fix: gb_vga_videomode_cur_menu= 1; break;  

  case video_mode_vga320x200x70hz_bitluni: gb_vga_videomode_cur_menu= 2; break;  
  case video_mode_vga320x200x70hz_fabgl: gb_vga_videomode_cur_menu= 3; break;  
  case video_mode_vga320x200x70hz_bitluni_apll_fix: gb_vga_videomode_cur_menu= 4; break;        
  
  case video_mode_vga320x240x60hz_bitluni: gb_vga_videomode_cur_menu= 5; break;    
  case video_mode_vga320x240x60hz_fabgl: gb_vga_videomode_cur_menu= 6; break;    
  case video_mode_vga320x240x60hz_bitluni_apll_fix: gb_vga_videomode_cur_menu= 7; break;      

  case video_mode_vga512x384x60hz_fabgl: gb_vga_videomode_cur_menu= 8; break;
  
  default:break;
 }
 */
 
 if (gb_use_debug==1){ Serial.printf("OSDUpdateAnchoAltoVGA mode:%d ancho:%d alto:%d id:%d 8colors:%d\r\n",modeVideo,gb_vga_ancho,gb_vga_alto,gb_vga_videomode_cur_menu,gb_vga_8colors); }
 //fflush(stdout);
 
 //gb_show_osd_main_menu=0;          
}

//********************************************************************************************
void ShowTinyVideoModeMenu()
{
 unsigned char aSelNum;
 unsigned char is8colors=0;
 unsigned char usepllcteforce=0;
 unsigned char usecustompll=0;
 unsigned int p0=0;
 unsigned int p1=0;
 unsigned int p2=0;
 unsigned int p3=0; 

 //static const unsigned char pin_config8[] = {  
 // PIN_RED_HIGH,
 // PIN_GREEN_HIGH,  
 // PIN_BLUE_HIGH,
 // 255,
 // 255,
 // 255,
 // PIN_HSYNC,
 // PIN_VSYNC
 //};  
 //static const unsigned char pin_config64[] = {  
 // PIN_RED_LOW,
 // PIN_RED_HIGH,
 // PIN_GREEN_LOW,
 // PIN_GREEN_HIGH,
 // PIN_BLUE_LOW,
 // PIN_BLUE_HIGH,
 // PIN_HSYNC,
 // PIN_VSYNC
 //};

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu(" VIDEO MODE", (char **)gb_video_mode_menu, gb_vga_videomode_cur_menu ,max_gb_video_mode_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("MODOS VIDEO", (char **)gb_video_mode_menu, gb_vga_videomode_cur_menu ,max_gb_video_mode_menu);
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior principal
  //gb_show_osd_main_menu= 0;
  gb_id_menu_cur= id_menu_video_options;
  return; 
 }     
 
 
 if (gb_use_debug==1){ Serial.printf("video aSelNum:%d\r\n",aSelNum); }
 
 switch (aSelNum)
 {
  //case 0: gb_vga_videomode_cur= 0; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; break; //360x200x70hz bitluni(3bpp)
  //case 1: gb_vga_videomode_cur= 1; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; break; //360x200x70hz bitluni apll(3bpp)
  //case 2: gb_vga_videomode_cur= 2; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; break; //320x200x70hz bitluni(3bpp)
  //case 3: gb_vga_videomode_cur= 3; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; break; //320x200x70hz fabgl(3bpp)
  //case 4: gb_vga_videomode_cur= 4; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; break; //320x200x70hz bitluni apll(3bpp)
  //case 5: gb_vga_videomode_cur= 5; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; break; //320x240x60hz bitluni(3bpp)
  //case 6: gb_vga_videomode_cur= 6; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; break; //320x240x60hz fabgl(3bpp)
  //case 7: gb_vga_videomode_cur= 7; is8colors=1; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; break; //320x240x60hz bitluni apll(3bpp)

  case 0:gb_vga_videomode_cur= 0; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=0; break;
  case 1:gb_vga_videomode_cur= 1; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_360x200; usepllcteforce=0; usecustompll=1; break;
  case 2:gb_vga_videomode_cur= 2; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=0; break;
  case 3:gb_vga_videomode_cur= 3; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=1; p0=0x00AE; p1=0x00CF; p2=0x0004; p3=0x0005; usecustompll=0; break;
  case 4:gb_vga_videomode_cur= 4; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x200; usepllcteforce=0; usecustompll=1; break;
  case 5:gb_vga_videomode_cur= 5; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; break;
  case 6:gb_vga_videomode_cur= 6; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=1; p0=0x000A; p1=0x0057; p2=0x0007; p3=0x0007; usecustompll=0; break;
  case 7:gb_vga_videomode_cur= 7; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=1; break;
  
  case 8:gb_vga_videomode_cur= 8; is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_512x384fabgl; usepllcteforce=1; p0=0x0000; p1=0x00C0; p2=0x0005; p3=0x0001; usecustompll=0; break; //512x384fabgl

  default: break;
 } 

 //OSDClearPantallaReal(); //borro primero la pantalla anterior 
 OSDClear();

 OSDUpdateAnchoAltoVGA(gb_vga_videomode_cur, is8colors);


 //Set Modo video BEGIN
 SetVideoInterrupt(0);
 delay(100);

 vga_free();
 delay(100);

 FreeInterrupt();
 delay(100);

 #ifdef use_lib_video4buffers_psram
 #else
  if (aSelNum==8)
  {//Si elijo modo 512x384 que requiere PSRAM, libero SRAM y fuerzo PSRAM
   ForcePSRAMVideo();
  }
 #endif 

 if (gb_use_debug==1){ Serial.printf("Trace - vga_init\r\n"); }
 //vga_init(pin_config,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3,usecustompll);
 vga_init((is8colors==1)?pin_config8:pin_config64,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3,usecustompll,is8colors, gb_vga_videomode_cur); 
 if (gb_use_debug==1){ Serial.printf("usepllcteforce:%d usecustompll:%d\r\n",usepllcteforce,usecustompll); }
 if (gb_use_debug==1){ Serial.printf("Trace - SetVideoInterrupt\r\n"); }
 //SetVideoInterrupt(1);    

 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
 PrepareColorsBitluniVGA(); //Llamar despues de tener gb_sync_bits
 for(unsigned char i=0;i<16;i++)
 {  
  gb_paleta16[i]= gb_const_colorNormal[(gb_paleta16_id[i])]; //color directo
 }//Necesito gb_sync_bits con la paleta del juego
 OSDClear();

 //Set Modo video END
 gb_ramfree_setupEnd= ESP.getFreeHeap();
 #ifdef use_lib_log_serial 
  if (gb_use_debug==1){ Serial.printf("RAM FREE %d\r\n", gb_ramfree_setupEnd); }
 #endif
 

 //PreparaPaleta();
 gb_show_osd_main_menu=0;  
}

//********************************************************************************************
void ShowTinyVideoPollMenu()
{
/*   
 unsigned char aSelNum;
 unsigned char idPollms=0;
 switch (gb_use_vga_poll_ms)
 {
  case 0: idPollms=0; break;
  case 20: idPollms=1; break;
  case 30: idPollms=2; break;
  case 40: idPollms=3; break;
  case 50: idPollms=4; break;
 }

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu(" VIDEO POLL", (char **)gb_video_poll_menu_en,idPollms,max_gb_video_poll_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("VIDEO SONDEO", (char **)gb_video_poll_menu_sp,idPollms,max_gb_video_poll_menu);
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior Video Options
  gb_id_menu_cur= id_menu_video_options;
  return; 
 }
 switch (aSelNum)
 {
  case 0: gb_use_vga_poll_ms= 0; break;
  case 1: gb_use_vga_poll_ms= 20; break;
  case 2: gb_use_vga_poll_ms= 30; break;
  case 3: gb_use_vga_poll_ms= 40; break;
  case 4: gb_use_vga_poll_ms= 50; break;
  default: gb_use_vga_poll_ms= 20; break;
 }          
*/

 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinyVideoFilterMenu()
{
/*   
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu(" VIDEO FILTER", (char **)gb_video_filter_menu_en,gb_use_video_filter,max_gb_video_filter_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("VIDEO FILTRO", (char **)gb_video_filter_menu_sp,gb_use_video_filter,max_gb_video_filter_menu);
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior Video Options
  gb_id_menu_cur= id_menu_video_options;
  return;
 }

 gb_use_video_filter= aSelNum;
 PrepareBWfilterVGA();

 //switch (aSelNum)
 //{
 // case 0: //blanco y negro
 //  gb_color_bw[0]= 0;
 //  gb_color_bw[1]= (gb_vga_8colors==1)?7:63;
 //  break;
 // case 1: //verde
 //  gb_color_bw[0]= 0;
 //  gb_color_bw[1]= (gb_vga_8colors==1)?2:12; //12 RGB en BGR es 1100 001100
 //  break;
 // case 2: //amarillo (3bpp) naranja(6bpp)
 //  gb_color_bw[0]= 0;
 //  gb_color_bw[1]= (gb_vga_8colors==1)?3:11; //56  //11 RGB en BGR es 56 1011 111000
 //  break;
 //}               
 
 gb_id_menu_cur= id_menu_main;
 gb_show_osd_main_menu=0;
 */
gb_show_osd_main_menu=0;          
}

//********************************************************************************************
void ShowTinyKeyboardPollMenu()
{
/*   
 unsigned char aSelNum;
 unsigned char idPollms=0;
 switch (gb_use_keyb_poll_ms)
 {
  case 0: idPollms=0; break;
  case 20: idPollms=1; break;
  case 30: idPollms=2; break;
  case 40: idPollms=3; break;
  case 50: idPollms=4; break;
 }


 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu(" KEYB POLL", (char **)gb_keyboard_poll_menu_en,idPollms,max_gb_keyboard_poll_menu);
 }
 else
 { 
  aSelNum = ShowTinyMenu("TECLADO POLL", (char **)gb_keyboard_poll_menu_sp,idPollms,max_gb_keyboard_poll_menu);     
 }
 
 if (aSelNum==255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return; 
 }
 switch (aSelNum)
 {
  case 0: gb_use_keyb_poll_ms= 10; break;
  case 1: gb_use_keyb_poll_ms= 20; break;
  case 2: gb_use_keyb_poll_ms= 30; break;
  case 3: gb_use_keyb_poll_ms= 40; break;
  case 4: gb_use_keyb_poll_ms= 50; break;
  default: break;
 }
*/
 gb_show_osd_main_menu=0;           
}

//****************************************************************************
//Menu velocidad emulador
void ShowTinyCPUMenu()
{   
 //0 (normal)
 //1 (fast)
 unsigned char aSelNum;
 unsigned char id= (gb_use_speed_fast==0)? 0: 1;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("  SPEED CPU", (char **)gb_cpu_menu_en ,id,max_gb_cpu_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("CPU VELOCIDAD", (char **)gb_cpu_menu_sp,id,max_gb_cpu_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 } 
 
 gb_use_speed_fast= (aSelNum==0)? 0: 1;

 
 gb_show_osd_main_menu=0;
 
}

//********************************************************************************************
void ShowTinyOSDMenu()
{
/*   
 unsigned char aSelNum; 
 unsigned char id= (gb_do_action_key_show_osd==1)? 0: 1;

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("  SHOW OSD", (char **)gb_bool_menu_en ,id,max_gb_bool_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("  VER OSD", (char **)gb_bool_menu_sp,id,max_gb_bool_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }

 gb_do_action_key_show_osd= (aSelNum==0)? 1: 0;
*/ 

 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinyDebugMenu()
{
 unsigned char aSelNum;
 unsigned char pos= (gb_use_debug==1)?0:1;

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("   DEBUG", (char **)gb_bool_menu_en ,pos,max_gb_bool_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("DEPURACION", (char **)gb_bool_menu_sp,pos,max_gb_bool_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }

 gb_use_debug= (aSelNum==0)?1:0;
 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("   RESET", (char **)gb_reset_menu_en ,0,max_gb_reset_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu(" REINICIO", (char **)gb_reset_menu_sp,0,max_gb_reset_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }

 if (aSelNum == 1)
 {
  vga_free();  
  ESP.restart();
 }
 else
 {
  gb_do_action_key_reset= 1;
  Serial.printf("Send reset soft\r\n");
 }

 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinySoundOptionsMenu()
{
 unsigned char aSelNum;

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("   SOUND ", (char **)gb_sound_options_menu ,0,max_gb_sound_options_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("  SONIDO ", (char **)gb_sound_options_menu,0,max_gb_sound_options_menu);
 }

 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }
 
 switch(aSelNum)
 {
  case 0: gb_id_menu_cur= id_menu_sound_volumen; break;
  case 1: gb_id_menu_cur= id_menu_sound_channel; break;
 }

 //gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinySoundChannelMenu()
{
 unsigned char aSelNum;
 unsigned char pos;

 pos= (gb_sound_mono_channel==1)?1:0;

 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu(" CHANNEL ", (char **)gb_sound_channel_menu,pos,max_gb_sound_channel_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("  CANAL  ", (char **)gb_sound_channel_menu,pos,max_gb_sound_channel_menu);
 }

 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }

 switch(aSelNum)
 {
  case 0: gb_sound_mono_channel= 0; break; //4 canales
  case 1: gb_sound_mono_channel= 1; break; //1 solo canal
 }

 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinySoundVolumenMenu()
{
   
 unsigned char aSelNum;
 unsigned char pos=0;
 
 switch (gb_snd_shift)
 {
  case 0: pos=0; break; //100 volumen
  case 1: pos=1; break; //75 volumen
  case 2: pos=2; break; //50 volumen
  case 3: pos=3; break; //25 volumen
  case 4: pos=4; break; //0 volumen
 }
 aSelNum = ShowTinyMenu("  SOUND   ", (char **)gb_sound_menu,pos,max_gb_sound_menu);
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }
 
 switch (aSelNum)
 {
  case 0: gb_snd_shift=0; break; //100 volumen
  case 1: gb_snd_shift=1; break; //75 volumen
  case 2: gb_snd_shift=2; break; //50 volumen
  case 3: gb_snd_shift=3; break; //25 volumen
  case 4: gb_snd_shift=4; break; //0 volumen
 }

 gb_show_osd_main_menu=0;
}



//********************************************************************************************
void ShowTinyRAM()
{   
 char cad[3][32];
 char *ptrCad[3]={cad[0],cad[1],cad[2]};
 unsigned int auxFree;

 sprintf(cad[0],"INIT: %d bytes (%d KB)",gb_ramfree_ini,(gb_ramfree_ini>>10));
 sprintf(cad[1],"SETUP: %d bytes (%d KB)",gb_ramfree_setupEnd,(gb_ramfree_setupEnd>>10));
 auxFree= ESP.getFreeHeap(); 
 sprintf(cad[2],"NOW: %d bytes (%d KB)",auxFree,(auxFree>>10));

 //for (unsigned char i=0;i<3;i++)
 //{
 // Serial.printf("%s\r\n",cad[i]);
 //}

 ShowTinyMenu("  RAM FREE",ptrCad, 0, 3);
 
 gb_id_menu_cur= id_menu_main;
 gb_show_osd_main_menu=0;
}

//********************************************************************************************
void ShowTinyAboutMenu()
{
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("   ABOUT", (char **)gb_about_menu_en,0,max_gb_about_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("ACERCA DE", (char **)gb_about_menu_sp,0,max_gb_about_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }
 
 gb_show_osd_main_menu=0;     
}

//********************************************************************************************
void ShowTinyHelpMenu()
{
 unsigned char aSelNum;
 if (gb_language_en==1)
 {
  aSelNum = ShowTinyMenu("    HELP", (char **)gb_help_menu_en,0,max_gb_help_menu);
 }
 else
 {
  aSelNum = ShowTinyMenu("   AYUDA", (char **)gb_help_menu_sp,0,max_gb_help_menu);
 }
 if (aSelNum == 255)
 {//Pulsa ESC vuelve a menu anterior Main menu
  gb_id_menu_cur= id_menu_main;
  return;
 }
 
 gb_show_osd_main_menu=0;          
}

//********************************************************************************************
void do_tinyOSD()
{
 unsigned char aSelNum;
 //int auxVol;
 //int auxFrec;
 
 //Serial.printf("do_tinyOSD BEGIN\r\n");

 if(gb_show_osd_main_menu==1)
 {
  OSDClear();
  
  while (gb_show_osd_main_menu==1)
  {                             
   switch (gb_id_menu_cur)
   {
    case id_menu_main: ShowTinyMainMenu(); break;
    case id_menu_load_level: ShowTinyLoadLevelMenu(); break;
    case id_menu_protection: ShowTinyProtectionMenu(); break;
    case id_menu_language: ShowTinyLanguageMenu(); break;
    case id_menu_video_options: ShowTinyVideoOptionsMenu(); break;
    case id_menu_video_mode: ShowTinyVideoModeMenu(); break;
    case id_menu_video_poll: ShowTinyVideoPollMenu(); break;
    case id_menu_video_filter: ShowTinyVideoFilterMenu(); break;
    case id_menu_keyb_options: ShowTinyKeyboardPollMenu(); break;
    case id_menu_cpu_options: ShowTinyCPUMenu(); break;
    case id_menu_osd: ShowTinyOSDMenu(); break;
    case id_menu_debug: ShowTinyDebugMenu(); break;
    case id_menu_help: ShowTinyHelpMenu(); break;
    case id_menu_reset: ShowTinyResetMenu(); break;
    case id_menu_sound_options: ShowTinySoundOptionsMenu(); break;
    case id_menu_sound_volumen: ShowTinySoundVolumenMenu(); break;
    case id_menu_sound_channel: ShowTinySoundChannelMenu(); break;
    case id_menu_ram: ShowTinyRAM(); break;
    case id_menu_about: ShowTinyAboutMenu(); break;    
    default: break;
   /*      
    case id_menu_video_filter: ShowTinyVideoFilterMenu(); break;    
   case id_menu_sound_options: ShowTinyAudioOptionsMenu(); break;
    case id_menu_sound_digital_mode: ShowTinyAudioDigitalModeMenu(); break;   
    case id_menu_sound_dac_mode: ShowTinyAudioDACModeMenu(); break;
     case id_menu_sound_dac_onoff: ShowTinyAudioDACOnOffMenu(); break;
     case id_menu_sound_dac_level: ShowTinyAudioDACLevelMenu(); break;
     case id_menu_sound_dac_halfwave: ShowTinyAudioDAChalfwaveMenu(); break;
    case id_menu_sound_poll: ShowTinyAudioPollMenu(); break;

   

   
   
   case id_menu_help: ShowTinyHelpMenu(); break;
      
   
   case id_menu_about: ShowTinyAboutMenu(); break;
   */
   }

   delay(20);
  }
 }
 
 OSDClear();
 gb_id_menu_cur= id_menu_main;
 gb_show_osd_main_menu=0;

 //Serial.printf("do_tinyOSD END\r\n");
 
 //return; 
}
