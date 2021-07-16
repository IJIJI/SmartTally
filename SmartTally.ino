#include "Arduino.h"
#include "SPI.h"
#include "Ethernet.h"
#include <ATEMbase.h>
#include <ATEMstd.h>



// MAC address and IP address for this *particular* Arduino / Ethernet Shield!
// The MAC address is printed on a label on the shield or on the back of your device
// The IP address should be an available address you choose on your subnet where the switcher is also present
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x6B, 0xB9 };      // <= SETUP!  MAC address of the Arduino
IPAddress clientIp(192, 168, 10, 132);        // <= SETUP!  IP address of the Arduino
IPAddress switcherIp(192, 168, 10, 240);     // <= SETUP!  IP address of the ATEM Switcher

// Include ATEMbase library and make an instance:
// The port number is chosen randomly among high numbers.

ATEMstd AtemSwitcher;



#define camNum 8

struct camData {
  bool program = false;
  bool preview = false;
  bool alert = false;
};

// struct camData cameras[camNum];

struct dataPackage {
  struct camData cam1;
  struct camData cam2;
  struct camData cam3;
  struct camData cam4;
  struct camData cam5;
  struct camData cam6;
  struct camData cam7;
  struct camData cam8;
};

struct dataPackage cameras;

long long lastSend;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting...");

  

  Ethernet.begin(mac,clientIp);

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(switcherIp);
  // AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();



  lastSend = millis();

  delay(500);


  Serial.println("Done.");
  Serial.print("\n\n");
}





void loop(){


  // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();

  if(dataCollect() || millis() - lastSend > 1000){
    applyData();
  }


  
}

bool dataCollect(){
  for (int x = 0; x < camNum; x++){
    cameras[x].program = AtemSwitcher.getProgramTally(x+1);
    cameras[x].preview = AtemSwitcher.getPreviewTally(x+1);
  }

  return true;
}

void applyData(){

}




// if(AtemSwitcher.getProgramTally(camera)){
//   digitalWrite(redLedPin1, HIGH);
//   digitalWrite(greenLedPin1, LOW);

// }
// else if(AtemSwitcher.getPreviewTally(camera)){
//   digitalWrite(greenLedPin1, HIGH);
//   digitalWrite(redLedPin1, LOW);

// }
// else{
//   digitalWrite(greenLedPin1, LOW);
//   digitalWrite(redLedPin1, LOW);

// }



// if(millis() - lastScan > 500){
//   lastScan = millis();

//   Serial.print("Program: ");
//   Serial.print(AtemSwitcher.getProgramInput());
//   Serial.print(" Preview: ");
//   Serial.print(AtemSwitcher.getPreviewInput());
//   Serial.print(" FTB: ");
//   Serial.println(AtemSwitcher.getFadeToBlackFrameCount());

  
// }