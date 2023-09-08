const int TEST_CASE = 1;

/*
  Configure the pins for the buttons and LEDS here
  -----------------------------------------------------
*/
const int BTN1 = 3;  // Button closest to header pins
const int BTN2 = 4;  // Button fartheest from LEDs
const int BTN3 = 5;  // Button farthest from header pins
const int BTN4 = 2;  // Button closest to LEDs

const int LED1 = 12;  // Leftmost LED
const int LED2 = 11;
const int LED3 = 10;
const int LED4 = 9;
const int LED5 = 8;  // Rightmost LED
/*
  ---------------------------------------------------
*/

const int BTN_PRESSED_SIGNAL = LOW;

/*
  Ordered such that index of LED in LEDS is their bit position in LED double bit pattern
  E.g. for Sample Pattern: 1101010000
  pattern:        11    01    01    00    00
  Bit Pos:         4     3     2     1     0
  Assigned LED: LED1  LED2  LED3  LED4  LED5
*/
const int LEDS[] = { LED5, LED4, LED3, LED2, LED1 };
const int LED_COUNT = 5;


/*
  LED Double Bit Pattern uses 2 bits for each LED:
  00 -> off
  01 -> blinking
  11 -> on

  You can combine the enumerated types to make a simple LED pattern, e.g.
  LED1 is on, but LED2 and LED3 will be blinking
  = LED1_ON | LED2_BLINKING | LED3_BLINKIng
*/
enum LEDDoubleBitPattern {
  LED_OFF=0b000,
  LED_BLINKING=0b001,
  LED_ON=0b011,

  /*
  LEDs are ordered left to right, so bit pattern:
  Case: LED1 stable, LED2 and LED3 blinking
  Sample pattern: 11   01   01   00   00
  Bit Pos:        4    3    2    1    0
  Assigned LED:   LED1 LED2 LED3 LED4 LED5
  */

  LED5_OFF      = LED_OFF      << 0,
  LED5_BLINKING = LED_BLINKING << 0,
  LED5_ON       = LED_ON       << 0,
  
  // left shift by 2 as each LED has 2 bits asssigned to it
  LED4_OFF      = LED_OFF      << 2,
  LED4_BLINKING = LED_BLINKING << 2,
  LED4_ON       = LED_ON       << 2,

  LED3_OFF      = LED_OFF      << 4,
  LED3_BLINKING = LED_BLINKING << 4,
  LED3_ON       = LED_ON       << 4,

  LED2_OFF      = LED_OFF      << 6,
  LED2_BLINKING = LED_BLINKING << 6,
  LED2_ON       = LED_ON       << 6,

  LED1_OFF      = LED_OFF      << 8,
  LED1_BLINKING = LED_BLINKING << 8,
  LED1_ON       = LED_ON       << 8,

  ALL_LEDS_ON = 0b01111111111,
  ALL_LEDS_BLINKING = 0b0101010101,
};

void millisDelay(unsigned long interval){
  unsigned long startingMillis = millis();
  while ( millis() - startingMillis < interval );
}

bool isBitHigh(long n, int pos){
  // Bits are numbered from right to left
  // 3 2 1 0
  // 1 0 0 1
  unsigned long bt = 1 << pos;
  return ( (n & bt) != 0 );
}

void setLEDsToDoubleBitPattern(unsigned int pattern, int blinkingLEDValue){
  /*
    Sets the LEDs to the specified double bit pattern, takes a blinking LED value for LEDs that are set to be blinking
    Uses double bits, see the enum LEDDoubleBitPattern for more information
  */

  unsigned int mask = 0b11;
  unsigned int curLedBits;

  for(int ledPos=0; ledPos < LED_COUNT; ledPos++){
    // left shift the mask so that we can isolate the LED double bits

    curLedBits = (pattern >> (ledPos*2)) & mask;

    // then use switch case to determine what to do
    if ( curLedBits == LED_ON)
      digitalWrite(LEDS[ledPos], HIGH);
    else if (curLedBits == LED_BLINKING){
      // printTime("Writing led pos to value");
      // Serial.println(ledPos);
      // Serial.println(blinkingLEDValue);
      digitalWrite(LEDS[ledPos], blinkingLEDValue);
    }
    else
      digitalWrite(LEDS[ledPos], LOW);
  }

}

