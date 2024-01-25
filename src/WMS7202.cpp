#include "Print.h"
#include "Arduino.h"
#include "WMS7202.h"

WMS7202::WMS7202(int csPin){
  _csPin = csPin;
  digitalWrite(csPin, HIGH);
  pinMode(_csPin, OUTPUT);
  SPI.begin();
  _digitalPotSPISettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
}

void WMS7202::reset(){
  sendCommandOverSPI(resetCommand);
}

void WMS7202::sleep(){
  sendCommandOverSPI(sleepCommand);
}

void WMS7202::wakeUp(){
  sendCommandOverSPI(wakeUpCommand);
}

void WMS7202::increase(byte TR){
  sendCommandOverSPI(increaseTRCommand | constrain(TR,0,1));
}

void WMS7202::decrease(byte TR){
  sendCommandOverSPI(decreaseTRCommand | constrain(TR,0,1));
}

 void WMS7202::loadTRFromNVMEM(byte TR, byte slot){
  sendCommandOverSPI(loadTRFromNVMEMCommmand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1));
  delay(2);
}

void WMS7202::writeTRToNVMEM(byte TR, byte slot){
  eraseNVMEM(constrain(TR,0,1), constrain(slot, 0, 3));
  sendCommandOverSPI(programNVMEMFromTRCommand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1));
  delay(2);
}

void WMS7202::writeToTR(byte TR, byte wiperPosition){
  sendDataOverSPI(writeTapRegisterCommand | constrain(TR,0,1), 0, constrain(wiperPosition,0,255));
}

void WMS7202::eraseNVMEM(byte TR, byte slot){
  sendCommandOverSPI(eraseNVMEMCommand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1));
  delay(2);
}

void WMS7202::programNVMEM(byte TR, byte slot, byte value, int outputBufferMode){
  eraseNVMEM(constrain(TR,0,1), constrain(slot, 0, 3));
  byte command = programNVMEMWithDataCommand | (constrain(slot, 0, 3) << 2) | constrain(TR, 0, 1);
  sendDataOverSPI(
  programNVMEMWithDataCommand | (constrain(slot, 0, 3) << 2) | constrain(TR, 0, 1),
  constrain(outputBufferMode, 0, 1),
  value
  );
  delay(2);
}

int WMS7202::readNVMEM(byte TR, byte slot, int* outputBufferMode){
  SPI.beginTransaction(_digitalPotSPISettings);
  byte buffer[3];
  buffer[0] = readNVMEMCommand | (constrain(slot, 0, 3) <<2) | constrain(TR, 0, 1);
  buffer[1] = 0;
  buffer[2] = 0;
  digitalWrite(_csPin, LOW);
  SPI.transfer(buffer, 3);
  buffer[0] = nopCommand;
  buffer[1] = 0;
  buffer[2] = 0;
  digitalWrite(_csPin, HIGH);
  delay(1);
  digitalWrite(_csPin, LOW);
  SPI.transfer(buffer, 3);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
  // only the last bit is needed, the rest are "no-care" bits, the datasheet
  // doesn't specifiy the value for these, so we need to filter those out with %
  *outputBufferMode = buffer[1] % 2;
  return buffer[2];
}

int WMS7202::readTapRegister(byte TR){
  SPI.beginTransaction(_digitalPotSPISettings);
  byte buffer[3];
  buffer[0] = readTapRegisterCommand | constrain(TR, 0, 1);
  buffer[1] = 0;
  buffer[2] = 0;

  digitalWrite(_csPin, LOW);

  SPI.transfer(buffer, 3);
  buffer[0] = nopCommand;
  buffer[1] = 0;
  buffer[2] = 0;
  digitalWrite(_csPin, HIGH);
  delay(1);
  digitalWrite(_csPin, LOW);
  SPI.transfer(buffer, 3);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
  return buffer[2];
  
}

void WMS7202::sendDataOverSPI(byte commandByte, byte dataByte1, byte dataByte2){
  SPI.beginTransaction(_digitalPotSPISettings);
  byte buffer[3];
  buffer[0] = commandByte;
  buffer[1] = dataByte1;
  buffer[2] = dataByte2;
  digitalWrite(_csPin, LOW);
  SPI.transfer(buffer,3);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
}

void WMS7202::sendCommandOverSPI(byte commandByte){
  SPI.beginTransaction(_digitalPotSPISettings);
  byte buffer[3];
  buffer[0] = commandByte;
  buffer[1] = 0;
  buffer[2] = 0;
  digitalWrite(_csPin, LOW);
  SPI.transfer(buffer,3);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
}

