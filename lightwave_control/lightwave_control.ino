#include <LightwaveRF.h>
#include "commands.h"

char commandString[6] = "";
char inputString[6] = "";
byte inputStringPos = 0;

boolean processCommand = false; 
boolean newCommand = false;
byte msg[10];
byte len = 10;
byte lamp;
byte commandType;
byte repeatSend = 12;
byte repeatCounter = 0;
byte ledPin = 13;

void flashLed() {
  for(int i=0; i<5; i++) {
    digitalWrite(13, HIGH);
    delay(5);
    digitalWrite(13, LOW);
    delay(5);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lw_setup(3, 2, 0);
}

void loop() {
//  lw_rx_wait();
//  lw_get_message(msg,&len);
//  printMsg(msg, len);

  if (newCommand) {
    repeatCounter = 0;
    parseInput(commandString, &commandType, &lamp);
    makeCommand(commandType, lamp, 1, msg);
    newCommand = false;
    processCommand = true;
  }

  if (processCommand) {
    set_delays(commandType);
    lw_send(msg);
    
    repeatCounter++;
    if (repeatCounter >= repeatSend || commandType == BRIGHT_DOWN || commandType == BRIGHT_UP) {
      repeatCounter = 0;
      processCommand = false;
    }
  }
  
  processSerial();
}

void processSerial() {
  while (Serial.available()) { 
    char inChar = (char)Serial.read(); 
    
    if (inChar == '\n') {
      memcpy(commandString, inputString, 6);
      
      inputStringPos = 0;
      newCommand = true;
      processCommand = false;
      flashLed();
      return;
    } 
    inputString[inputStringPos] = inChar;
    inputStringPos++;
  }
}

void printMsg(byte *msg, byte len) {
  for(int i=0; i<len; i++) {
    Serial.print(msg[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
