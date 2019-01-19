#include <Arduino.h>
#include <TEA5767N.h>
#include "RadioController.h"

RadioController::RadioController() {
  radio = TEA5767N();
  state = false;
}
void RadioController::init(float nfreq) {
  radio.setStereoNoiseCancellingOn();
  radio.selectFrequency(nfreq);
  radio.setStandByOn();
}
void RadioController::setRadioState(bool radioState) {
  if(radioState) {
    radio.setStandByOff();
  } else {
    radio.setStandByOn();
  }
  #ifdef DEBUG
  Serial.println("set radio state : " + String(radioState));
  #endif
}

