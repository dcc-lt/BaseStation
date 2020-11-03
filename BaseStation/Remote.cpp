/*
  d. bodnar  revised 6-16-2016
*/
#include "Arduino.h"
const int ledPin =  13;      // the number of the LED pin
int buttonPin = 4; // button on rotary
static int pinA = 2; // Rotary Encoder
static int pinB = 3; // Rotary Encoder
volatile byte aFlag = 0;
volatile byte bFlag = 0;
volatile byte encoderPos = 0;
volatile byte oldEncPos = 0;
volatile byte reading = 0;
int old_pos = encoderPos;
int dir = 0; // direction
int buttonState = 0;
int encoderChange = 0; // flag to show encoder changed
byte Key;
#include<EEPROM.h>
char key ;
int LED = 13; // LED to blink when DCC packets are sent in loop
byte Fx = 0;
// Array set for 4 Loco2 - change the 7 numbers in the next 7 declarations
int maxLocos = 4;// number of loco addresses
int LocoAddress[4] = {1830, 3, 999, 4444};
int LocoDirection[4] = {1, 1, 1, 1};
int LocoSpeed[4] = {0, 0, 0, 0};
byte LocoFN0to4[4];
byte LocoFN5to8[4];
byte Locofn9to12[4];// 9-12 not yet implemented
int xxxxx = 0;
int pot1Pin = A3;
int potValue = 0;
int NewPotValue = 0;
int potValueOld = 0;
int counter = 0;
int Trigger1 = 3;
int Trigger2 = 4;
//int TrigVal1 = 0;
//int TrigVal2 = 0;
int old_speed = 0;
int ZeroSpeedFlag = 0;
int ActiveAddress = 0; // make address1 active
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
int z = 0;
int powerTemp = 0;
int i = 0;
char VersionNum[] = "2.6a"; ///////////////////////// //////////////////////VERSION HERE///////
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 6, 7, 8 }; //{8,7,6,5 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 10, 11}; // {11,10,9}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//byte ledPin = 13;
boolean blink = false;
boolean ledPin_state;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = millis();
const long interval = 300;

int debug = 0; // set to 1 to show debug info on serial port - may cause issues with DCC++ depending on what is sent

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(buttonPin, INPUT);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  //  pinMode(Trigger1, INPUT);
  //  pinMode(Trigger2, INPUT);
  //  digitalWrite(Trigger1, HIGH);// turn on pullup resistors
  //  digitalWrite(Trigger2, HIGH);// turn on pullup resistors
  // randomSeed(analogRead(0));
  pinMode(LED, OUTPUT);
  lcd.begin (16, 2); //  LCD is 16 characters x 2 lines
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);  // Switch on the backlight
  lcd.home (); // go home
  Serial.begin (115200);
  getAddresses();  // read eeprom
  lcd.setCursor(0, 0);
  lcd.print("Hold button to");
  lcd.setCursor(0, 1);
  lcd.print("vary MaxLocos ");
  lcd.print(maxLocos);
  delay(1000);
  buttonState = digitalRead(buttonPin);
  // Serial.println(buttonState);
  if (buttonState == LOW) {
    getNumberOfLocos();
    //    do {  // routine to stay here till button released & not toggle direction
    //      buttonState = digitalRead(buttonPin);
    //    }      while (buttonState == LOW);
  }

  lcd.clear();
  lcd.print("DCC++ Throttle");
  lcd.setCursor(0, 1);
  lcd.print("6-11-16 v");
  for (int i = 0; i < 4; i++) {
    lcd.print(VersionNum[i]);
  }

  Serial.print("6-11-2016  version ");
  for (int i = 0; i < 4; i++) {
    Serial.print(VersionNum[i]);
  }
  if (debug == 1) Serial.println("");
  Serial.print("<0>");// power off to DCC++ unit
  delay(1500);
  pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
  digitalWrite(ledPin, HIGH);           // Turn the LED on.
  ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  lcd.clear();
  doMainLCD();
}  // END SETUP

