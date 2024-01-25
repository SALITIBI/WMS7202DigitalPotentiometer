
#ifndef WMS7202_h
#define WMS7202_h
#include <SPI.h>
#include "Arduino.h"
#include "WMS7202.h"

class WMS7202

{
  public:
    const byte potA = 0x0;
    const byte potB = 0x1;
    
    WMS7202(int csPin);
    
    void reset();
    void sleep();
    void wakeUp();
    void increase(byte TR);
    void decrease(byte TR);
    void loadTRFromNVMEM(byte TR, byte slot);
    void writeTRToNVMEM(byte TR, byte slot);
    void writeToTR(byte TR, byte wiperPosition);
    void programNVMEM(byte TR, byte slot, byte value, int outputBufferMode);
    void eraseNVMEM(byte TR, byte slot);
    int readNVMEM(byte TR, byte slot, int* outputBufferMode);
    int readTapRegister(byte TR);
    
  private:
    const byte nopCommand = 0x0 << 4;
    const byte readTapRegisterCommand = 0xC << 4;
    const byte writeTapRegisterCommand = 0x4 << 4;
    const byte readNVMEMCommand = 0xA << 4;
    const byte programNVMEMWithDataCommand = 0x2 << 4;
    const byte loadTRFromNVMEMCommmand = 0xB << 4;
    const byte programNVMEMFromTRCommand = 0x3 << 4;
    const byte increaseTRCommand = 0x7 << 4;
    const byte decreaseTRCommand = 0xF << 4;
    const byte sleepCommand = 0x8 << 4;
    const byte wakeUpCommand = 0x1 << 4;
    const byte eraseNVMEMCommand = 0xD << 4;
    const byte resetCommand = 0x9 << 4;

    int _csPin;
    SPISettings _digitalPotSPISettings;
    void sendDataOverSPI(byte commandByte, byte dataByte1, byte dataByte2);
    void sendCommandOverSPI(byte commandByte);


};
#endif