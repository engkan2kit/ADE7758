#include "Arduino.h"
#include <SPI.h>
#include "ADE7758.h"
#include <avr/wdt.h>

//public
ADE7758::ADE7758(SPIClass ADE7758SPI, int SSpin):
    _ADE7758SPI(ADE7758SPI),
    _SSpin(SSpin)
{
	
  	digitalWrite(_SSpin, HIGH);
    //Ensure Chip select pin is output
 	pinMode(_SSpin, OUTPUT);
}

void ADE7758::begin()
{
	//normal mode
	write8bits(OPMODE, 0x44);

}

int ADE7758::getWattHR(char phase)
{
	return read16bits(AWATTHR+phase);
}

int ADE7758::getVARHR(char phase)
{
	return read16bits(AVARHR+phase);
}

int ADE7758::getVAHR(char phase)
{
	return read16bits(AVAHR+phase);
}

long ADE7758::VRMS(char phase)
{
  char i=0;
  long volts=0;
  getVRMS(phase);//Ignore first reading
  for(i=0;i<100;++i){
          volts+=getVRMS(phase);
  }
  //average
  return volts/100;
}

long ADE7758::IRMS(char phase)
{
  char i=0;
  long current=0;
  getIRMS(phase);//Ignore first reading
  for(i=0;i<100;++i){
    current+=getIRMS(phase);
  }
  //average
  return current/100;
}

long ADE7758::waveform(char phase,char source)
{

}

void ADE7758::powerOff()
{

}

void ADE7758::powerON()
{

}

void ADE7758::sleep()
{

}

void ADE7758::wakeUp()
{

}

long ADE7758::getInterruptStatus(void){
        return read24bits(STATUS);
}


long ADE7758::getResetInterruptStatus(void){
        return read24bits(RSTATUS);
}



//private

void ADE7758::enableChip()
{
    digitalWrite(_SSpin, LOW);
}

void ADE7758::disableChip()
{
    digitalWrite(_SSpin, HIGH);
}

void ADE7758::write8bits(char reg, unsigned char data)
{
    enableChip();
        
    delayMicroseconds(50);
    _ADE7758SPI.transfer(REG_WRITE(reg));
    delayMicroseconds(50);
    _ADE7758SPI.transfer((unsigned char)data);
    delayMicroseconds(50);
    
    disableChip();
}

void ADE7758::write16bits(char reg, unsigned int data)
{
    enableChip();
    
    delayMicroseconds(50);
    _ADE7758SPI.transfer(REG_WRITE(reg));
    delayMicroseconds(50);
    _ADE7758SPI.transfer((unsigned char)((data>>8)&0xFF));
    delayMicroseconds(50);
    _ADE7758SPI.transfer((unsigned char)(data&0xFF));
    delayMicroseconds(50);

    disableChip();
}

unsigned char ADE7758::read8bits(char reg)
{
    enableChip();
    
    unsigned char ret;
    delayMicroseconds(50);
    _ADE7758SPI.transfer(REG_READ(reg));
    delayMicroseconds(50);
    ret=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    
    disableChip();

    return ret;
}

unsigned int ADE7758::read16bits(char reg)
{
    enableChip();
    unsigned int ret=0;
    unsigned char ret0=0;
    delayMicroseconds(50);
    _ADE7758SPI.transfer(REG_READ(reg));
    delayMicroseconds(50);
    ret=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    ret0=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    
    disableChip();
    ret= (ret<<8)|ret0;
    return ret;
}

unsigned long ADE7758::read24bits(char reg)
{
    enableChip();
    unsigned long ret=0;
    unsigned int ret1=0;
    unsigned char ret0=0;
    delayMicroseconds(50);
    _ADE7758SPI.transfer(REG_READ(reg));
    delayMicroseconds(50);
    ret=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    ret1=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    ret0=_ADE7758SPI.transfer(0x00);
    delayMicroseconds(50);
    
    disableChip();
    ret= (ret<<16)|(ret1<<8)|ret0;
    return ret;
}

long ADE7758::getIRMS(char phase)
{
    long lastupdate = 0;
    ADE7758::getResetInterruptStatus(); // Clear all interrupts
    lastupdate = millis();
    while( !  ( ADE7758::getInterruptStatus() & (ZXA+phase) )  )   // wait Zero-Crossing
    { // wait for the selected interrupt to occur
            if ( ( millis() - lastupdate ) > 100) 
            { 
                    wdt_reset();
                    Serial.println("\n--> getIRMS Timeout - no AC input"); 
                    break;  
            }
    }          
    return read24bits(AIRMS+phase);
}

long ADE7758::getVRMS(char phase)
{
    long lastupdate = 0;
    ADE7758::getResetInterruptStatus(); // Clear all interrupts
    lastupdate = millis();
    while( !  ( ADE7758::getInterruptStatus() & (ZXA+phase) )  )   // wait Zero-Crossing
    { // wait for the selected interrupt to occur
            if ( ( millis() - lastupdate ) > 100) 
            { 
                    wdt_reset();
                    Serial.println("\n--> getIRMS Timeout - no AC input"); 
                    break;  
            }
    }          
    return read24bits(AVRMS+phase);
}
	