void setLEDsToDoubleBitPattern(unsigned int pattern) { setLEDsToDoubleBitPattern(pattern, LOW); }

void resetLEDs(){ setLEDsToDoubleBitPattern(LED_OFF); }

int waitForBtnPressAndBlinkLEDs(int btns[], int btnCount, unsigned int ledPattern){
  /*
    Waits for any of the given buttons to be pressed, while blinking the LEDs at the specified interval
    Returns the pin of the button pressed
    ledPattern is an led double bit pattern
  */

  if ( btnCount < 1 ) return -1;

  int pressedBtn = -1;
  unsigned long start;
  unsigned long interval = 500;
  unsigned long cur;
  int curInterval = 0, prevInterval = -1 ;
  int curBlinkingValue = HIGH;

  // LEDS start high
  setLEDsToDoubleBitPattern(ledPattern, curBlinkingValue);
  start = millis();

  while ( pressedBtn == -1 ){

    // check the button for state
    for(int i=0; i < btnCount; i++) {
      if (digitalRead(btns[i]) == BTN_PRESSED_SIGNAL) {
        pressedBtn = btns[i];
        // block till button is released
        while ( digitalRead(pressedBtn) == BTN_PRESSED_SIGNAL );
        break;
      }
    }

    if ( pressedBtn != -1 ) break;

    // set the state of the LEDs
    cur = millis();
    curInterval = (int)((cur - start) / interval);
    if (cur != start && (cur - start) % interval  == 0 && prevInterval != curInterval ){
      curBlinkingValue = !curBlinkingValue;
      setLEDsToDoubleBitPattern(ledPattern, curBlinkingValue);

      // set the new interval
      prevInterval = curInterval;
    }
  }

  resetLEDs();
  return pressedBtn; 
}

unsigned int ledPattern;
void setup() {
  int outputPins[]= { LED1, LED2, LED3, LED4, LED5 };
  int inputPins[] = { BTN1, BTN2, BTN3, BTN4 };

  /*
    Set the mode for output pins to output
  */
  for ( int pin : outputPins ){
    pinMode(pin, OUTPUT);
  }

  /*
    Set the mode for input pins to input
  */
  for ( int pin : inputPins ){
    pinMode(pin, INPUT);
  }

  randomSeed(analogRead(0));

  ledPattern = LED5_ON;
}

int btns[] = { BTN1, BTN2, BTN3, BTN4 };

void loop() {
  if ( TEST_CASE == 1) {
    // testing LED output
    // LEDs should light up left to right
    setLEDsToDoubleBitPattern(ALL_LEDS_ON);
    delay(1000);
    unsigned int pt = LED1_ON;
    int cnt = 5;
    while ( cnt > 0 ){
      setLEDsToDoubleBitPattern(pt);
      delay(1000);
      pt = pt >> 2;
      cnt--;
    }
  }
  else if (TEST_CASE == 2){
    // initial program starts with LED5 on
    // Button 1 turns on LED 1
    // Button 2 turns on LED 2
    // Button 3 turns on LED 3
    // Button 4 turns on LED 4
    int btn = waitForBtnPressAndBlinkLEDs(btns, 4, ledPattern);
    switch(btn){
      case BTN1:
        ledPattern = LED1_ON;
        break;
      case BTN2:
        ledPattern = LED2_ON;
        break;
      case BTN3:
        ledPattern = LED3_ON;
        break;
      case BTN4:
        ledPattern = LED4_ON;
        break;
      default:
        ledPattern = LED5_ON;
        break;
    }
  }
}