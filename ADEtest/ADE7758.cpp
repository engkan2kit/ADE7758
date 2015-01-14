#include "Arduino.h"
#include "spi.h"
#include "software_spi.h"
#include "ADE7758.h"
#include <avr/wdt.h>
extern void isrGDO0event(void);
//public
ADE7758::ADE7758(int SSpin):
    _SSpin(SSpin)
{
	
  	digitalWrite(_SSpin, HIGH);
    //Ensure Chip select pin is output
 	pinMode(_SSpin, OUTPUT);
}

void ADE7758::begin()
{
	//normal mode
	//write8bits(OPMODE, 0x44);
        spi_setup();
        write8bits(OPMODE, 0x05);
    
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
  for(i=0;i<10;++i){
          volts+=getVRMS(phase);
          delayMicroseconds(50);
  }
  //average
  return volts/10;
}

long ADE7758::IRMS(char phase)
{
  char i=0;
  long current=0;
  getIRMS(phase);//Ignore first reading
  for(i=0;i<10;++i){
    current+=getIRMS(phase);
    delayMicroseconds(50);
  }
  //average
  return current/10;
}

void accumulateEnergy()
{
    
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


int ADE7758::lineFreq(char phase){
    uint8_t mmode;
    mmode = read8bits(MMODE);
    write8bits(MMODE,(mmode&0x11111100 )| phase);
    delay(10);
    return read16bits(FREQ);
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
        
    delay(10);
    spi_transfer(REG_WRITE(reg));
    delay(2);

    spi_transfer(data);

    delay(1);
    
    disableChip();
}

void ADE7758::write16bits(char reg, unsigned int data)
{
    enableChip();
    
    delay(10);
    spi_transfer(REG_WRITE(reg));
    delay(2);
    spi_transfer((unsigned char)((data>>8)&0xFF));
    delay(2);
    spi_transfer((unsigned char)(data&0xFF));
    delay(1);

    disableChip();
}

unsigned char ADE7758::read8bits(char reg)
{
    enableChip();
    
    unsigned char ret;
    delay(10);
    spi_transfer(REG_READ(reg));
    delay(2);
    ret=spi_transfer(0x00);
    delay(1);
    
    disableChip();

    return ret;
}

unsigned int ADE7758::read16bits(char reg)
{
    enableChip();
    unsigned int ret=0;
    unsigned char ret0=0;
    delay(10);
    spi_transfer(REG_READ(reg));
    delay(2);
    ret=spi_transfer(0x00);
    delay(2);
    ret0=spi_transfer(0x00);
    delay(1);
    
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
    delay(10);
    spi_transfer(REG_READ(reg));
    delay(2);
    ret=spi_transfer(0x00);
    delay(2);
    ret1=spi_transfer(0x00);
    delay(2);
    ret0=spi_transfer(0x00);
    delay(1);
    
    disableChip();
    ret= (ret<<16)|(ret1<<8)|ret0;
    return ret;
}

long ADE7758::getIRMS(char phase)
{
    return read24bits(AIRMS+phase);
}

long ADE7758::getVRMS(char phase)
{          
    return read24bits(AVRMS+phase);
}
	
ADE7758 ADE(ADE_SS);
