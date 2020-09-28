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




#define camera 5


#define redLedPin 3
#define greenLedPin 5

long long lastScan;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);

  

  Ethernet.begin(mac,clientIp);

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(switcherIp);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();



  lastScan = millis();

  delay(500);

  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  Serial.println("Done.");
  Serial.print("\n\n");
}





void loop(){


  // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();


  if(AtemSwitcher.getProgramTally(camera)){
    digitalWrite(redLedPin, HIGH);
  }
  else{
    digitalWrite(redLedPin, LOW);
  }

  if(AtemSwitcher.getPreviewTally(camera)){
    digitalWrite(greenLedPin, HIGH);
  }
  else{
    digitalWrite(greenLedPin, LOW);
  }


  if(millis() - lastScan > 500){
    lastScan = millis();

    Serial.print("Preview: ");
    Serial.print(AtemSwitcher.getPreviewInput(camera));
    Serial.print(" Program: ");
    Serial.println(AtemSwitcher.getProgramInput(camera));
  }
  
}

