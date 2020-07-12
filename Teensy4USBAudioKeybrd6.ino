// PCM5102-DAC  Teensy 4
// VCC          5V
// GND          GND
// LRCK         20 LRCLK1
// DATA         7  OUT1A
// BCK          21 BCLK1

//////////////////////////////////////////////////////////////////
// Set Tools -> USB Type -> (All of the Above)
// Audio + Keyboard (mediakeys) (+ pseudoSerial)
// This requires the modifide boards.txt and usb_desc.h
// Re-programming is easier if Teensy loader is closed
// before pressing the reset button on Teensy 4 - else
// it is difficult to show the port in the Arduino IDE (windows)
//////////////////////////////////////////////////////////////////
#include <Audio.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  // Used an old version
#include <Bounce.h>

#define DIR_NONE 0x00           // No complete step yet.
#define DIR_CW   0x10           // Clockwise step.
#define DIR_CCW  0x20           // Anti-clockwise step.

// Roatary encoder and Volume
volatile unsigned int state;  // Used in interrupt
volatile int count = 0;       // count each indent
volatile int old_count = 0;   // check for count changed

// State definitions state table (emits a code at 00 only)
// states are: (NAB) N = 0: clockwise;  N = 1: counterclockwiswe
#define R_START     0x3
#define R_CW_BEGIN  0x1
#define R_CW_NEXT   0x0
#define R_CW_FINAL  0x2
#define R_CCW_BEGIN 0x6
#define R_CCW_NEXT  0x4
#define R_CCW_FINAL 0x5

const unsigned char ttable[8][4] = {
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},                // R_CW_NEXT
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_BEGIN,  R_START},                // R_CW_BEGIN
    {R_CW_NEXT,  R_CW_FINAL,  R_CW_FINAL,  R_START | DIR_CW},       // R_CW_FINAL
    {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},                // R_START
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},                // R_CCW_NEXT
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_FINAL, R_START | DIR_CCW},      // R_CCW_FINAL
    {R_CCW_NEXT, R_CCW_BEGIN, R_CCW_BEGIN, R_START},                // R_CCW_BEGIN
    {R_START,    R_START,     R_START,     R_START}                 // ILLEGAL
};

#define RotaryA 5 
#define RotaryB 6 
#define RotaryC 4        // Press Knob = Volume Mute and Display = off

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// Create Bounce objects for each button
// Pushbuttons are optional
Bounce button0 = Bounce(0, 10);  // 10 ms debounce
Bounce button1 = Bounce(1, 10);  
Bounce button4 = Bounce(4, 10);  

int DisplayOff = 0;
elapsedMillis fps = 0;
int quiet = 1;

// Teensy GUItool generated code
AudioInputUSB            usb1;           //xy=97,150
AudioAmplifier           amp1;           //xy=318,120
AudioAmplifier           amp2;           //xy=318,204
AudioAnalyzePeak         peak1;          //xy=476,60
AudioAnalyzePeak         peak2;          //xy=478,273
AudioOutputI2S           i2s1;           //xy=794,141
AudioConnection          patchCord1(usb1, 0, amp1, 0);
AudioConnection          patchCord2(usb1, 1, amp2, 0);
AudioConnection          patchCord3(amp1, 0, i2s1, 0);
AudioConnection          patchCord4(amp1, peak1);
AudioConnection          patchCord5(amp2, 0, i2s1, 1);
AudioConnection          patchCord6(amp2, peak2);

////////////////////////////////////////////////////////
// Valid transition states ISR roto_sm.ino by boolrules
////////////////////////////////////////////////////////
void AB_isr( ) 
{
    // Grab state of input pins.
    unsigned char pinstate = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );

    // Determine new state from the pins and state table.
    state = ttable[state & 0x07][pinstate];

    if( state & DIR_CW )    
      { count++;                            // count up for clockwise
        Keyboard.press(KEY_MEDIA_VOLUME_INC); 
        Keyboard.release(KEY_MEDIA_VOLUME_INC);
      }
    if( state & DIR_CCW )   
      { count--;                           // count down for counterclockwise
        Keyboard.press(KEY_MEDIA_VOLUME_DEC); 
        Keyboard.release(KEY_MEDIA_VOLUME_DEC);
      }
    
}

