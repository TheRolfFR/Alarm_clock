#include <Wire.h>
#include <DS3231.h> // https://github.com/jarzebski/Arduino-DS3231

Clock::Clock(int intPin, int pinM, int pinP) {
  DS3231 c;
  clock = c;
  
  milli = millis();
  interruptPin = intPin;
  pinPlus = pinP;
  pinMinus = pinM;

  pinMode(pinP, INPUT);
  pinMode(pinM, INPUT);
}

void Clock::init() {
  clock.begin();
}

void Clock::update(String page, bool hourOrMin) {
  // reset need of this information
  dateChanged = false;

  // initialisized to false
  int hourAdd = 0, minuteAdd = 0;

  // update if minute passed
  if(millis() - milli > 6000) {
    // allows date refresh
    dateChanged = true;
    
    milli = millis();
    dt = clock.getDateTime();

    // update temp
    temperature = clock.readTemperature();
  }

  // if current page is clock
  if(page == "clock") {
    if(digitalRead(pinPlus)) {
      dateChanged = true;
      if(hourOrMin) {
        hourAdd = 1;
      } else {
        minuteAdd = 1;
      }
    }
    else if(digitalRead(pinMinus)) {
      dateChanged = true;
      if(hourOrMin) {
        hourAdd = -1;
      } else {
        minuteAdd = -1;
      }
    }

    // update RTC date
    if(hourAdd || minuteAdd) {
      clock.setDateTime(dt.year, dt.month, dt.day, dt.hour + hourAdd, dt.minute + minuteAdd, 0);
    }
  }
}
String Clock::getDOW() {
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

String Clock::getMonth() {
  // return clock.dateFormat("F", dt));
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

String Clock::readTemperature() {
  return String(clock.readTemperature());
}
