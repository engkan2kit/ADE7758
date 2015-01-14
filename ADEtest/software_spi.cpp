#include "software_spi.h"
//#include "adc_board.h"
#include "Arduino.h"
#include "pins.h"
#define SOFTWARE_SPI
#ifdef SOFTWARE_SPI

  static inline void delayCycles(int cycles)
  {
      for (int i=0; i++; i<cycles)
          __asm__("nop\n\t");
         // __nop();
  }
  
  
  #if SPI_MSBFIRST
    unsigned char msk[] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
    #define PCK() (bits[0] << 7 | bits[1] << 6 | bits[2] << 5 | bits[3] << 4 | bits[4] << 3 | bits[5] << 2 | bits[6] << 1 | bits[7])
  #else
    unsigned char msk[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    #define PCK() (bits[0]  | bits[1] << 1 | bits[2] << 2 | bits[3] << 3 | bits[4] << 4 | bits[5] << 5 | bits[6] << 6 | bits[7]<<7)
  #endif
  
  
  
  void spi_setup()
  {
    pinMode(_SS, OUTPUT);
    pinMode(_MISO, INPUT);
    pinMode(_MOSI, OUTPUT);
    pinMode(_SCK, OUTPUT);
    //
     digitalWrite(_SS, HIGH);
    #if _CPOL
     digitalWrite(_SCK, HIGH);
    #else
     digitalWrite(_SCK, LOW);
    #endif
  }
  
  // chip selection has been declared outside
  inline void spi_select() {digitalWrite(_SS, LOW);}
  inline void spi_unselect() {digitalWrite(_SS, HIGH);}
  
  //mode 0: SCK idle low, phase: reading at middle of SCK HIGH pulse
  //mode 1: SCK idle low, phase: reading at middle of SCK LOW pulse
  //this big-bang should work for both  CPHA=1  and CPHA=0
  uint8_t spi_transfer(uint8_t b) 
  {
    uint8_t reply=0;
    char bits[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //reading buffer
  
    /*  hardware SPI
    SPDR=b;
    spi_wait();
    reply = SPDR;
    return reply;*/
  
      //cli();  it will cause error on Arduino, most of SPI should be interrupt tolerable
      //spi_select(); // should be called outside, may be required by one transition
      delayCycles(DELAY_CYCLES);   // checking timing characteristics, need delay from CS to rising edge?
      // here, delay is added, to make CPHA=1 and CPHA=0 both work!
      
      for(uint8_t _bit = 0;_bit < 8;_bit++)
      {
      #if _CPHA
        #if _CPOL 
          digitalWrite(_SCK, LOW);  //  propagation at rising edge
       #else
          digitalWrite(_SCK, HIGH);  // change this to LOW for CPOL=1
       #endif
          digitalWrite(_SCK, HIGH);  // change this to LOW for CPOL=1
          delayCycles(DELAY_CYCLES_P0);    
      
          digitalWrite(_MOSI, !!(b & msk[_bit]));
          delayCycles(DELAY_CYCLES_P1); //  propagation
       #if _CPOL 
         digitalWrite(_SCK, HIGH);  // data will be captured at falling edge
       #else
          digitalWrite(_SCK, LOW);  
       #endif
         delayCycles(DELAY_CYCLES_C0); // holding low, so there is enough time for data preparation and changing
      
         bits[_bit] = digitalRead(_MISO); // reading at the middle of SCK pulse
         delayCycles(DELAY_CYCLES_C1);  // wait until data is fetched by slave device,  while SCK low, checking DATAsheet for this interval 
      
      #else
          // changing MOSI big while SCK low, propogation 
          digitalWrite(_MOSI, !!(b & msk[_bit]));
          delayCycles(DELAY_CYCLES_P1); // there is a requirement of LOW and HIGH have identical interval!
  
        #if _CPOL
         digitalWrite(_SCK, LOW); 
        #else
         digitalWrite(_SCK, HIGH);
        #endif
         delayCycles(DELAY_CYCLES_C0);    // 
      
         bits[_bit] = digitalRead(_MISO); // reading at the middle of SCK pulse
         delayCycles(DELAY_CYCLES_C1);  // wait until data is fetched by slave device,  while SCK high, checking DATAsheet for this interval 
         
        #if _CPOL
          digitalWrite(_SCK, HIGH);
        #else
         digitalWrite(_SCK, LOW);  // data will change at falling edge
        #endif
         delayCycles(DELAY_CYCLES_P0); // holding low, so there is enough time for data preparation and changing
      
      #endif
  
      }
     delayCycles(DELAY_CYCLES);  // checking timing characteristics, it is no needed by AD7730, from CS to rising edge
     // spi_unselect();  
  
      //sei();  // it will cause error on Arduino, most of SPI should be interrupt tolerable
  
     reply = PCK();
  
    return reply;
   
  }

#endif
