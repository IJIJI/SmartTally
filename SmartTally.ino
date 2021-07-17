#include "Arduino.h"
#include "SPI.h"
#include "Ethernet.h"
#include <ATEMbase.h>
#include <ATEMext.h>





#define latchPin 5	  //? Latch pin of shift register
#define clockPin 6	  //? Clock pin of shift register
#define dataPin 4	    //? Data pin of shift register
#define enablePin 3   //? Output enable pin of shift register


//? MAC address and IP address for this *particular* Arduino / Ethernet Shield!
//? The MAC address is printed on a label on the shield or on the back of your device
//? The IP address should be an available address you choose on your subnet where the switcher is also present
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x6B, 0xB9 };       //! SETIP MAC address of the Arduino
IPAddress clientIp(192, 168, 10, 132);        //! SETUP IP address of the Arduino
IPAddress switcherIp(192, 168, 10, 240);      //! SETUP IP address of the ATEM Switcher

//? Include ATEMbase library and make an instance:
//? The port number is chosen randomly among high numbers.

ATEMext AtemSwitcher;


//! SETTINGS

#define camNum 8

uint8_t camNums[] = {1, 2, 3, 4, 5, 6, 7, 8};

bool checkMe[] = {1, 0, 0, 0};

bool simultProgPrev = false;

bool bothProgramOnAuto = true;

//!

struct portTally {
  bool program = false;
  bool preview = false;
  // bool alert = false;
  // long long alertStart = 0;
};


struct portData {
  portTally tally;
  uint8_t input = 1;
};

portData ports[camNum];



struct switcherData {
  String name;
  uint8_t meCount;
  uint8_t auxCount;
  uint8_t inputCount;
  // TODO check if in transition
};

switcherData switcherInfo;

//!

long long lastUpdate;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting...");

  //? Set all the pins of shift register as OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);


// TODO CHECK IF NETWORK WORKS HERE AND IN LOOP
  Ethernet.begin(mac,clientIp);

  //? Initialize a connection to the switcher:
  AtemSwitcher.begin(switcherIp);

  AtemSwitcher.connect();


  switcherInfo.name = AtemSwitcher.getProductIdName();
  switcherInfo.meCount = AtemSwitcher.getTopologyMEs();
  switcherInfo.auxCount = AtemSwitcher.getTopologyAUXbusses();
  switcherInfo.inputCount = AtemSwitcher.getTopologySources();

  Serial.print("Switcher: ");
  Serial.println(switcherInfo.name);
  Serial.print(" Sources: ");
  Serial.print(switcherInfo.inputCount);
  Serial.print(" ME's: ");
  Serial.print(switcherInfo.meCount);
  Serial.print(" Auxes: ");
  Serial.println(switcherInfo.auxCount);


  for (uint8_t x = 0; x < camNum; x++){
    ports[x].input = camNums[x];
  }



  shiftOut(dataPin, clockPin, LSBFIRST, B10101010);
  shiftOut(dataPin, clockPin, LSBFIRST, B10101010);
  delay(500);
  shiftOut(dataPin, clockPin, LSBFIRST, B01010101);
  shiftOut(dataPin, clockPin, LSBFIRST, B01010101);
  delay(500);
  shiftOut(dataPin, clockPin, LSBFIRST, 255);
  shiftOut(dataPin, clockPin, LSBFIRST, 255);
  delay(1000);
  shiftOut(dataPin, clockPin, LSBFIRST, 0);
  shiftOut(dataPin, clockPin, LSBFIRST, 0);

  Serial.println("Done.");
  Serial.print("\n\n");
}





void loop(){


  // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();

  if(millis() - lastUpdate > 100){
    tallyClearMem();


    for (int h = 0; h < switcherInfo.meCount; h++){
      uint8_t program = AtemSwitcher.getProgramInputVideoSource(h);
      uint8_t preview = AtemSwitcher.getPreviewInputVideoSource(h);

      tallySetMem(program, preview);
    }
   
    tallyUpdatePorts();
    lastUpdate = millis();
  }


  
}


void tallyClearMem(){
  tallyClearMemProg();
  tallyClearMemPrev();
  
}

void tallyClearMemProg(){
  for (uint8_t x = 1; x <= camNum; x++){
    ports[x-1].tally.program = false;
  }
}

void tallyClearMemPrev(){
  for (uint8_t x = 1; x <= camNum; x++){
    ports[x-1].tally.preview = false;
  }
}

void tallySetMem(uint8_t program, uint8_t preview){
  tallySetMemProgram(program);
  tallySetMemPreview(preview);
}

void tallySetMemProgram(uint8_t camera){
  for (uint8_t x = 1; x <= camNum; x++){

    if(ports[x-1].input == camera){
      ports[x-1].tally.program = true;
    }
  }
}

void tallySetMemPreview(uint8_t camera){
  for (uint8_t x = 1; x <= camNum; x++){

    if(ports[x-1].input == camera){
      ports[x-1].tally.preview = true;
    }
  }
}

void tallyUpdatePorts() {

  int i = 0;
  byte shiftData = 0;
  for (int x = 0; x < camNum; x++){
    
    if(ports[x].tally.program){
      bitWrite(shiftData, i, 1);
    }
    i++;
    if(ports[x].tally.preview){
      if (!ports[x].tally.program || simultProgPrev){
        bitWrite(shiftData, i, 1);
      }      
    }
    i++;


    if (i >= 8){
      shiftOut(dataPin, clockPin, LSBFIRST, shiftData);
      i = 0;
      shiftData = 0;
    }
  }
  


}




bool dataCollect(){
  // for (int x = 0; x < camNum; x++){
  //   cameras[x].program = AtemSwitcher.getProgramTally(x+1);
  //   cameras[x].preview = AtemSwitcher.getPreviewTally(x+1);
  // }

  return true;
}

void applyData(){

}



