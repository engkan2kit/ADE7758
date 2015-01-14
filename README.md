ADE7758
=======
THIS IS NOT FOR BEGINNERS. RISKS of ELECTRICUTION, DAMAGE TO ELECTRONIC DEVICES ATTACHED INCLUDING LAPTOPS are POSSIBLE.
USE at your OWN RISK.

Arduino Library for ADE7758

This Library is not a complete ADE7758 library. I just implemented only the functions I used in a certain project. 

I included an arduino .ino file for you test inside the ADEtest folder and I think also the ADE7758 in the root directory is a test. 

The library uses software SPI and the pinning is defined in the PINS.h. If you want to use the hardware SPI, change the software SPI functions to hardware SPI equivalent inside ADE7758.cpp and the use the hardware SPI pins.I used the hardware SPI before but I had to change it software SPI because I had to dedicate the hardware SPI alone with a very time sensitive IC. Feel free to change it. I didn't have time to edit this to use both.

You can implement other functions by filling up the empty functions I  left in the .cpp file. Just read the datasheet for the algorithm.

for SCHEMATIC:

Use at your own RISK. This circuit has been tested by Computer and Electrical Engineering Majors in one of the 230V RMS line-to-line phase of a 3-Phase 60Hz source. This circuit works even if your feed 3 single-phase input as long as you connect the neutral line to the VN.

Jumpers JP1,2 and 3 are for when you decide to use two-watt meter method (tested). Remove jumpers when you want to use 3 watt-meter method. NEVER TOUCH the board(even the ground plane) when the power is on!

On each branch in VAP, VBP and VCP, I added a series 1M resistor and a pull down 1K resistor after the 1M. This is a voltage divider to divide the line voltage to LineV*1K/(1K+1M) or approximately LineV/1000 so a 110V line to ground will appear as 110mV in the VAP of the IC. 

For two-watt meter method (please read about this), JP1,2 and 3 is shorted. Connect line Voltage A to VAP header. Line voltage B to VAP in the header connect Line Voltage C to VN header (no need to connect anything to VCP header). Therefore, what you read in AV register is the VAC voltage and BV register is the VBC. In two-watt meter method, you only need two line voltages and the currents running to these lines to solve the over-all power. So connect your current sensor to IA and IB only.
NOTE: Current Sensor you should use is a current transformer. Add a low resistance high power resistor between the two pins of the CT as shunt so that you can read voltage. The current channels in the IC reads voltage. Compensate for the ratio in the program, the resistor and voltage to current conversion based on the resistor (I=V/R) in your program. 
Advantage: no need for neutral ground. Disadvantage: some information are not available.

For three watt meter method just remove tha short in JP1,2 and 3. Connect voltage A, B, C and Neutral lines to VAP,VBP,VCP and VN respectively. connect all line currents to each corresponding current input header.

Also notice that the ADE is isolated from the SPI header using high speed opto-isolators so that there is less risk of damaging your Arduino and PC.

Connect to arduino SPI pins you used the SPI header in the schematic. In PCB layout, use separate ground planes for AC and MCU DC sides. THE AC side has AC components, Analog DC and digital DC(DC side of the ADE7758). Use the same ground plane for AC and Analog DC. SO a total of 3 Ground planes. Connect the Analog ground to DC side of the ADE7758 by at a single point. (Read about mixed signal routing). 
