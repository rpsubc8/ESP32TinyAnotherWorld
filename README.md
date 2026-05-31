Another World port ESP32 TTGO VGA32 v1.4

<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewAnother.gif'></center>

Very preliminary version.

<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewAnotherCar.gif'></center>

<br><br>
<h1>Fake True Color</h1>
It's a scaling from 320x200 to 640x400 with 2x2 Bayer dithering.<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewAnotherFakeColor.gif'></center>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewAnotherCarFakeColor.gif'></center>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewLevel4FakeColor.gif'></center>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewLevel6FakeColor.gif'></center>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewLevel7FakeColor.gif'></center>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/previewLevel8FakeColor.gif'></center>

<br><br>
<h1>Features</h1>
<ul>
 <li><b>Video mode (normal version):</b>360x200, 320x200, 320x240, 512x384</li>
 <li><b>Video mode (fake true color version):</b>640x400</li>
 <li>RGB 16 bits conversion to BBGGRR 6 bits</li>
 <li>PSRAM</li>
 <li>Huge App Partition</li>
 <li>TTGO VGA32 v1.4</li>
 <li><b>keys:</b> Up, down, left, right, space, enter</li>
 <li>Video mode (normal version) boot key 0,1,2,3,4,5,6,7,8</li>
 <li>Video mode (fake true color version) boot key 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E</li>
 <li>Speed fast or normal (OSD menu)</li>
 <li>Load part game (OSD menu)</li>
 <li>BASIC sound DAC GPIO 25 (100%, 75%, 50%, 25%, 0)</li>
 <li>Visual Studio 1.48.1 PLATFORMIO 2.2.1 Espressif32 v3.3.2</li>
 <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
 <li>ArduinoDroid (6.3.1)</li> 
 <li><b>Precompile version: </b> <a href='https://github.com/rpsubc8/ESP32TinyAnotherWorld/tree/main/esp32/precompile'>https://github.com/rpsubc8/ESP32TinyAnotherWorld/tree/main/esp32/precompile</a></li>
</ul>

<br><br>
<h1>Video mode normal version boot</h1>
Both at the ESP32 startup (by pressing a key) and in the OSD, you can choose the video modes:<br><br>

| Key | Mode         | Detail             |
| --- | ------------ | ------------------ |
| 0   | 360x200x70hz | bitluni(6bpp)      |  
| 1   | 360x200x70hz | bitluni apll(6bpp) |
| 2   | 320x200x70hz | bitluni(6bpp)      |
| 3   | 320x200x70hz | fabgl(6bpp)        |
| 4   | 320x200x70hz | bitluni apll(6bpp) |
| 5   | 320x240x60hz | bitluni(6bpp)      |
| 6   | 320x240x60hz | fabgl(6bpp)        |
| 7   | 320x240x60hz | bitluni apll(6bpp) |
| 8   | 512x384x60hz | fabgl(6bpp)        |


<br><br>
<h1>Video mode fake True Color version</h1>
Both at the ESP32 startup (by pressing a key) and in the OSD, you can choose the video modes:<br><br>

| Key | Mode                                | Detail                                                    |
| --- | ------------------------------------| --------------------------------------------------------- |
| 0   | 640x400x70hz bitluni(6bpp)          | 70x400x70Hz                                               |
| 1   | 640x400x60hz END bitluni 6bpp       | 640x400x60Hz H(31.46 kHz)  frame(525 lines)  640x480x60Hz |
| 2   | 640x400x60hz WINDOW bitluni 6bpp    | 640x400x60Hz frame(525 lines)   640x480x60Hz              |
| 3   | 640x400x60hz MASK bitluni 6bpp      | 640x400x60Hz frame(400 lines)   640x480x60Hz              |
| 4   | 640x400x67hz HP bitluni 6bpp        | 640x400x67Hz  640x480x60Hz  640x350x67Hz                  | 
| 5   | 640x400x75hz FAST75 bitluni 6bpp    | 640x400x75Hz  frame(500 lines) 640x480x76Hz               |
| 6   | 640x400x67hz bitluni 6bpp           | 640x400x67Hz  H(31.5 kHz) V(67 Hz) 640x350x63Hz           |
| 7   | 640x400x75hz CVT75 bitluni 6bpp     | 640x400x75Hz  H(31.62 kHz)  640x350x75Hz                  |
| 8   | 640x400x70hz GTF70 bitluni 6bpp     | 640x400x70Hz  H(29.3 kHz)  640x350x72Hz                   |
| 9   | 640x400x75hz TRICK bitluni 6bpp     | 640x400x75Hz  frame(500 lines) 640x480x75Hz               |
| A   | 640x400x72hz TRICK bitluni 6bpp     | 640x400x72Hz  frame(520 lines)  640x480x73Hz              |
| B   | 640x400x71hz OK bitluni 6bpp        | 640x400x71Hz  640x480x60Hz  720x400x71Hz                  |
| C   | 640x400x60hz FORCE bitluni 6bpp     | 640x400x60Hz  H(30 Khz) V(60 Hz)    848x480x60Hz          |
| D   | 640x400x70hz POLAR bitluni 6bpp     | 720x400x70Hz                                              |
| E   | 640x400x60hz ASIMETRIC bitluni 6bpp | 640x400x60Hz  H(31.46 kHz)   720x400x70Hz                 |



<br><br>
<h1>Flash fake normal version</h1>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/flash01normal.gif'></center>


<br><br>
<h1>Flash fake True Color version</h1>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/flash01faketruecolor.gif'></center>

<br><br>
<h1>Flash fake True Color (combine) version</h1>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyAnotherWorld/main/preview/flash01combinefaketruecolor.gif'></center>

