/*

  This is a simple test program for the 4x4 button pad from sparkfun
  For details, please see http://interactingobjects.com/wp-admin/post.php?post=380

*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// Key mapping => Each key is assigned a char which will be used
//                to determine row / col of pressed key
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};

// Rows and cols pins definition
byte pinLedRGB[ROWS][3] = {{2,3,4},{5,6,7},{8,9,10},{11,12,13}}; // 3 pins (R,G & B) per LEDs rows
byte pinBtn[ROWS]       = {22,23,24,25};                         // Buttons rows
byte pinLedGnd[COLS]    = {30,31,32,33};                         // LEDs cols (GND)
byte pinBtnGnd[COLS]    = {26,27,28,29};                         // Buttons cols (GND)

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), pinBtn, pinBtnGnd, ROWS, COLS); 

// Standard colors 
const byte COLORS=8;
byte colors[COLORS][3] =  { 
  {255, 0  , 0  }, 
  {0  , 255, 0  }, 
  {0  , 0  , 255},
  {255, 255, 0  },
  {255, 0  , 255},
  {0  , 255, 255},
  {255, 255, 255},
  {255, 254, 0}
};
                            
//-------------------------------------------------------------------------
// To switch off all leds
//-------------------------------------------------------------------------
void ledResetMatrix() {
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledOff(row,col);
    } 
  }
}

//-------------------------------------------------------------------------
// To enable 1 led
//-------------------------------------------------------------------------
void ledOn(byte row, byte col, byte color[3]) {
  analogWrite(pinLedRGB[row][0],color[0]);
  analogWrite(pinLedRGB[row][1],color[1]);
  analogWrite(pinLedRGB[row][2],color[2]);
  
  digitalWrite(pinLedGnd[col],LOW);
}

//-------------------------------------------------------------------------
// To disable 1 led
//-------------------------------------------------------------------------
void ledOff(byte row, byte col) {
  analogWrite(pinLedRGB[row][0],0);
  analogWrite(pinLedRGB[row][1],0);
  analogWrite(pinLedRGB[row][2],0);
  
  digitalWrite(pinLedGnd[col],HIGH);
}

//-------------------------------------------------------------------------
// Switch on all leds sequentially
//-------------------------------------------------------------------------
void testAllLeds() {
  int period=50; // in ms
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
       ledOn(row,col,colors[(col+row)%COLORS]);
       delay(period);
       ledOff(row,col);     
    }
  }
}

//-------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Setting pins
  for(int row=0;row<ROWS;row++) {
    pinMode(pinBtn[row],OUTPUT);
    pinMode(pinBtnGnd[row],OUTPUT);
    pinMode(pinLedGnd[row],OUTPUT);
    for(int col=0;col<COLS;col++) {
      pinMode(pinLedRGB[row][col],OUTPUT);
      ledOff(row,col);
    }
  }
  
  // And testing leds
  testAllLeds();
}

//-------------------------------------------------------------------------
byte count=0;          // Number of time a key was pressed. Used to "rotate" thru colors
byte key;              // Key pressed

void loop() {
  // Reading keyboard
  char customKey=customKeypad.getKey();

  // Key was pressed, customKey is non empty
  if (customKey){

    char hexKey[]= { '0', 'x', '0', 0 };
    hexKey[2]=customKey;                  // Building an hex number...
    key = strtol(hexKey,0,16);            // ... so it can be converted to an int
    byte row=key/4;                       // ... to calculate the coordinates of the key
    byte col=key%4;
    
    ledResetMatrix();                     // Switching off all leds ...
    ledOn(row,col,colors[count%COLORS]);  // ... before switching on the one which was pressed

    count++;                              // incrementing count

    // Some debug output
    Serial.print("Hex:");Serial.print(hexKey);
    Serial.print("|Key:"); Serial.print(key);
    Serial.print("|Prv:"); Serial.print(previousKey);
    Serial.print("|Row:"); Serial.print(key/4);
    Serial.print("|Col:"); Serial.print(key%4);
    Serial.print("|Cnt:"); Serial.print(count);
    Serial.println();
  }
}
