#include <Nextion.h> // https://github.com/bborncr/nextion
#include <SoftwareSerial.h>
#include "NextionDisplay.h"
#include "clock.h"

#ifndef C_NEXTIONDISPLAY
#define C_NEXTIONDISPLAY

#define BTN_SWITCH_RADIO "b_sr"
#define BTN_HORM "b_horm"
#define PIC_SW_MIN 3

NextionDisplay::NextionDisplay(){}

void NextionDisplay::init(Nextion *nex, Clock *clo, int light) {
  myNextion = nex;
  myClock = clo;
  myNextion->init();
  myNextion->sendCommand(("dims=" + String(light)).c_str()); // 20
}
String NextionDisplay::listen() {
  String inString = myNextion->listen(); //check for message
  if(inString != "") {
    Serial.println("raw : " + inString);
    if(inString.indexOf("on_") != -1) {
      page = inString.substring(3);
    }
  }
  return inString;
}

void NextionDisplay::setFrequency(float f) {
  Serial.println(String(f) + " " + String(lastFreq));
  if(f != lastFreq) {
    char freq_display[6]; // XXX.X\0
    // remove one decimal to float
    dtostrf(f, 4, 1, freq_display );

    Serial.println(freq_display);
    
    myNextion->setComponentText("home.frequency", String(freq_display));
    myNextion->setComponentText("freq.frequency", String(freq_display) + " MHz");
  }
}

void NextionDisplay::update() {
  if(myClock->dateChanged) {
    String aString = String(myClock->getDOW() + " " + myClock->dt.day + " " + myClock->getMonth() + " " + myClock->dt.year);
    
    myNextion->setComponentText("home.date", aString);
    
    aString = ((myClock->dt.hour < 10) ? "0" : "") + String(myClock->dt.hour) + ":" + ((myClock->dt.minute < 10) ? "0" : "") + String(myClock->dt.minute);
    if(myClock->alarmOn) { aString = " " + aString + "."; }

    myNextion->setComponentText("home.hour", aString);
    myNextion->setComponentText("time.hour", aString);
    
    updateTemperature();
    updatePage();
  }
}

void NextionDisplay::updatePage() {
  myNextion->sendCommand(("page" + page).c_str());
  #ifdef DEBUG
  Serial.println("page refreshed : " + page);
  #endif
}

void NextionDisplay::updateTemperature() {
  myNextion->setComponentText("home.temperature", String(myClock->clock.readTemperature()));
}

#endif
