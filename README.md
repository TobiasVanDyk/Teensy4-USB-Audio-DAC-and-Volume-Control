# Teensy4-USB-Audio-DAC-and-Volume-Control
This is a Teensy 4 (https://www.pjrc.com/store/) USB Audio DAC (PCM 5102) and Volume Control using Volume Up and Down buttons (Teensy4USBAudioKeybrd2.ino) and with an additional rotary encoder as volume control (Teensy4USBAudioKeybrd6.ino). 

The encoder push-button switches the display on and off (the mute function is not enabled in the Teensy 4 USB Audio device). It is also convenient to attach a pushbutton to the Teensy 4 on/off pin - pressing it for 5 seconds will switch the DAC off and pressing it again for 2 seconds will switch it back on.

### Schematic and construction and parts
Schematic Teensy 4.0 and a PCM5102 DAC and a 0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield:
<br>
<p align="center">
<img src="Teensy4AudioDAC2.png" width="640" /> 
<br>
  
Bottom part contains the Teensy 4.0 and the PCM5102 DAC. The case lid holds the 64x48 OLED, 3 pushbuttons, and the encoder.
<br>
<p align="center">
<img src="Case1.png" width="940" />  
<br>  
All wired up - decide to discard the three pushbuttons - they add little functionality.
<br>
<p align="center">
<img src="Case4.png" width="940" />  
<br>
  
Finished - must buy a proper aluminium knob instead of a toothpase cap.
<br>
<p align="center">
<img src="Case5.png" width="940" />  
<br>
  
0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield:
<br>
<p align="center">
<img src="WemosOled.png" width="700" />  
<br> 
  
KY040 Rotary Encoder2:
<br>
<p align="center">
<img src="KY040 Rotary Encoder.png" width="900" />  
<br>

### Programming

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

### Initial testing and further construction details:
<p align="center">
<img src="UsbAudioVolumeButtons2.png" width="940" />  
<br>
<p align="center">
<img src="UsbAudioVolumeButtons1.png" width="940" />  
<br>

Bottom part contains the Teensy 4.0 and the PCM5102 DAC. The top part consists of the 64x48 OLED, 3 pushbuttons, and the encoder.
<br>
<p align="center">
<img src="Case2.png" width="940" />  
<br>
<p align="center">
<img src="Case3.png" width="940" />  
<br>
