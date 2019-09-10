#include <Wire.h>
#include "NextionDisplay.h"
#include "RadioController.h"
#include "Clock.h"

static ClockController *myClock = new ClockController();
static NextionDisplay *myNextionDisplay = new NextionDisplay();
static RadioController *myRadio = new RadioController();

#define LED_PIN 13
#define NEXTION_START_LUMINOSITY 35 // 35 - 20
#define NEXTION_TX_PIN 4
#define NEXTION_RX_PIN 5
#define RADIO_DEFAULT_FREQUENCY 90

void setup() {
  Serial.begin(9600);
  myClock->begin();
  myNextionDisplay->begin(NEXTION_TX_PIN, NEXTION_RX_PIN, NEXTION_START_LUMINOSITY);
  myRadio->begin(RADIO_DEFAULT_FREQUENCY);
  // myClock->resetDateAndTime();
}

void loop() {
  myNextionDisplay->update();
}
