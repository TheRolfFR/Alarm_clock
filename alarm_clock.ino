#include <Wire.h>
#include "EEPROMHandler.h"
#include "NextionDisplay.h"
#include "OtherComponents.h"
#include "RadioController.h"
#include "ClockController.h"

// MHz
#define FREQ_MIN_MHZ 87.5f
#define FREQ_MAX_MHZ 108.f

#define NEXTION_START_LUMINOSITY 35 // 35 - 20
#define NEXTION_TX_PIN 4 // D4
#define NEXTION_RX_PIN 5 // D5
#define RADIO_DEFAULT_FREQUENCY 96.5f

#define ALARM_DEFAULT_HOUR 6
#define ALARM_DEFAULT_MINUTE 35

#define RADIO_POTENTIOMETER_PIN A0

static EEPROMHandler eHandler(FREQ_MIN_MHZ, FREQ_MAX_MHZ);
static OtherComponents myOtherComponents;
static ClockController myClock;
static NextionDisplay myNextionDisplay;
static RadioController myRadio(FREQ_MIN_MHZ, FREQ_MAX_MHZ);

void setup() {
  // eHandler.setDefaults(RADIO_DEFAULT_FREQUENCY, ALARM_DEFAULT_HOUR, ALARM_DEFAULT_MINUTE);
  
  Serial.begin(9600);
  myClock.begin(eHandler.getAlarmHour(), eHandler.getAlarmMinute());
  myNextionDisplay.begin(NEXTION_TX_PIN, NEXTION_RX_PIN, NEXTION_START_LUMINOSITY);
  myRadio.begin(eHandler.getRadioFrequency());
  myOtherComponents.begin(RADIO_POTENTIOMETER_PIN);
  //myClock.resetDateAndTime();
}

void loop() {
  myNextionDisplay.update();
  myClock.update(false);
}
