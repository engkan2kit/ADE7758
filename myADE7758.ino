#include "Arduino.h"
#include <SPI.h>
#include "ADE7758.h"

char inputChar;         // a string to hold incoming data
boolean rx = false;  // whether the string is complete
ADE7758 myADE(SPI,3);
void setup()
{
  Serial.begin(9600);
  
  SPI.begin();
  
  myADE.begin();
  Serial.println("ADE7758 begins operating");
  delay(1000);
}


void loop()
{
  
  if (rx) {
    switch(inputChar){
      case 'a': 
              Serial.println("ENERGY readings for each phase:");
              Serial.println(myADE.getWattHR(PHASE_A), HEX);
              Serial.println(myADE.getWattHR(PHASE_B), HEX);
              Serial.println(myADE.getWattHR(PHASE_C), HEX);
              break;
      case 'b':
              Serial.println("CURRENT RMS readings for each phase:");
              Serial.println(myADE.IRMS(PHASE_A), HEX);
              Serial.println(myADE.IRMS(PHASE_B), HEX);
              Serial.println(myADE.IRMS(PHASE_C), HEX);
              break;
      case 'c':
              Serial.println("VOLTAGE RMS readings for each phase:");
              Serial.println(myADE.VRMS(PHASE_A), HEX);
              Serial.println(myADE.VRMS(PHASE_B), HEX);
              Serial.println(myADE.VRMS(PHASE_C), HEX);
              break;
      case 'd':
              Serial.println("Complex Energy readings for each phase:");
              Serial.println(myADE.getVAHR(PHASE_A), HEX);
              Serial.println(myADE.getVAHR(PHASE_B), HEX);
              Serial.println(myADE.getVAHR(PHASE_C), HEX);
              break;
      default:
          Serial.println("unknown command");
    }
    rx = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    rx = true;
  }
}
