#include "Print.h"
#include "Arduino.h"
#include "WMS7202.h"

WMS7202::WMS7202(int csPin){
  _csPin = csPin;
  digitalWrite(csPin, HIGH);
  pinMode(_csPin, OUTPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(SPI_CLOCK_DIV16);
}

void WMS7202::reset(){
  sendDataOverSPI(resetCommand, 0, 0);
}

void WMS7202::sleep(){
  sendDataOverSPI(sleepCommand, 0, 0);
}

void WMS7202::wakeUp(){
  sendDataOverSPI(wakeUpCommand, 0, 0);
}

void WMS7202::increase(byte TR){
  sendDataOverSPI(increaseTRCommand | constrain(TR,0,1), 0, 0);
}

void WMS7202::decrease(byte TR){
  sendDataOverSPI(decreaseTRCommand | constrain(TR,0,1), 0, 0);
}

 void WMS7202::loadTRFromNVMEM(byte TR, byte slot){
  sendDataOverSPI(loadTRFromNVMEMCommmand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1), 0, 0);
  delay(2);
}

void WMS7202::writeTRToNVMEM(byte TR, byte slot){
  eraseNVMEM(constrain(TR,0,1), constrain(slot, 0, 3));
  sendDataOverSPI(programNVMEMFromTRCommand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1), 0, 0);
  delay(2);
}

void WMS7202::writeToTR(byte TR, byte wiperPosition){
  sendDataOverSPI(writeTapRegisterCommand | constrain(TR,0,1), 0, constrain(wiperPosition,0,255));
}

void WMS7202::eraseNVMEM(byte TR, byte slot){
  sendDataOverSPI(eraseNVMEMCommand | (constrain(slot, 0, 3) << 2) | constrain(TR,0,1), 0, 0);
  delay(2);
}

void WMS7202::programNVMEM(byte TR, byte slot, byte value, int outputBufferMode){
  eraseNVMEM(constrain(TR,0,1), constrain(slot, 0, 3));
  byte command = programNVMEMWithDataCommand | (constrain(slot, 0, 3) << 2) | constrain(TR, 0, 1);
  Serial.println("");
  Serial.print("Command to send: ");
  Serial.print(command, BIN);
  Serial.print(" output buffer mode: ");
  Serial.print(outputBufferMode, BIN);
  Serial.print(" value: ");
  Serial.println(value);
  sendDataOverSPI(
  programNVMEMWithDataCommand | (constrain(slot, 0, 3) << 2) | constrain(TR, 0, 1),
  constrain(outputBufferMode, 0, 1),
  value
  );
  delay(2);
}

int WMS7202::readNVMEM(byte TR, byte slot, int* outputBufferMode){
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
  // only the last bit is needed, the rest are "no-care" bits, the datasheet
  // doesn't specifiy the value for these, so we need to filter those out with %
  *outputBufferMode = buffer[1] % 2;
  return buffer[2];
}

int WMS7202::readTapRegister(byte TR){
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
  return buffer[2];
}

void WMS7202::sendDataOverSPI(byte commandByte, byte dataByte1, byte dataByte2){
  byte buffer[3];
  buffer[0] = commandByte;
  buffer[1] = dataByte1;
  buffer[2] = dataByte2;
  digitalWrite(_csPin, LOW);                // select chip
  SPI.transfer(buffer,3);             // configure target pot with wiper position
  digitalWrite(_csPin, HIGH);               // de-select chip
}
