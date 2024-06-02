
#ifndef VGA_6BIT_H_FILE
#define VGA_6BIT_H_FILE

#include "gbConfig.h"
#ifdef use_lib_tinybitluni_fast

#include <cstdint>

//Modos de video
#define video_mode_360x200x70hz_bitluni 0 
#define video_mode_360x200x70hz_bitluni_apll_fix 1
#define video_mode_vga320x200x70hz_bitluni 2
#define video_mode_vga320x200x70hz_fabgl 3
#define video_mode_vga320x200x70hz_bitluni_apll_fix 4
#define video_mode_vga320x240x60hz_bitluni 5
#define video_mode_vga320x240x60hz_fabgl 6
#define video_mode_vga320x240x60hz_bitluni_apll_fix 7


//JJ class VgaMode;
//JJ extern const VgaMode vga_mode_320x240;
//JJ extern const VgaMode vga_mode_288x240;
//JJ extern const VgaMode vga_mode_240x240;

//JJ aniadido
//JJ extern const VgaMode vga_mode_320x200;
//JJ extern const VgaMode vga_mode_200x150;

//hfront hsync hback pixels vfront vsync vback lines divy pixelclock hpolaritynegative vpolaritynegative
const unsigned int VgaMode_vga_mode_200x150[12]={
 6,18,32,200, 1,2,22,600,4,9000000, 0,0
};

//320x200 720x400 31.4Khz 70Hz
const unsigned int VgaMode_vga_mode_320x200[12]={
 8,48,24,320,12,2,35,400,2,12587500,0,1 //funciona con 0,1
};

//320x240 640x480 31.4Khz 60Hz
const unsigned int VgaMode_vga_mode_320x240[12]={ 
 8,48,24,320,11,2,31,480,2,12587500,1,1
 //const Mode VGA::MODE320x240(8, 48, 24, 320, 11, 2, 31, 480, 2, 12587500, 1, 1);
};

//360x200 720x400 31.3Khz 70.4Hz
const unsigned int VgaMode_vga_mode_360x200[12]={ 
 8,54,28,360,11,2,32,400,2,14161000,0,1 //funciona con 0,1 polaridad v y h alreves
 //const Mode VGA::MODE360x200(8, 54, 28, 360, 11, 2, 32, 400, 2, 14161000, 1, 0);
};

//400x300 800x600 35.1KHz 56.2Hz
const unsigned int VgaMode_vga_mode_400x300[12]={
 12,36,64,400,1,2,22,600,2,18000000,0,0
};



//const int VgaMode_vga_mode_360x350[12]={
// 8,54,28,360,11,2,32,350,1,14161000,1,1 
//};

//360x400   720x400 31.2Khz 70.4Hz
const unsigned int VgaMode_vga_mode_360x400[12]={
 //8,54,28,360,11,2,32,400,1,14161000,1,0
 8,54,28,360,11,2,32,400,1,14161000,0,1
};

//320x350 31.4 Khz 640x350 70Hz
const unsigned int VgaMode_vga_mode_320x350[12]={
 //8,48,24,320,37,2,60,350,1,12587500,0,1
 8,48,24,320,37,2,60,350,1,12587500,1,0
};

//320x400 720x400 31.4Khz 70Hz
const unsigned int VgaMode_vga_mode_320x400[12]={
 //8,48,24,320,12,2,35,400,1,12587500,1,0
 8,48,24,320,12,2,35,400,1,12587500,0,1
};

//640x480
const unsigned int VgaMode_vga_mode_640x480[12]={
 16,96,48,640,11,2,31,480,1,25175000,1,1 
};

//640x400 720x400 31.4Khz 70Hz
const unsigned int VgaMode_vga_mode_640x400[12]={
 //16,96,48,640,12,2,35,400,1,25175000,1,0
 16,96,48,640,12,2,35,400,1,25175000,0,1
};


//400x300 custom(384,264) 800x600 35.1KHz 56.2Hz
//hf:12 hs:36 hb:60 hr:384 
//vf:37 vSync:2 vb:58 vr:528
//vd:2 pc:17280000
//hSyncPolarity:0 vSyncPolarity:0
const unsigned int VgaMode_vga_mode_400x300_384x264[12]={
 12,36,60,384 ,37,2,58,528 ,2,17280000,0,0
};

//400x300 custom(360,240) 800x600 35.2KHz 56.3Hz
//hf:8 hs:32 hb:60 hr:360
//vf:61 vSync:2 vb:82 vr:480
//vd:2 pc:16200000
//hSyncPolarity:0 vSyncPolarity:0
const unsigned int VgaMode_vga_mode_400x300_360x240[12]={
 8,32,60,360 ,61,2,82,480 ,2,16200000,0,0
};

//800x600   832x624 35.2Khz 54.2Hz
const unsigned int VgaMode_vga_mode_800x600[12]={
 24,72,128,800 ,1,2,22,600, 1,36000000,0,0
};





//extern const int VgaMode_vga_mode_200x150[12];


//JJ void vga_init(const int *vga_pins, const VgaMode &mode = vga_mode_320x240, bool double_buffered = true);
//void vga_init(const unsigned char *vga_pins, const VgaMode &mode = vga_mode_320x240, bool double_buffered = true);

void vga_init(const unsigned char *pin_map, const unsigned int *mode, bool double_buffered,unsigned char pllcteforce, unsigned int p0,unsigned int p1,unsigned int p2,unsigned int p3,unsigned char pllcustomforce,unsigned char is8colors, unsigned char vga_video_mode_id);

void vga_swap_buffers(bool wait_vsync = true);
void vga_clear_screen(uint8_t color);
unsigned char **vga_get_framebuffer();
unsigned char vga_get_sync_bits();
int vga_get_xres();
int vga_get_yres();

unsigned char vga_get_vsync_inv_bit(void);
unsigned char vga_get_hsync_inv_bit(void);
int vga_get_y_res(void);
void vga_free(void);
void vga_freeGPIO(void);
unsigned int vga_get_pll_cte_p0(void);
unsigned int vga_get_pll_cte_p1(void);
unsigned int vga_get_pll_cte_p2(void);
unsigned int vga_get_pll_cte_p3(void);

void SetVideoInterrupt(unsigned char auxState);
//void WaitVsync(void);
void custom_rtc_clk_apll_enable(bool enable, uint32_t sdm0, uint32_t sdm1, uint32_t sdm2, uint32_t o_div);
void FreeInterrupt(void);

#endif

#endif //VGA_6BIT_H_FILE
