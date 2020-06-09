#include "Arduino.h"
#include "Tally.h"


const int clockPin = 6;
const int dataPin = 4;
const int latchPin = 5;

Tally tally(6, 4, 5); // Define the clock, data and latch pins.

const int prgmPos[] = {1, 6, 2, 8, 3};
const int prvPos[] = {5, 4, 8, 2, 7};

void setup(){
  Serial.begin(9600);
  Serial.println("Starting...");
  tally.startup();
  Serial.println("Done.");
}

void loop(){
  for(int x = 0; x < 5; x++){

    tally.update(prgmPos[x], 0);
    delay(400);
  }

  for(int x = 0; x < 5; x++){

    tally.update(0, prvPos[x]);
    delay(400);
  }
  
}