void getNumberOfLocos() {
  //maxLocos = 4;// number of loco addresses
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loco # now = ");
  lcd.print(maxLocos);
  lcd.setCursor(0, 1);
  lcd.print("new # (1-4) ");
  do {
    key = keypad.getKey();
    if (debug == 1) Serial.print("key = ");
    if (debug == 1) Serial.println(key);
    key = key - 48 - 1; // convert from ascii value
    //Serial.println(key);
    if (key == 1 || key == 2 || key == 3 || key == 4) { // 1-4
      maxLocos = key + 1;
      if (debug == 1)    Serial.print("new maxLocos = ");
      if (debug == 1)  Serial.println(maxLocos);
      lcd.print(maxLocos);
      delay(1400);
      saveAddresses();
      break;
    }
  } while (key != 1 || key != 2 || key != 3 || key != 4);
}


void loop() {
  /*  // thie routine refreshes DCC commands ever "interval" milliseconds
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
    // save the last time
    previousMillis = currentMillis;
    doDCC();
    }
  */
  key = keypad.getKey();
  if (key == 42) { // *
    all2ZeroSpeeed();
    getLocoAddress();
    key = 0;
  }
  //NEW ROUTINE USING ROTARY ENCODER FOR SPEED AND BUTTON ON IT FOR DIRECTION
  getEncoder();
  if (encoderChange == 1) {
    encoderChange = 0;
    LocoSpeed[ActiveAddress] = encoderPos;
    doDCC();
    doMainLCD();
  }
  buttonState = digitalRead(buttonPin);
  // Serial.println(buttonState);
  if (buttonState == LOW) {
    delay(50);
    buttonState = digitalRead(buttonPin); // check a 2nd time to be sure
    if (buttonState == LOW) {// check a 2nd time to be sure
      LocoDirection[ActiveAddress] = !LocoDirection[ActiveAddress];
      if (LocoDirection[ActiveAddress] == 0) {
        digitalWrite(ledPin, LOW);
      }
      else digitalWrite(ledPin, HIGH);
      doDCC();
      doMainLCD();
      do {  // routine to stay here till button released & not toggle direction
        buttonState = digitalRead(buttonPin);
      }      while (buttonState == LOW);
    }
  }
  if (key == 35) { // #
    ActiveAddress++;
    if (ActiveAddress >= maxLocos) ActiveAddress = 0;
    doMainLCD();
    delay(200);
    key = 0;
    encoderPos = LocoSpeed[ActiveAddress];
    doDCC();
  }
  if (key != 42 && key != 35 && key >= 40) {
    doFunction();
  }
}  //END LOOP

void getEncoder() {
  if (oldEncPos != encoderPos) {
    encoderPos = constrain(encoderPos, 0, 126);
    oldEncPos = encoderPos;
    encoderChange = 1; // flag to show a change took place
  }
}//END GET ENCODER ROUTINE

//START DO FUNCTION BUTTONS
int doFunction() {
  key = key - 48 - 1; // convert from ascii value
  lcd.setCursor (14, 1);       // go to end of 2nd line
  ///  lcd.print("FN code ");
  lcd.print(key, DEC + 1);
  if (debug == 1) Serial.print("got a keypad button ");
  if (debug == 1) Serial.println(key, DEC);
  if (key <= 4) {
    if (bitRead(LocoFN0to4[ActiveAddress], key) == 0 ) {
      bitWrite(LocoFN0to4[ActiveAddress], key, 1);
    }
    else {
      if (bitRead(LocoFN0to4[ActiveAddress], key) == 1 ) {
        bitWrite(LocoFN0to4[ActiveAddress], key, 0);
      }
    }
    doDCCfunction04();
    Serial.print(LocoFN0to4[ActiveAddress], BIN);
    if (debug == 1) Serial.println(" LocoFN0to4[ActiveAddress] d ");
    if (debug == 1) Serial.print(LocoFN0to4[ActiveAddress], DEC);
    if (debug == 1) Serial.println(" LocoFN0to4[ActiveAddress]");
  }
  if (key >= 5 && key <= 8) {
    key = key - 5;
    if (bitRead(LocoFN5to8[ActiveAddress], key) == 0 ) {
      bitWrite(LocoFN5to8[ActiveAddress], key, 1);
    }
    else {
      if (bitRead(LocoFN5to8[ActiveAddress], key) == 1 ) {
        bitWrite(LocoFN5to8[ActiveAddress], key, 0);
      }
    }
    doDCCfunction58();
    if (debug == 1) Serial.print(LocoFN5to8[ActiveAddress], BIN);
    if (debug == 1) Serial.println(" LocoFN5to8[ActiveAddress] d ");
    if (debug == 1) Serial.print(LocoFN5to8[ActiveAddress], DEC);
    if (debug == 1) Serial.println(" LocoFN5to8[ActiveAddress]");
  }
  if (key == -1)
  {
    lcd.setCursor (14, 1);       // go to end of 2nd line
    ///    lcd.print("FN code ");
    lcd.print(key, DEC);
    key = 0;
    LocoFN0to4[ActiveAddress] = B00000000; //clear variables for which functions are set
    LocoFN5to8[ActiveAddress] = B00000000;
    doDCCfunction04();
    doDCCfunction58();
    delay(500);
    key = 0;
  }
  key = 0;
  // delay(500);
  doMainLCD();
}//END DO FUNCTION BUTTONS

