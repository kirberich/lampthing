#include <LightwaveRF.h>
#include "commands.h"

char commandString[6] = "";
char inputString[6] = "";
byte inputStringPos = 0;

boolean stringComplete = false; 
byte msg[10];
byte len = 10;
byte lamp;
byte commandType;
byte repeatSend = 12;
byte repeatCounter = 0;

void setup() {
  Serial.begin(9600);
  lw_setup(3, 2, 0);
}

void loop() {
//  lw_rx_wait();
//  lw_get_message(msg,&len);
//  printMsg(msg, len);

  if (stringComplete) {
    if (repeatCounter == 0) {
      //Serial.println(commandString);
      parseInput(commandString, &commandType, &lamp);
      makeCommand(commandType, lamp, 1, msg);
    }
//    printMsg(msg, len);
    lw_send(msg);
    
    repeatCounter++;
    if (repeatCounter >= repeatSend) {
//      commandString = String("");
//      inputString = String("");
      repeatCounter = 0;
      stringComplete = false;
    }
  }
}

void serialEvent() {
  // Handle serial input
  
  while (Serial.available()) { 
    char inChar = (char)Serial.read(); 
    
    if (inChar == '\n') {
      memcpy(commandString, inputString, 6);
      
      inputStringPos = 0;
      repeatCounter = 0;
      stringComplete = true;
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
