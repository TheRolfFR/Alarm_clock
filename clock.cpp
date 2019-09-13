#include <Wire.h>
#include <DS3231.h> // https://github.com/jarzebski/Arduino-DS3231
#include "clock.h"

#define ALARM_DEFAULT_HOUR 6
#define ALARM_DEFAULT_MINUTE 35
#define ALARM_DELAY_MINUTE 2

ClockController::ClockController() {
  DS3231 c; // hook up module SDA to A4 and SCL to A5
  rtc = c;
}

void ClockController::begin() {
  // communicating with the clock
  rtc.begin();

  // setting default alarm time
  alarmHour = ALARM_DEFAULT_HOUR;
  alarmMinute = ALARM_DEFAULT_MINUTE;
  _updateAlarm();
  
  // setting the default delay for alarm
  rtc.setAlarm2(0, 0, ALARM_DELAY_MINUTE, DS3231_MATCH_M, false);
}
void ClockController::_updateAlarm() {
  rtc.setAlarm1(0, alarmHour, alarmMinute, 0, DS3231_MATCH_H_M_S, false);
}
void ClockController::increaseAlarmHour() {
  alarmHour = (alarmHour + 1) % 24;
  _updateAlarm();
}
void ClockController::increaseAlarmMinute() {
  alarmMinute = (alarmMinute + 1) % 60;
  _updateAlarm();
}
String ClockController::getAlarmHour() {
  return ((alarmHour < 10) ? "0" : "") + String(alarmHour);
}
String ClockController::getAlarmMinute() {
  return ((alarmMinute < 10) ? "0" : "") + String(alarmMinute);
}
String ClockController::_getAlarmPointer() {
  return (rtc.isArmed1()) ? "." : " ";
}
void ClockController::_getDt() {
  dt = rtc.getDateTime();
}
String ClockController::_getMonth() {
  _getDt();
  switch(dt.month) {
    case 1:
      return "JANVIER";
    case 2:
      return "FEVRIER";
    case 3:
      return "MARS";
    case 4:
      return "AVRIL";
    case 5:
      return "MAI";
    case 6:
      return "JUIN";
    case 7:
      return "JUILLET";
    case 8:
      return "AOUT";
    case 9:
      return "SEPTEMBRE";
    case 10:
      return "OCTOBRE";
    case 11:
      return "NOVEMBRE";
    default:
      return "DECEMBRE";
  }
}
String ClockController::_getDOW() {
  _getDt();
  switch(dt.dayOfWeek) {
    case 1:
      return "LUNDI";
    case 2:
      return "MARDI";
    case 3:
      return "MERCREDI";
    case 4:
      return "JEUDI";
    case 5:
      return "VENDREDI";
    case 6:
      return "SAMEDI";
    default:
      return "DIMANCHE";
  }
}
float ClockController::readTemperature() {
  return rtc.readTemperature();
}
bool ClockController::isAlarmOn() {
  if(rtc.isAlarm1() && !rtc.isArmed2()) {
    rtc.armAlarm2(true);
  }
  return rtc.isAlarm1() || rtc.isAlarm2();
}
void ClockController::armAlarm(bool state) {
  rtc.armAlarm1(state);
  if(!state) {
    rtc.armAlarm2(state); // disable alarm 2 too
  }
}
void ClockController::resetDateAndTime() {
  rtc.setDateTime(__DATE__, __TIME__);
}
String ClockController::getFullTime() {
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
String ClockController::getFullDate() {
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