void showFirstLine() {
  // break;
  if (debug == 1) Serial.println(" ");
  lcd.setCursor(0, 0);
  lcd.print("                ");// clear
  lcd.setCursor(0, 0);
  lcd.print("L");
  lcd.print(ActiveAddress + 1);
  lcd.print(" = ");
  lcd.print(LocoAddress[ActiveAddress]);

  for (int zzz = 0; zzz <= 3; zzz++) {
    if (debug == 1) Serial.print("add # ");
    if (debug == 1) Serial.print(zzz + 1);
    if (debug == 1) Serial.print(" ");
    if (debug == 1) Serial.println(LocoAddress[zzz]);
  }
}

void getLocoAddress() {
  int saveAddress =   LocoAddress[ActiveAddress];
  Serial.print("<0>");// power off to DCC++ unit
  int total = 0;
  counter = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Dcc Addr # ");
  lcd.print(ActiveAddress + 1);
  if (debug == 1) Serial.println(" @ top");
  do {
    //  TrigVal2 = digitalRead(Trigger2);   // read the input pin
    //  if (TrigVal2 == 0) break;
    key = keypad.getKey();
    if (key == '#' | key == '*') { //abort when either is hit
      //LocoAddress[ActiveAddress] = saveAddress;
      total = saveAddress;
      break;// exit routine if # button pressed - ABORT new address
    }
    if (key) {
      counter++;
      int number =  key - 48;
      total = total * 10 + number;
      if (debug == 1) Serial.print("TOTAL = ");
      if (debug == 1) Serial.println(total);
      lcd.setCursor(0, 1);
      if (total == 0) {   // print multiple zeros for leading zero number
        for (int tempx = 1; tempx <= counter; tempx++) {
          lcd.print("0");
        }
      }
      else lcd.print(total);
      if (debug == 1) Serial.print("Counter = ");
      if (debug == 1) Serial.print(counter);
      if (debug == 1) Serial.print("  key = ");
      if (debug == 1) Serial.print(key);
      if (debug == 1) Serial.print("   val = ");
      if (debug == 1) Serial.println(number);
    }
  }
  while (counter <= 3); //  collect exactly 4 digits
  LocoAddress[ActiveAddress] = total;
  saveAddresses();
  lcd.clear();
  doMainLCD();
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      if (key == '#') {
        digitalWrite(ledPin, !digitalRead(ledPin));
        ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
      }
      break;
    case RELEASED:
      if (key == '*') {
        digitalWrite(ledPin, ledPin_state);   // Restore LED state from before it started blinking.
        blink = false;
      }
      break;
    case HOLD:
      if (key == '*') {
        blink = true;    // Blink the LED when holding the * key.
      }
      break;
  }
}

void doDCC() {
  if (debug == 1) Serial.println(LocoDirection[ActiveAddress] );
  Serial.print("<1>");
  Serial.print("<t1 ");
  Serial.print(LocoAddress[ActiveAddress] );//locoID);
  Serial.print(" ");
  Serial.print(LocoSpeed[ActiveAddress] );
  Serial.print(" ");
  Serial.print(LocoDirection[ActiveAddress] );
  Serial.println(">");
}

void doDCCfunction04() {
  Serial.write("<f ");
  Serial.print(LocoAddress[ActiveAddress] );
  Serial.print(" ");
  int fx = LocoFN0to4[ActiveAddress] + 128;
  Serial.print(fx);
  Serial.print(" >");
}

