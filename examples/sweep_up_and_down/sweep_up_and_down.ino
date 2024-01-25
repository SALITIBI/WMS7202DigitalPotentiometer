#include <WMS7202.h>

int csPin = D8;
int maxPosition = 256;
WMS7202 digitalPot(D8);

void setup() {
  SPI.begin();
  digitalPot.wakeUp();
  digitalPot.writeToTR(digitalPot.potA, 0);
  digitalPot.writeToTR(digitalPot.potB, 0);
}

void loop() {

  for(int i = 0; i < maxPosition; i++){
    digitalPot.increase(digitalPot.potA);
    digitalPot.increase(digitalPot.potB);
    delay(50);
  }
  
  for(int i = 0; i < maxPosition; i++){
    digitalPot.decrease(digitalPot.potA);
    digitalPot.decrease(digitalPot.potB);
    delay(50);
  }

}
