#include <MainTest.h>
#include <string.h>
#include "RFID_Reader.h"
#include "Speaker.h"
#define RFIDResetPin P4_3
#define LED RED_LED
#define G_LED GREEN_LED

const int buttonPin = PUSH2;
int buttonState = 0;

MainTest tester;

void setup(){

Serial.begin(9600);
Serial.println("Beginning integration testing...");

pinMode(RFIDResetPin, OUTPUT);
digitalWrite(RFIDResetPin, HIGH);
pinMode(buttonPin, INPUT_PULLUP);
pinMode(LED, OUTPUT);
digitalWrite(LED, LOW);
pinMode(G_LED, OUTPUT);
digitalWrite(G_LED, LOW);

Serial.println("Starting RFID reader...");

 tester.mainTest();
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
