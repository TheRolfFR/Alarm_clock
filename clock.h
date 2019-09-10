#ifndef H_CLOCK
#define H_CLOCK

#include <DS3231.h>

class ClockController {
  private:
    // clock data
    DS3231 rtc;
    RTCDateTime dt;
  public:
    ClockController();
    
    void begin();
    
    void update(String page, bool buttonPressed, String newString);

    // alarm
    void increaseAlarmHour();
    
    void increaseAlarmMinute();
    
    void armAlarm(bool state);

    float readTemperature();

    String getFullTime();
    String getFullDate();

    bool isAlarmOn();
    String getAlarmHour();
    String getAlarmMinute();

    void resetClock();
  private:
    String _getDOW();
    String _getMonth();
    String _getAlarmPointer();
    void _getDt();
};

#endif
