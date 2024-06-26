# WMS7202 Digital Potentiometer library

# Summary

* This library lets you control the WMS7202 series digital potentiometers with ease.
  Examples:
   * WMS7202050P
   * WMS7202100P
* Datasheet link: https://www.digikey.ca/htmldatasheets/production/836814/0/0/1/wms7202.html
* You can communicate with the chip using the SPI.

# About the chip

* It is a dual channel digital potentiometer.
* It comes with different values, e.g. 50kOhm, 100 kOhm.
* It has a non-volatile memory which can be used to store up to 4 different values per channel.
* All potentiometers are loaded with the value stored in the NVMEM position 0 for their respective 
channel on power up.

# Instruction set
You can find more info on these in the datasheet in section 7.7

```
//Software reset the part to the power up state 
void reset();
```
```
// Discontinue clock supply to  the logic and memories 
void sleep();
```
```
//Clock supply to the logic and memories,
//you need to call this after powering up before any other operation can be performed
void wakeUp();
```
```
//Increment tap register value by one 
void increase(byte TR);
```
```
//Decrement tap register value by one 
void decrease(byte TR);
```
```
//Load the selected NVMEM location into the tap register 
void loadTRFromNVMEM(byte TR, byte slot);
```
```
//Takes the current potentiometer settings and saves in the selected NVMEM location. 
void writeTRToNVMEM(byte TR, byte slot);
```
```
//Writes a value to the tap register of the selected channel 
void writeToTR(byte TR, byte wiperPosition);
```
```
//Writes a value to the selected NVMEM register of the selected channel 
void programNVMEM(byte TR, byte slot, byte value, int outputBufferMode);
```
```
//Erases the 9 bit word stored at the specified NVMEM location.
void eraseNVMEM(byte TR, byte slot);
```
```
//Read and return the value of the selected NVMEM location. 
//by passing a pointer, outputBufferMode acts as an output parameter.
int readNVMEM(byte TR, byte slot, int* outputBufferMode);
```
```
// Read the value of the selected tap register 
int readTapRegister(byte TR);
```

# Getting started

```
WMS7202 digitalPot(D8);
...
SPI.begin();
digitalPot.wakeUp();
...
digitalPot.writeToTR(digitalPot.potA, 230);
...
digitalPot.increase(digitalPot.potA);
...
```

# Setting up the hardware
* The pinout can be found in section 5. of the datasheet.
* In order for the memory functionalities to work, you need to supply WP pin with Vdd (connect WP pin to the supply voltage)
* The SDO pin also has to be pulled up in order for the data to be read. I used a 10kOhm resistor between Vdd and SDO for this.

## Things to note
* I have never managed to get the R/B pin working. It is supposed to be pulled up, and it should indicate when the chip is ready to accept more commands. For the memory operations I have introduced a delay of 2ms instead of using this pin.
  * This value can be also found in the datasheet in Table 10 - Timing parameters as the value for "Store to NVMEM Save Time". This didn't introduce any performance penalty for my applications, but it is something to be aware of.
* There is also a small delay in the reading operations as well. It is 1ms.
### You can find out more by checking out the examples.
