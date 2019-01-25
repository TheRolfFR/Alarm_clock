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
    NextionDisplay *m_nextionDisplay;
    RTCDateTime dt;

    void init(NextionDisplay*);
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
