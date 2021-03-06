#ifndef C_NEXTIONDISPLAY
#define C_NEXTIONDISPLAY
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
#include "ClockController.h"
#include "RadioController.h"
#include "OtherComponents.h"

#define SWITCH_RADIO_MESSAGE "b_switch_radio"
#define SWITCH_RADIO_OBJNAME "switchradio"
#define PIC_RADIO_OFF "3"
#define PIC_RADIO_ON "4"

extern OtherComponents myOtherComponents;
extern ClockController myClock;
extern RadioController myRadio;

class NextionDisplay {
  private:
    int luminosityPin;
    bool luminosityState = HIGH;
    float lastFreq = 0.0;
    bool buttonSwitchPressed = false;
    bool radioState = false;
    String page = "home";

    long milli;

    SoftwareSerial *serial;
    Nextion *nextion;
  public:
    NextionDisplay() {
    }

    void begin(byte txPin, byte rxPin, int light) {
      serial = new SoftwareSerial(txPin, rxPin); // (Nextion TX, Nextion RX) 4, 5
      nextion = new Nextion(*serial, 9600); //create a Nextion object named nextion using the nextion serial port @ 9600bps
      
      nextion->init();
      _sendCommand(("dims=" + String(light)).c_str()); // 20
      goToHome();
      milli = millis();
    }

    void goToHome() {
      _goTo("home");
      _refreshHome();
    }

    void update() {
      String message = _listen();
      if(message != "") {
        Serial.println(message);
        if(message.startsWith("to_")) { // if a button wants to go somewhere
          _goTo(message.substring(3)); // then go
        }
        else if(message.startsWith("on_")) { // if you landed on a page
          page = message.substring(3); // tell me

          // if on landing to go to home
          if(page.equals("landing")) { goToHome(); }
        } else if(message.equals("increaseAlarmHour")) {
          myClock.increaseAlarmHour();
          _refreshAlarm();
        } else if(message.equals("increaseAlarmMinute")) {
          myClock.increaseAlarmMinute();
          _refreshAlarm();
        } else if(message.equals(SWITCH_RADIO_MESSAGE)) {
          myRadio.triggerRadioState();
          _updateImage();
        } else if(message.equals("increase_frequency")) {
          _modifyFrequency(+1);
        } else if(message.equals("decrease_frequency")) {
          _modifyFrequency(-0.1);
        }
      }

      // update home every minute
      if(millis() - milli >= 60000) {
        milli = millis();
        _refreshHome();
      }
    }

  private:
    String _listen() {
      String r = nextion->listen();
      r.replace(String("?"), String(""));
      return r;
    }
    void _goTo(String page) {
      // sends a message to change page
      nextion->sendCommand(String("page " + page).c_str()); // needs constant char

      // if you go on home refresh it beforehand
      if(page.equals("home")) { _refreshHome(); }
      // if on frequency refresh frequency beforehand
      if(page.equals("freq")) { _refreshFreq(); _updateImage(); }
      // if on time refresh time beforehand
      if(page.equals("time")) { _refreshAlarm(); }
    }
    void _refreshPage() {
      _goTo(page);
    }
    void _refreshHome() {
      nextion->setComponentText("home.time", myClock.getFullTime());
      nextion->setComponentText("home.date", myClock.getFullDate());
      nextion->setComponentText("home.temperature", String(myClock.readTemperature()));
      float f = myRadio.getFrequency();
      nextion->setComponentText("home.frequency", String(f));
    }
    void _refreshFreq() {
      String frequency = String(myRadio.getFrequency());
      Serial.println(frequency);
      frequency += String("MHz");
      nextion->setComponentText("freq.frequency", frequency);
    }
    void _refreshAlarm() {
      nextion->setComponentText("time.hourText", myClock.getAlarmHour());
      nextion->setComponentText("time.minuteText", myClock.getAlarmMinute());
    }
    void _updateImage() {
      if(myRadio.isStandBy()) {
        _sendCommand((String(SWITCH_RADIO_OBJNAME) + String(".pic=") + String(PIC_RADIO_OFF)).c_str());
      } else {
        _sendCommand((String(SWITCH_RADIO_OBJNAME) + String(".pic=") + String(PIC_RADIO_ON)).c_str());
      }
    }
    void _sendCommand(const char* cmd){
      Serial.println(cmd);
      nextion->sendCommand(cmd);
    }
    void _modifyFrequency(float variation) {
      myRadio.modifyFrequency(variation);
      _refreshFreq();
    }
};

#endif
