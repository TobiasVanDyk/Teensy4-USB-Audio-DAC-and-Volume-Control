// PCM5102 bd   Teensy 4
// VCC          5V
// GND          GND
// LRCK         20
// DATA         7
// BCK          21

// set Tools > USB Type to Audio (+ Serial if needed)
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce.h>

#define DIR_NONE 0x00           // No complete step yet.
#define DIR_CW   0x10           // Clockwise step.
#define DIR_CCW  0x20           // Anti-clockwise step.

// Volume
unsigned int state;
int count = 0;         // count each indent
int old_count = 0;     // check for count changed

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
#define RotaryC 4        // Volume Mute = Button on Volume

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(0, 10);  // 10 ms debounce time is appropriate
Bounce button1 = Bounce(1, 10);  // for most mechanical pushbuttons

boolean AnyBtnPress = false;
byte n = 255;
byte VolumeLevel = 16;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// GUItool: begin automatically generated code
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
// GUItool: end automatically generated code

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
        //VolumeLevel = 255;
      }
    if( state & DIR_CCW )   
      { count--;                           // count down for counterclockwise
        Keyboard.press(KEY_MEDIA_VOLUME_DEC); 
        Keyboard.release(KEY_MEDIA_VOLUME_DEC);
        //VolumeLevel = 32; 
      }
    
}

void DACInfo()
{ display.println("PCM5102");
  display.println("USB DAC");
  display.println();
  display.println("32bit");
  display.println("384kHz");
  display.display();
}

void ClrDisplay()
{ display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  //display.display();
}

void VolumeNum (long VNum, int TopBottom)
{ display.setCursor(0,0); // x and y position 0-63, 0-47
  display.print("V:");
  display.print(VNum); 
  display.println();
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
  // Could interfere with pin 13 signals
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
  
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);

  pinMode(RotaryA, INPUT_PULLUP);  //5
  pinMode(RotaryB, INPUT_PULLUP);  //6
  pinMode(RotaryC, INPUT_PULLUP);  //4

  AnyBtnPress = false;

  state = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );     // Initialise state.
  old_count = 0;

  attachInterrupt( RotaryA, AB_isr, CHANGE );   // pin-change interrupts: 
  attachInterrupt( RotaryB, AB_isr, CHANGE );
  
  // generate the high voltage from the 3.3v line internally
  // initialize with the I2C addr 0x3C (for the 64x48)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.display();
  delay(1000); // init settling time
  ClrDisplay();
  DACInfo();
  //delay(2000);

}

elapsedMillis fps=0;
int quiet=1;

void loop()
{
  float vol = usb1.volume();  // read PC volume setting
  amp1.gain(vol);             // set gain according to PC volume
  amp2.gain(vol);

  quiet=1;
  
  if(fps > 200) 
  { if (peak1.available() && peak2.available())
       { fps=0;
         quiet=0;
         uint8_t leftPeak=peak1.read() * 50.0;
         uint8_t rightPeak=peak2.read() * 50.0;
      
         ClrDisplay();
         PeakNum (leftPeak, rightPeak, 0);
         //VolumeNum (newPosition, 0);
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
  
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();

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

  
}
