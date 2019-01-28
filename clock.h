#ifndef H_CLOCK
#define H_CLOCK

#include <DS3231.h>
#include "NextionDisplay.h"

class Clock {
  private:
    int pinPlus;
    int pinMinus;
    int interruptPin;
    long milli;
  public:
    Clock(int, int, int);
    DS3231 clock;
    RTCDateTime dt;

    void init();
    void update(String, bool);
    void increaseHour();
    void increaseMinute();
    void update();
    
    String getMonth();
    String getDOW();

    String readTemperature();
    
    void activateAlarm();
    
    bool alarmOn = false;
    bool dateChanged = false;
    int temperature = 0;
};
#endif