void DACInfo()
{ display.println("PCM5102");
  display.println("USB DAC");
  display.println();
  display.println("16bit");   // Actually 32bit
  display.println("44.1kHz"); // 384kHz
}

void ClrDisplay()
{ display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  //display.display();
}

void PeakNum (uint8_t LPeak, uint8_t RPeak, int TopBottom)
{ display.setCursor(0,0); // x and y position 0-63, 0-47
  display.print("L:");
  display.print(LPeak); 
  display.setCursor(31,0); // x and y position 0-63, 0-47
  display.print("R:");
  display.print(RPeak); 
  display.println();
}

void PeakBar (uint8_t Peak, char *LR)
{ char *Bar[] = {""," ","  ","   ","    ","     ","      ","       "};
  if (Peak>7) Peak=7;
  display.setTextColor(WHITE);
  display.print(LR);
  display.setTextColor(BLACK, WHITE); // 'inverted' text 
  display.println(Bar[Peak]);
  display.println();
}

void setup()
{
  AudioMemory(12);
 
  // Configure the pins for input mode with pullup resistors.
  pinMode(0, INPUT_PULLUP); // Pin 0
  pinMode(1, INPUT_PULLUP); // Pin 1
  pinMode(4, INPUT_PULLUP); // Pin 4 Rotary Encoder Push Switch
  
  pinMode(RotaryA, INPUT_PULLUP);    // Pin 5 interrupt attached
  pinMode(RotaryB, INPUT_PULLUP);    // Pin 6 interrupt attached
 
  DisplayOff = 0;
  
  state = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );     // Initialise state.
  old_count = 0;

  attachInterrupt( RotaryA, AB_isr, CHANGE );   // pin-change interrupts: 
  attachInterrupt( RotaryB, AB_isr, CHANGE );
  
  // generate the high voltage from the 3.3v line internally
  // initialize with the I2C addr 0x3C (for the 64x48)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.display();
  delay(1000);    // init settling time
  ClrDisplay();
  DACInfo();      // Initial Info
  delay(1000);    // init settling time
}

void loop()
{
  float vol = usb1.volume();  // read PC volume setting
  amp1.gain(vol);             // set gain according to PC volume
  amp2.gain(vol);

  quiet=1;
  
  if ((fps > 200) && (DisplayOff == 0)) 
  { if (peak1.available() && peak2.available())
       { fps=0;
         quiet=0;
         uint8_t leftPeak=peak1.read() * 50.0;
         uint8_t rightPeak=peak2.read() * 50.0;
      
         ClrDisplay();
         PeakNum (leftPeak, rightPeak, 0);
         display.println();
         PeakBar (leftPeak, "L ");
         PeakBar (rightPeak, "R ");
         display.display();
       } else if (quiet==1)
                 { ClrDisplay();
                   PeakNum (0, 0, 0);
                   display.println();
                   PeakBar (0, "L ");
                   PeakBar (0, "R ");
                   display.display();
                 } 
  }
  
  // Update all the buttons
  button0.update();
  button1.update();
  button4.update();

  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button0.fallingEdge()) {
    Keyboard.press(KEY_MEDIA_VOLUME_DEC);
    Keyboard.release(KEY_MEDIA_VOLUME_DEC);
  }
  if (button1.fallingEdge()) {
    Keyboard.press(KEY_MEDIA_VOLUME_INC);
    Keyboard.release(KEY_MEDIA_VOLUME_INC);
  }
  if (button4.fallingEdge()) {
    if (DisplayOff == 0) 
       { ClrDisplay();
         display.display();  
         DisplayOff = 1; 
       } else DisplayOff = 0; 
    //Keyboard.press(KEY_MEDIA_MUTE);   // Has no effect problem with Teensy Audio USB device
    //Keyboard.release(KEY_MEDIA_MUTE); // Even if mute pressed inside windows no effect on volume
  }
  
}
