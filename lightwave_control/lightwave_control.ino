#include <LightwaveRF.h>
#include "commands.h"

char commandString[8] = "";
char inputString[8] = "";
byte inputStringPos = 0;

boolean processCommand = false; 
boolean newCommand = false;
byte msg[10];
byte len = 10;
byte lamp;
byte remote;
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

  if (processCommand) {
    set_delays(commandType);
    makeCommand(commandType, lamp, remote, msg);
    lw_send(msg);
    repeatCounter++;
    if (repeatCounter >= repeatSend || commandType == BRIGHT_DOWN || commandType == BRIGHT_UP) {
      repeatCounter = 0;
      processCommand = false;
    }
  }
  
  process_serial();
}

void process_serial() {
  if (Serial.available()) {
    // 'c' starts a command, in the form of 'c00 00 00'
    if (Serial.read() == 'c') {
      commandType = Serial.parseInt();
      lamp = Serial.parseInt();
      remote = Serial.parseInt();
      repeatCounter = 0;
      processCommand = true;
      Serial.println("set");
    }
  }
}

void printMsg(byte *msg, byte len) {
  for(int i=0; i<len; i++) {
    Serial.print(msg[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
