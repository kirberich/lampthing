// Observed lightwave command bytes

// Command format:
// Byte1  Byte2  Byte3   Byte4   Byte5-10
// Level1 Level2 Channel Command Remote ID

byte levels1[] = {0xDE, 0xF6, 0x7D, 0xB7, 0x7E, 0xBD, 0xBD, 0xBD}; // OFF, ON, ALL OFF, DIM DOWN, DIM UP, MOOD1, MOOD2, MOOD3
byte levels2[] = {0xF6, 0xF6, 0xF6, 0xF6, 0x6F, 0xED, 0xEB, 0xDE}; // OFF, ON, ALL OFF, DIM DOWN, DIM UP, MOOD1, MOOD2, MOOD3

// Mood remote uses Group1 0-3 for its 4 different switch settings rather than the first of each group
byte lamps[ ] = {         // == channel
  0xF6, 0xEE, 0xED, 0xEB, // Group 1 0-3
  0xDE, 0xDD, 0xDB, 0xBE, // Group 2 4-7
  0xBD, 0xBB, 0xB7, 0x7E, // Group 3 8-11
  0x7D, 0x7B, 0x77, 0x6F, // Group 4 12-15
  0x6F,                   // ALL OFF 16
  0x6F                    // MOOD    17
}; 

byte commands[] = {0xF6, 0xEE, 0xF6, 0xF6, 0xEE, 0xED, 0xEd, 0xED}; // OFF, ON, ALL OFF, DIM DOWN, DIM UP, MOOD1, MOOD2, MOOD3

byte ids[] = {
  0x6F, 0xEB, 0xEE, 0xB7, 0xB7, 0xBE,  // Remote 1
  0x6F, 0xEB, 0xF6, 0xB7, 0xEB, 0xBE,  // Remote 2
  0x6F, 0xEE, 0xEE, 0xEE, 0xF6, 0x7E   // Mood remote
};

byte dimLevels[] = {
  1111 0110,
  1110 1110,
  1110 1101,
  1110 1011,
  1101 1110,
  1101 1101,
  1101 1011,
  1011 1110,
  1011 1101,
  1011 1011,
  1011 0111,
  0111 1110,
  0111 1101,
  0111 1011,
  0111 0111,
  0110 1111
};

// CONSTANTS
byte OFF = 0;
byte ON = 1;
byte ALL_OFF = 2;
byte BRIGHT_DOWN = 3;
byte BRIGHT_UP = 4;
byte MOOD1 = 5;
byte MOOD2 = 6;
byte MOOD3 = 7;

void makeCommand(byte type, byte lamp, byte remote_num, byte* command) {
  byte brightnessLevel = 0;
  if (type >= 32) {
    brightnessLevel = type - 32;
    type -= 32;
    // Set type (level1, 2 and command bytes)
    command[0] = 0xBD;
    command[1] = dimLevels[brightnessLevel];
    command[3] = commands[1];
  } else {
    // Set type (level1, 2 and command bytes)
    command[0] = levels1[type];
    command[1] = levels2[type];
    command[3] = commands[type];
  }
  
  // Set lamp
  command[2] = lamps[lamp];

  // Set remote id
  memcpy(&command[4], &ids[remote_num*6], 6);
}

void parseInput(String inputString, byte* commandType, byte* lamp) {
  // Input is expected in the form of CC LL, with a byte for the command (see CONSTANTS) and a byte for the lamp

  char commandTypeBuffer[3] = {inputString[0], inputString[1], 0};
  char lampBuffer[3] = {inputString[3], inputString[4], 0};

  *commandType = (byte)atoi(commandTypeBuffer);
  *lamp = (byte)atoi(lampBuffer);
}
