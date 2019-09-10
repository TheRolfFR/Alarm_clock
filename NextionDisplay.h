#ifndef C_NEXTIONDISPLAY
#define C_NEXTIONDISPLAY
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
#include "clock.h"

#define BTN_SWITCH_RADIO "b_sr"
#define BTN_HORM "b_horm"
#define PIC_SW_MIN 3

SoftwareSerial nextion(4, 5); // (Nextion TX, Nextion RX) 4, 5
static Nextion myN(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
extern Clock myClock;
extern RadioController myRadio;

class NextionDisplay {
  private:
   int luminosityPin;
   bool luminosityState = HIGH;
   float lastFreq = 0.0;
   bool buttonSwitchPressed = false;
   bool radioState = false;
   String page = "home";
   
  public:
    NextionDisplay(int lPin) {
      luminosityPin = lPin;
    }

    void init(int light) {
      myN.init();
      myN.sendCommand(("dims=" + String(light)).c_str()); // 20

      Serial.println("NextionDisplayed started");
      
      pinMode(luminosityPin, INPUT_PULLUP);
    }

    String listen() {
      String inString = myN.listen(); //check for message
      if(inString != "") {
        Serial.println("raw : " + inString);
        if(inString.indexOf("on_") != -1) {
          page = inString.substring(3);
        }
        if(inString.indexOf("to_") != -1) {
          page = inString.substring(3);
          updatePage();
        }
        if(inString == "b_sr") {
          bool state = myRadio.setRadioState();
          myN.sendCommand(("sr.pic=" + String(3 + state*1)).c_str()); // 3 - 4
        }
        else if(inString == "increaseAlarmHour") {
          myClock.increaseAlarmHour();
        }
        else if(inString == "increaseAlarmMinute") {
          myClock.increaseAlarmMinute();
        }
      }
      return inString;
    }

    void setFrequency(float f) {
     Serial.println(String(f) + " " + String(lastFreq));
      if(f != lastFreq) {
        char freq_display[6]; // XXX.X\0
        // remove one decimal to float
        dtostrf(f, 4, 1, freq_display );
    
        Serial.println(freq_display);
        
        myN.setComponentText("home.frequency", String(freq_display));
        myN.setComponentText("freq.frequency", String(freq_display) + " MHz");
      }
    }

    void update(bool force = false) {
      if(myClock.isDateChanged() || force) {
        myClock.setDateChanged(false);
        //Serial.println("Starting refresh");
        
        String aString = String(myClock.getDOW() + " " + myClock.getDt().day + " " + myClock.getMonth() + " " + myClock.getDt().year);
        
        myN.setComponentText("home.date", aString);
        
        aString = ((myClock.getDt().hour < 10) ? "0" : "") + String(myClock.getDt().hour) + ":" + ((myClock.getDt().minute < 10) ? "0" : "") + String(myClock.getDt().minute);
        if(myClock.isAlarmOn()) { aString = " " + aString + "."; }
    
        myN.setComponentText("home.hour", aString);
    
        myN.setComponentText("time.hourText", myClock.getAlarmHour());
        myN.setComponentText("time.minuteText", myClock.getAlarmMinute());
        myN.setComponentText("time.pointerText", myClock.getAlarmPointer());
        
        updateTemperature();
        updatePage();
        updatePage();
        
        //Serial.println("Ended refresh");
      }
      if(digitalRead(luminosityPin) != luminosityState || force) {
        luminosityState = digitalRead(luminosityPin);
        if(luminosityState == HIGH) {
          myN.sendCommand("dims=20"); // 20 luminosity high
        } else {
          myN.sendCommand("dims=2"); //  2 luminosity low
        }
        myClock.armAlarm(!luminosityState);
      }
    }

    void updatePage() {
      myN.sendCommand(("page " + page).c_str());
      //Serial.println("page refreshed : " + page);
    }

    void updateTemperature() {
      myN.setComponentText("home.temperature", String(myClock.getClock().readTemperature()));
    }

    String getPage() {
      return page;
    }

    bool getButtonSwitchPressed() {
      return buttonSwitchPressed;
    }
};

#endif
