#include <LightwaveRF.h>
#include "commands.h"

unsigned long old_time;
unsigned long new_time;
byte on_command[10];
byte off_command[10];
int on_time = 10000;
int on_time_elapsed = 0;
boolean motion = false;
boolean is_on = false;
boolean keep_on = false;
byte msg[10];
byte len = 10;
int8_t command;

extern byte ids[];

void setup() {
  pinMode(6, INPUT); // Motion detector
  pinMode(13, OUTPUT);
  lw_setup(3, 2, 0);
 
  makeCommand(ON, 12, 1, on_command);
  makeCommand(OFF, 12, 1, off_command);
}

int8_t process_command() {
  if(!lw_get_message(msg, &len)) {
    return -1;
  }
  
  // Check lamp (should be 15/0x6F
  if(msg[2] != 0x6F) {
    return -1;
  }
  
  // Check remote id (allow only remote 0)
  for (byte i=0; i<6; i++) {
    if (msg[i+4] != ids[i]) {
      return -1;
    }
  }
  
  // Check command for on or off
  if (msg[3] == commands[ON]) {
    delay(1000);
    return 1;
  }
  if (msg[3] == commands[OFF]) {
    delay(1000);
    return 0;
  }
  return -1;
}

void loop() {
  motion = digitalRead(6);
  command = process_command();
  if (command == 1) {
    keep_on = true;
  } else if (command == 0) {
    keep_on = false;
    lw_send(off_command);
    is_on = false;
  }
  
  digitalWrite(13, is_on);
  
  if (keep_on) {
    if (!is_on) {
      lw_send(on_command);
      is_on = true;
    }
  } else {
    if (motion) {
      on_time_elapsed = 0;
      
      if (!is_on) {
        lw_send(on_command);
        is_on = true;
      }
    } else {
      if (on_time_elapsed > on_time) {
        if(is_on) {
          lw_send(off_command);
          // Wait for a bit so the detector can settle after the lamp turns on
          delay(2000);
          is_on = false;
        }
      }
    }
  }
  delay(50);
  on_time_elapsed += 50;
}
