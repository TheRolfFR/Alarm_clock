#include <Wire.h>
#include <SoftwareSerial.h>
#include "RadioController.h"
#include "NextionDisplay.h"

/* IF YOU WANT TO DEBUG */

#define DEBUG true

RadioController radio;

SoftwareSerial nextion(4, 5); // (Nextion TX, Nextion RX) 4, 5
Nextion myN(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
NextionDisplay myDisplay;

Clock myClock(2, 6, 7); // interruptPin, minusPin, plusPin

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(9600);

  radio.init(98.4);

  myClock.init();
  //myClock.clock.setDateTime(__DATE__, __TIME__);
  
  myDisplay.init(&myN, &myClock, 2);
  myDisplay.setFrequency(98.4);
  digitalWrite(13, LOW);
}

void loop() {
  myDisplay.listen();
  
  radio.update();
  myClock.update(myDisplay.page, myDisplay.buttonSwitchPressed);
  myDisplay.update();
}
