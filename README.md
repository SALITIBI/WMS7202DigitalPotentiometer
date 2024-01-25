# WMS7202 Digital Potentiometer library

This is still a work in progress.

# Summary

This library lets you control the WMS7202 digital potentiometer with ease.
Datasheet link: https://www.digikey.ca/htmldatasheets/production/836814/0/0/1/wms7202.html
You can communicate with the chip using the SPI.

#Usage
```
WMS7202 digitalPot(D8);
digitalPot.wakeUp();
...
digitalPot.writeToTR(digitalPot.potA, 230);
...
digitalPot.increase(digitalPot.potA);
...
```