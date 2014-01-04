#include "LightwaveRF.h"

byte on[] = {0xf6,0xf6,0xf6,0xee,0x6f,0xeb,0xbe,0xed,0xb7,0x7b};
byte off[] = {0xf6,0xf6,0xf6,0xf6,0x6f,0xeb,0xbe,0xed,0xb7,0x7b};

void setup() {
  Serial.begin(57600);
  lw_tx_setup(8);
}

void loop() {
  if(Serial.available() > 0) {
    char c = Serial.read();
    if (c == '1') {
      turnOn();
    }
    if (c == '0') {
      turnOff();
    }
  }
}

void turnOn() {
  Serial.println("Turning on");
  lw_send(on);
}

void turnOff() {
  Serial.println("Turning off");
  lw_send(off);
}
