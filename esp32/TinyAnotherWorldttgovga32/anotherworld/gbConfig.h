#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //Use data file from dataflash
 #define use_lib_dataflash
 
 //Use sound esp32
 #define use_lib_esp32sound
 //Use only one channel
 #define use_lib_esp32sound_mono_channel
 
 //usar bitluni scanline pointer
 #define use_lib_bitlunivga

 //usar biluni vga6bit.cpp
 #define use_lib_tinybitluni_fast

 //Not use double numbers (IEEE not fpu) calculate vga frequency
 //#define use_lib_fix_double_precision

 //Debug i2s bitluni vga frequency
 #define use_lib_debug_i2s


 //Video mode boot (select only one option)
 #define use_lib_boot_vga_360x200x70hz_bitluni_6bpp
 //#define use_lib_boot_vga_360x200x70hz_bitluni_apll_6bpp
 //#define use_lib_boot_vga_320x200x70hz_bitluni_6bpp
 //#define use_lib_boot_vga_320x200x70hz_fabgl_6bpp
 //#define use_lib_boot_vga_320x200x70hz_bitluni_apll_6bpp
 //#define use_lib_boot_vga_320x240x60hz_bitluni_6bpp
 //#define use_lib_boot_vga_320x240x60hz_fabgl_6bpp
 //#define use_lib_boot_vga_320x240x60hz_bitluni_apll_6bpp
 //#define use_lib_boot_vga_512x384x60hz_fabgl_6bpp

 //Time to select video mode at startup (ms)
 #define use_lib_boot_time_select_vga 500



 //4 buffer videos in psram o sram
 //#define use_lib_video4buffers_psram



 //log trace
 #define use_lib_log_serial


 //keyboard remote UART
 //#define use_lib_keyboard_uart

 //timeout read millis
 #define use_lib_keyboard_uart_timeout 0

 //log keyboard serial
 //#define use_lib_log_keyboard_uart


 //Logs
 //#define use_log_readEntriesFlash
 //#define use_log_fps_show
 //#define use_log_snd_playSound

 //Version de pruebas minimos datos de prueba
 //#define use_lib_ultra_mini_test
 //#define use_lib_mini_test










 #ifdef use_lib_boot_vga_512x384x60hz_fabgl_6bpp
  //512x384 force use PSRAM to free SRAM
  #ifndef use_lib_video4buffers_psram
   #define use_lib_video4buffers_psram
  #endif 
 #endif


 #ifdef use_lib_boot_vga_360x200x70hz_bitluni_6bpp
  //360-320= 40 DIV 2= 20 DIV 4 = 5
  #define use_lib_offset_x 5
  #define use_lib_offset_y 0
  #define use_lib_video 0
  #define use_lib_width 360
  #define use_lib_height 200
 #else
  #ifdef use_lib_boot_vga_360x200x70hz_bitluni_apll_6bpp
   #define use_lib_offset_x 5
   #define use_lib_offset_y 0
   #define use_lib_video 1
   #define use_lib_width 360
   #define use_lib_height 200   
  #else 
   #ifdef use_lib_boot_vga_320x200x70hz_bitluni_6bpp
    #define use_lib_offset_x 0
    #define use_lib_offset_y 0
    #define use_lib_video 2
    #define use_lib_width 320
    #define use_lib_height 200    
   #else
    #ifdef use_lib_boot_vga_320x200x70hz_fabgl_6bpp 
     #define use_lib_offset_x 0
     #define use_lib_offset_y 0
     #define use_lib_video 3
     #define use_lib_width 320
     #define use_lib_height 200         
    #else 
     #ifdef use_lib_boot_vga_320x200x70hz_bitluni_apll_6bpp
      #define use_lib_offset_x 0
      #define use_lib_offset_y 0     
      #define use_lib_video 4
      #define use_lib_width 320
      #define use_lib_height 200          
     #else
      #ifdef use_lib_boot_vga_320x240x60hz_bitluni_6bpp
       #define use_lib_offset_x 0
       #define use_lib_offset_y 20      
       #define use_lib_video 5
       #define use_lib_width 320
       #define use_lib_height 240    
      #else 
       #ifdef use_lib_boot_vga_320x240x60hz_fabgl_6bpp
        #define use_lib_offset_x 0
        #define use_lib_offset_y 20             
        #define use_lib_video 6
        #define use_lib_width 320
        #define use_lib_height 240            
       #else 
        #ifdef use_lib_boot_vga_320x240x60hz_bitluni_apll_6bpp
         #define use_lib_offset_x 0
         #define use_lib_offset_y 20                     
         #define use_lib_video 7
         #define use_lib_width 320
         #define use_lib_height 240             
        #else 
         #ifdef use_lib_boot_vga_512x384x60hz_fabgl_6bpp
          #define use_lib_offset_x 24
          #define use_lib_offset_y 92
          #define use_lib_video 8
          #define use_lib_width 512
          #define use_lib_height 384              
         #endif
        #endif
       #endif
      #endif
     #endif
    #endif
   #endif
  #endif
 #endif




#endif


