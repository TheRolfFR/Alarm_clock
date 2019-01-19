#include <DS3231.h>
#include "NextionDisplay.h"

#ifndef H_CLOCK
#define H_CLOCK

class Clock {
  private:
    int pinPlus;
    int pinMinus;
    int interruptPin;
    long milli;
  public:
    Clock(int, int, int);
    DS3231 clock;
    NextionDisplay *nextionDisplay;
    RTCDateTime dt;
    
    void updateClock();
    void increaseHour();
    void increaseMinute();
    
    String getMonth();
    String getDOW();

    String readTemperature();
    
    void activateAlarm();
    
    bool alarmOn = false;
    bool dateChanged = false;
    int temperature = 0;
};
#endif

