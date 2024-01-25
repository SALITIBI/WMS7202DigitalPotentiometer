#include <SPI.h>
#include <WMS7202.h>

WMS7202 digitalPot(D8);

void setup() {
  SPI.begin();
  Serial.begin(115200);
  digitalPot.wakeUp();
  
  Serial.println("WMS7202 Demo");
  Serial.println();

  delay(2000);
  //Set all memory slots for all channels to have 0 value saved.
  digitalPot.programNVMEM(digitalPot.potB,0,0,0);
  digitalPot.programNVMEM(digitalPot.potB,1,0,0);
  digitalPot.programNVMEM(digitalPot.potB,2,0,0);
  digitalPot.programNVMEM(digitalPot.potB,3,0,0);
  digitalPot.programNVMEM(digitalPot.potA,0,0,0);
  digitalPot.programNVMEM(digitalPot.potA,1,0,0);
  digitalPot.programNVMEM(digitalPot.potA,2,0,0);
  digitalPot.programNVMEM(digitalPot.potA,3,0,0);
  delay(2000);

  //Set the tap registers (the resistor tap position) to 230
  digitalPot.writeToTR(digitalPot.potA, 230);
  digitalPot.writeToTR(digitalPot.potB, 230);

  //write these values to the NVMEM slot 0
  digitalPot.writeTRToNVMEM(digitalPot.potB, 0);
  digitalPot.writeTRToNVMEM(digitalPot.potA, 0);

  //read back memory contents and send it over serial
  prettyPrintMemoryContent();
  delay(2000);
  Serial.println("*********************************************************");
  // set different values for all the NVMEM slots
  digitalPot.programNVMEM(digitalPot.potB,1,50,0);
  digitalPot.programNVMEM(digitalPot.potB,2,100,0);
  digitalPot.programNVMEM(digitalPot.potB,3,150,0);
  digitalPot.programNVMEM(digitalPot.potA,1,50,0);
  digitalPot.programNVMEM(digitalPot.potA,2,100,0);
  digitalPot.programNVMEM(digitalPot.potA,3,150,0);

    //read back memory contents and send it over serial
  prettyPrintMemoryContent();
  delay(2000);
}

void loop() {
  for(int i = 0; i < 4; i++){
    int bufferMode;
    Serial.print("Setting value of TR from memory slot: ");
    Serial.print(i);
    Serial.print(" value: ");
    Serial.println(digitalPot.readNVMEM(digitalPot.potA, i, &bufferMode));
    digitalPot.loadTRFromNVMEM(digitalPot.potB, i);
    digitalPot.loadTRFromNVMEM(digitalPot.potA, i);
    delay(1000);
  }
}

void prettyPrintMemoryContent(){
  for(int i = 0; i < 4; i++){
    printMemorySlotContent(digitalPot.potA, i);
    printMemorySlotContent(digitalPot.potB, i);
  }
}

void printMemorySlotContent(byte pot, byte slot){
  int outputBufferMode = 555;
  int memory = digitalPot.readNVMEM(pot, slot, &outputBufferMode);
  Serial.println();
  Serial.print("pot: ");
  Serial.print(pot);
  Serial.print(" slot: ");
  Serial.println(slot);
  Serial.print("read from memory:");
  Serial.print("value: ");
  Serial.print(memory);
  Serial.print(" bufferMode: ");
  Serial.println(outputBufferMode);
}