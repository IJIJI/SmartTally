/*
  Tally.h - Library for controlling tally lights connected with 74hc595 shift registers.
  Created by IJssel Koster, June 8, 2020.
*/



#include "Arduino.h"
#include "Tally.h"

Tally::Tally(int clock, int data, int latch)
{
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  _clock = clock;
  _data = data;
  _latch = latch;
}

void Tally::update(){
  byte prgm = B00000000;
  if (Program > 0 && Program <= 8){
    bitWrite(prgm, Program - 1, 1);
  }
  
  
  byte prvw = B00000000;
  if (Preview > 0 && Preview <= 8){
    bitWrite(prvw, Preview - 1, 1);
  }
  
  shiftOut(_data, _clock, LSBFIRST, prvw);
  shiftOut(_data, _clock, LSBFIRST, prgm);
  digitalWrite(_latch, HIGH);
  delayMicroseconds(5);
  digitalWrite(_latch, LOW);
}

void Tally::update(int programin, int previewin){
  
  byte prgm = B00000000;
  if (programin){
    Program = programin;
  }
  
  if (Program > 0 && Program <= 8){
    bitWrite(prgm, Program - 1, 1);
  }
  
  
  
  byte prvw = B00000000;
  if (previewin){
    Preview = previewin;
  }
  if (Preview > 0 && Preview <= 8){
    bitWrite(prvw, Preview - 1, 1);
  }
  shiftOut(_data, _clock, LSBFIRST, prvw);
  shiftOut(_data, _clock, LSBFIRST, prgm);
  digitalWrite(_latch, HIGH);
  delayMicroseconds(5);
  digitalWrite(_latch, LOW);
}


void Tally::startup(){
  byte greenByte = B00000000;
  byte redByte = B00000000;

  shiftOut(_data, _clock, LSBFIRST, greenByte);
  shiftOut(_data, _clock, LSBFIRST, redByte);

  digitalWrite(_latch, HIGH);
  delayMicroseconds(5);
  digitalWrite(_latch, LOW);

  delay(1000);

  for (int x = 0; x < 8; x++){
    bitWrite(redByte, x, 1);

    shiftOut(_data, _clock, LSBFIRST, greenByte);
    shiftOut(_data, _clock, LSBFIRST, redByte);

    digitalWrite(_latch, HIGH);
    delayMicroseconds(5);
    digitalWrite(_latch, LOW);
    
    delay(150);
  }

  for (int x = 0; x < 8; x++){
    bitWrite(greenByte, x, 1);
    bitWrite(redByte, x, 0);

    shiftOut(_data, _clock, LSBFIRST, greenByte);
    shiftOut(_data, _clock, LSBFIRST, redByte);

    digitalWrite(_latch, HIGH);
    delayMicroseconds(5);
    digitalWrite(_latch, LOW);
    
    delay(150);
  }

  for (int x = 0; x < 8; x++){
    bitWrite(redByte, x, 1);

    shiftOut(_data, _clock, LSBFIRST, greenByte);
    shiftOut(_data, _clock, LSBFIRST, redByte);

    digitalWrite(_latch, HIGH);
    delayMicroseconds(5);
    digitalWrite(_latch, LOW);
    
    delay(150);
  }


  delay(1000);
}