void doDCCfunction58() {
  Serial.write("<f ");
  Serial.print(LocoAddress[ActiveAddress] );
  Serial.print(" ");
  int fx = LocoFN5to8[ActiveAddress] + 176;
  Serial.print(fx);
  Serial.print(" >");
}

void all2ZeroSpeeed() {  // set flag to 1 to stop, set to 0 to restore
  for (int tempx = 0; tempx <= maxLocos; tempx++) {
    Serial.print("<t1 ");
    Serial.print(LocoAddress[tempx] );//locoID);
    Serial.print(" ");
    if (ZeroSpeedFlag == 1) {
      Serial.print(0);//LocoSpeed[0] );
    }
    else Serial.print(LocoSpeed[0] );
    Serial.print(" ");
    Serial.print(LocoDirection[1] );
    Serial.write(">");
  }
}

void getAddresses() {
  int xxx = 0;
  for (int xyz = 0; xyz <= maxLocos - 1; xyz++) {
    LocoAddress[xyz] = EEPROM.read(xyz * 2) * 256;
    LocoAddress[xyz] = LocoAddress[xyz] + EEPROM.read(xyz * 2 + 1);
    if (LocoAddress[xyz] >= 10000) LocoAddress[xyz] = 3;
    if (debug == 1) Serial.println(" ");
    if (debug == 1) Serial.print("loco = ");
    if (debug == 1) Serial.print(LocoAddress[xyz]);
    if (debug == 1) Serial.print("  address# = ");
    if (debug == 1) Serial.print(xyz + 1);
  }
  if (debug == 1) Serial.println(" ");
  maxLocos = EEPROM.read(20);
  if (debug == 1) Serial.print("EEPROM maxLocos = ");
  if (debug == 1) Serial.println(maxLocos);
  if (maxLocos >= 4) maxLocos = 4;
}

void saveAddresses() {
  int xxx = 0;
  for (int xyz = 0; xyz <= maxLocos - 1; xyz++) {
    xxx = LocoAddress[xyz] / 256;
    if (debug == 1) Serial.println(" ");
    if (debug == 1) Serial.print("loco = ");
    if (debug == 1) Serial.print(LocoAddress[xyz]);
    if (debug == 1) Serial.print("  address# = ");
    if (debug == 1) Serial.print(xyz);
    if (debug == 1) Serial.print(" msb ");
    if (debug == 1) Serial.print(xxx);
    if (debug == 1) Serial.print(" writing to ");
    if (debug == 1) Serial.print(xyz * 2);
    if (debug == 1) Serial.print(" and ");
    if (debug == 1) Serial.print(xyz * 2 + 1);
    EEPROM.write(xyz * 2, xxx);
    xxx = LocoAddress[xyz] - (xxx * 256);
    if (debug == 1) Serial.print(" lsb ");
    if (debug == 1) Serial.print(xxx);
    EEPROM.write(xyz * 2 + 1, xxx);
  }
  EEPROM.write(20, maxLocos);
}

void doMainLCD() {
  lcd.setCursor(0, 0);
  lcd.print("S=");
  lcd.print(LocoSpeed[ActiveAddress], DEC);
  lcd.print(" ");
  lcd.setCursor(5 , 0);
  if (LocoDirection[ActiveAddress] == 1 ) {
    lcd.print(" > ");
  }
  else {
    lcd.print(" < ");
  }
  lcd.setCursor(8, 0);
  lcd.print("L=");
  if (LocoAddress[ActiveAddress] <= 9) {
    lcd.print("0");  // add leading zero for single digit addresses
  }
  lcd.print(LocoAddress[ActiveAddress] , DEC);
  lcd.print("   ");
  lcd.setCursor(14, 0);
  lcd.print("#");
  lcd.setCursor (15, 0);       // go to end of 1st line
  lcd.print(ActiveAddress + 1);
  lcd.setCursor(5, 1); // start of 2nd line
  String temp = "0000" + String(LocoFN0to4[ActiveAddress], BIN);  // pad with leading zeros
  int tlen = temp.length() - 5;
  lcd.print(temp.substring(tlen));
  temp = "000" + String(LocoFN5to8[ActiveAddress], BIN);
  tlen = temp.length() - 4;
  lcd.setCursor(0, 1); // start of 2nd line
  lcd.print(temp.substring(tlen));
}

void PinA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    if (encoderPos != 0)encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
