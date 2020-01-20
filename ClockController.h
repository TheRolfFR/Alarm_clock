#ifndef H_CLOCK
#define H_CLOCK

#include <Wire.h>
#include <DS3231.h> // https://github.com/jarzebski/Arduino-DS3231
#include "RadioController.h"
// #include "lang_en.h"
#include "lang_fr.h"

#define ALARM_DELAY_MINUTE 2

extern EEPROMHandler eHandler;
extern RadioController myRadio;

class ClockController {
  private:
    // clock data
    DS3231 rtc;
    RTCDateTime dt;

    int alarmHour;
    int alarmMinute;
    
    String _getDOW(){
      _getDt();

      // return day of week from lang
      return daysOfWeek[dt.dayOfWeek];
    }
    
    void _updateAlarm() {
      eHandler.setAlarm(alarmHour, alarmMinute);
      rtc.setAlarm1(0, alarmHour, alarmMinute, 0, DS3231_MATCH_H_M_S, false);
    }
    String _getMonth() {
      _getDt();

      // return month from lang
      return months[dt.month];
    }
    String _getAlarmPointer() {
      return (rtc.isArmed1()) ? "." : " ";
    }
    void _getDt() {
      dt = rtc.getDateTime();
    }
  
  public:
    ClockController() : rtc(), dt(), alarmHour(), alarmMinute() {
      // hook up module SDA to A4 and SCL to A5
    }
    
    void begin(int a_h, int a_m) {
      alarmHour = a_h;
      alarmMinute = a_m;
      // communicating with the clock
      rtc.begin();
    
      // first update of alarm
      _updateAlarm();
      
      // setting the default delay for alarm
      rtc.setAlarm2(0, 0, ALARM_DELAY_MINUTE, DS3231_MATCH_M, false);
    }

    // main update function
    void update(bool isButtonPressed) {
      if(isAlarmOn()) {
        myRadio.setRadioState(true);
        if(isButtonPressed) {
          this->armAlarm(false);
        }
      }
    }

    // alarm
    void increaseAlarmHour() {
      alarmHour = (alarmHour + 1) % 24;
      _updateAlarm();
    }
    void increaseAlarmMinute() {
      alarmMinute = (alarmMinute + 1) % 60;
      _updateAlarm();
    }
    void armAlarm(bool state) {
      rtc.armAlarm1(state);
      if(!state) {
        rtc.armAlarm2(state); // disable alarm 2 too
      }
    }

    // temperature
    float readTemperature() {
      return rtc.readTemperature();
    }

    String getFullTime(){
      _getDt();
      
      String timeString = String(" ");
      if(dt.hour < 10) {
        timeString += "0";
      }
      timeString += dt.hour;
      timeString += ":";
      if(dt.minute < 10) {
        timeString += "0";
      }
      timeString += dt.minute;
      timeString += _getAlarmPointer();
      
      return timeString;
    }
    String getFullDate() {
      _getDt();
      
      String dateString = _getDOW();
      dateString += " ";
      dateString += dt.day;
      dateString += " ";
      dateString += _getMonth();
      dateString += " ";
      dateString += dt.year;
    
      return dateString;
    }

    bool isAlarmOn() {
      if(rtc.isAlarm1() && !rtc.isArmed2()) {
        rtc.armAlarm2(true);
      }
      return rtc.isAlarm1() || rtc.isAlarm2();
    }
    String getAlarmHour() {
      return ((alarmHour < 10) ? "0" : "") + String(alarmHour);
    }
    String getAlarmMinute() {
      return ((alarmMinute < 10) ? "0" : "") + String(alarmMinute);
    }

    void resetDateAndTime() {
      rtc.setDateTime(__DATE__, __TIME__);
    }
};

#endif
