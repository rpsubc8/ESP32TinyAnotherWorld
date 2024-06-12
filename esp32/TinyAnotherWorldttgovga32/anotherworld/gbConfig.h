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
 //This option is not yet available 
 //#define use_lib_boot_vga_360x200x70hz_bitluni_6bpp
 //#define use_lib_boot_vga_360x200x70hz_bitluni_apll_6bpp
 //#define use_lib_boot_vga_320x200x70hz_bitluni_6bpp
 //#define use_lib_boot_vga_320x200x70hz_fabgl_6bpp
 //#define use_lib_boot_vga_320x200x70hz_bitluni_apll_6bpp
 //#define use_lib_boot_vga_320x240x60hz_bitluni_6bpp
 //#define use_lib_boot_vga_320x240x60hz_fabgl_6bpp
 //#define use_lib_boot_vga_320x240x60hz_bitluni_apll_6bpp 

 //Time to select video mode at startup (ms)
 #define use_lib_boot_time_select_vga 500


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
 //#define use_lib_mini_test

#endif


