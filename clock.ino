#include <Wire.h>
#include <DS3231.h>

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

void Clock::init(NextionDisplay *nd) {
  nextionDisplay = nd;

  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
}

void Clock::updateClock() {
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
  return "SHUT UP MOTHER F#CKER";
}

String Clock::readTemperature() {
  return String(clock.readTemperature());
}

