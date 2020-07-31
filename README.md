# Teensy4-USB-Audio-DAC-and-Volume-Control

Binary peak audio level display mode values range from 0 to 11111111:
<p align="left">
<img src=images/"BinaryMode1.jpg" width="450" />
</p>

This is a [**Teensy 4**](https://www.pjrc.com/store/) USB Audio DAC [**PCM5102**](https://www.robotics.org.za/PCM5102) and USB Volume Control using Volume Up and Down buttons (Teensy4USBAudioKeybrd2.ino) and with an additional rotary encoder as volume control (as in Teensy4USBAudioKeybrd7.ino).

The encoder switch toggles the display on and off, and also cycle through three different displaymodes, which is changed each time the switch is pressed (i.e. pressed twice). The three displaymodes are bar graph, and two binary modes (0 to 11111111), as normal and inverted text. (It was noted that the mute function is not functional for the Teensy 4 USB Audio device - even using the mute from directly in windows has no effect). It may also be convenient to attach an additional pushbutton to the Teensy 4 on-off pin - pressing it for 5 seconds will switch the DAC off and pressing it again for 2 seconds will switch it back on.

Later another DAC was tried - namely an [**ESS ES9023 Sabre**](http://www.suptronics.com/miniPCkits/x900.html) 24-bit/192kHz DAC, with connections in Raspberry Pi GPIO layout: DATA pin 40 to Teensy pin 7, BCLK pin 12 to Teensy pin 21, LRCK pin 35 to Teensy pin 20 (MCLK is left unconnected and do not connect it to GND). It used the Teensy 4.0's +5v supply and GND. Audio quality was equal or better (subjectively), than the PCM5102, and the USB volume control also worked as it should. Refer to the last picture on this page for further connection details.

Note that changes are required to boards.txt and to the Teensy 4 usb_desc.h - the current "All of the Above" option does not show in the Arduino IDE when using the unmodified files. Refer to the two screendumps in the Programming section below.

### Schematic and construction and parts

Schematic of the Teensy 4.0 with a PCM5102 or ES9023 DAC and a 0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield.
<p align="left">
<img src="Teensy4AudioDAC.jpg" width="640" /><br>
</p>

1 - Case bottom houses the Teensy 4.0 and the PCM5102 DAC. The case lid holds the 64x48 OLED, optionally 3 pushbuttons, and the rotary encoder.<br>
2 - All wired up - I decided to discard the three pushbuttons - they add little functionality. May add them again at a later stage.<br>
3 - Finished - but it deserves a proper aluminium knob instead of a toothpase cap.
<p align="left">
<img src="Case1.jpg" width="300" />  
<img src="Case4.jpg" width="300" />  
<img src="Case5.jpg" width="300" />  
</p>

0.66" SSD1306 Wemos D1 Mini 64x48 i2c OLED Shield and the KY040 Rotary Encoder.
<p align="left">
<img src="WemosOled.jpg" width="350" />  
<img src="KY040 Rotary Encoder.jpg" width="350" />  
</p>

PCM5102 DAC from [**Micro-Robotics**](https://www.robotics.org.za/PCM5102) and ES9023 DAC from [**Suptronics**](http://www.suptronics.com/miniPCkits/x900.html).
<p align="left">
<img src="dac.jpg" width="700" />  
</p><

### Programming

Audio device in windows.
<p align="left">
<img src="AudioDevice1.jpg" width="350" />  
</p>

Old and new USB choice in Arduino IDE for Teensy 4 (refer to the changes made in boards.txt and usb_desc.h).
<p align="left">
<img src="Teensy4USBTypes1.jpg" width="350" />  
<img src="Teensy4USBTypes2.jpg" width="350" /> 
</p>

### Initial testing and further construction details

Initial testing with 2 volume up/down pushbuttons, and also with a rotary encoder as a USB volume control.
<p align="left">
<img src="UsbAudioVolumeButtons2.jpg" width="350" />  
<img src="UsbAudioVolumeButtons1.jpg" width="350" />  
</p>

Left: Case bottom (at the top) houses the Teensy 4.0 and the PCM5102 DAC. The case lid (at the bottom) houses the 64x48 OLED, 3 pushbuttons (optional), and the rotary encoder.<br>
Right: ESS ES9023 Sabre DAC X900 Raspberry Pi GPIO format.
<p align="left">
<img src="Case2.jpg" width="350" />  
<img src="ES9023.jpg" width="350" />
</p>
