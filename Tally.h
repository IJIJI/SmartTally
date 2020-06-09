/*
  Tally.h - Library for controlling tally lights connected with 74hc595 shift registers.
  Created by IJssel Koster, June 8, 2020.
*/



#ifndef Tally_h
#define Tally_h


#include "Arduino.h"

class Tally
{

  public:
    Tally(int clock, int data, int latch);

    void update();
    void update(int programin, int previewin);

    int Program;
    int Preview;
    
    void startup();
  private:
    int _clock;
    int _data;
    int _latch;
    byte pgmByte = B00001000;
    byte prvByte = B01000000;

};

#endif

