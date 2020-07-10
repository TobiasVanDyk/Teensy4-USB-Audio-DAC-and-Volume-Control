# Teensy4-USB-Audio-DAC-and-Volume-Control
This is a Teensy 4 (https://www.pjrc.com/store/) USB Audio DAC (PCM 5102) and Volume Control using Volume Up and Down buttons (Teensy4USBAudioKeybrd2.ino) and with an additional rotary encoder as volume control (Teensy4USBAudioKeybrd6.ino). 

The encoder push-button switches the display on and off (the mute function is not enabled in the Teensy 4 USB Audio object). It is also convenient to attach a pushbutton to the Teensy 4 on/off pin - pressing it for 5 seconds will switch the DAC off and pressing it again for 2 seconds will switch it back on.
<br>
<p align="center">
<img src="UsbAudioVolumeButtons2.png" width="940" />  
<br>
<p align="center">
<img src="UsbAudioVolumeButtons1.png" width="940" />  
<br>

Schematic Teensy 4.0 and a PCM5102 DAC and a 0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield:
<br>
<p align="center">
<img src="Teensy4AudioDAC2.png" width="640" /> 
<br>
  
0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield:
<br>
<p align="center">
<img src="WemosOled.png" width="700" />  
<br> 
  
KY040 Rotary Encoder2:
<br>
<p align="center">
<img src="KY040 Rotary Encoder1.jpg" width="300" />  
<p align="center">
<img src="KY040 Rotary Encoder2.jpg" width="320" />  
<br>
  
Audio device in windows:
<br>
<p align="center">
<img src="AudioDevice1.png" width="414" />  
<br>

New USB choice in Arduino IDE for Teensy 4 (refer to changes in boards.txt and usb_desc.h):
<br>
<p align="center">
<img src="Teensy4USBTypes1.png" width="506" />  
<br>
<p align="center">
<img src="Teensy4USBTypes2.png" width="584" />  
<br> 
  

