// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define MIDI_PIN        2
#define BUTTON_PIN      4
#define TRANSISTOR_PIN  5

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SoftwareSerial mySerial(2, 3); // RX, TX

boolean writeLine = false;
int lastCC = 0;
boolean status = false;

//boolean pressed = false;
int previous = 0;

void setup() {
  mySerial.begin(31250);
  Serial.begin(9600);
  Serial.println("init");
  
  pinMode(MIDI_PIN, INPUT);
  pinMode(3, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TRANSISTOR_PIN, OUTPUT);
  
   if (digitalRead(BUTTON_PIN) == LOW) {
     previous = 0;
   } else {
     previous = 1;
   }
}

void printMessage() {
  if (status) {
    digitalWrite(TRANSISTOR_PIN, HIGH);   
  } else {
    digitalWrite(TRANSISTOR_PIN, LOW);   
  }
}

void loadValue() {
  byte value = EEPROM.read(lastCC);   
  
  if (value == 0) {
    status = false;
  } else {
    status = true; 
  }
  
  Serial.print("CC");
  Serial.print(lastCC,DEC);
  Serial.print(" - ");
  if (status)   
    Serial.println("ON");
  else
    Serial.println("OFF");  
  
  printMessage();
}

void changeValue() {
  status = !status;
  
  Serial.print("CC");
  Serial.print(lastCC,DEC);
  Serial.print(" - ");
  if (status)   
    Serial.println("ON");
  else
    Serial.println("OFF");  
  
  if (status) {
    EEPROM.write(lastCC,1);
  } else {
    EEPROM.write(lastCC,0);
  }
  
  printMessage();
}

void loop() {
  if (mySerial.available()) {
    writeLine = true;
    /*
    int value = mySerial.read();
    Serial.print(value);
    Serial.print(",");
    */
    int value = mySerial.read();
    //Serial.print(value);
    //Serial.print(",");
    
    if (value == 192) {
      value = mySerial.read();
      //Serial.print(value);
      //Serial.print(",");
      lastCC = value;
      //printMessage(); 
      loadValue();
    }
    
  } else {  
    if (writeLine == true) {
      Serial.println();
      writeLine = false;
    }
  }  
  
  if (digitalRead(BUTTON_PIN) == LOW && previous == 1) {
    changeValue();
    previous = 0;
  }
  
  if (digitalRead(BUTTON_PIN) == HIGH && previous == 0) {
    changeValue();
    previous = 1;
  }
  
    
  
  /*if (digitalRead(BUTTON_PIN) == LOW) {
    if (!pressed) {
      //status = !status;    
      //printMessage();   
      changeValue();
      Serial.print("high");
    }    
    pressed = true;
  } else {
     pressed = false;
  } */
}
