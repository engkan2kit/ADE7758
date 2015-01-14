#include "Arduino.h"

#include "spi.h"
#include "ADE7758.h"
#include <avr/wdt.h>
char inputChar;         // a string to hold incoming data
boolean rx = false;  // whether the string is complete
uint8_t mmode;
void setup()
{
  Serial.begin(9600);
  ADE.begin();
  Serial.println("ADE7758 begins operating");
  
  delay(1000);
}


void loop()
{
  
  if (rx) {
    switch(inputChar){
      case 'b':
              Serial.println("CURRENT RMS readings for each phase:");
              Serial.println(ADE.IRMS(PHASE_A), DEC);

              break;
      case 'c':
              Serial.println("VOLTAGE RMS readings for each phase:");
              Serial.println(ADE.VRMS(PHASE_A), DEC);

              break;
              
      case 'z':
              Serial.println("SPI Test");
              Serial.println( ADE.read8bits(OPMODE),DEC);
              break;
      case 'q':
              Serial.println("SPI Test");
              Serial.println( ADE.read8bits(0),DEC);
              break;
      case 'f':
              Serial.println("Freq phase A");

              mmode = ADE.read8bits(MMODE);
              ADE.write8bits(MMODE,(mmode&0x11111100 ));
              delay(10);
              Serial.println( ADE.lineFreq(0),DEC);
              break;
      case 'g':
              Serial.println("Freq phase B");
              
              mmode = ADE.read8bits(MMODE);
              ADE.write8bits(MMODE,(mmode&0x11111100 )| 0x01);
              delay(10);
              Serial.println( ADE.lineFreq(1),DEC);
              break;
      case 'h':
              Serial.println("Freq phase C");
              Serial.println( ADE.lineFreq(2),DEC);
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
    inputChar = (char)Serial.read(); 
    rx = true;
  }
}
