#include <Wire.h>
//#include <DS3231.h> // https://github.com/jarzebski/Arduino-DS3231
#include <SoftwareSerial.h>
//#include <Nextion.h> // https://github.com/bborncr/nextion
//#include <TEA5767N.h> // https://github.com/mroger/TEA5767
#include "RadioController.h"
#include "NextionDisplay.h"


/* IF YOU WANT TO DEBUG */

#define DEBUG true

/* TOGGLE THE INSTRUCTIONS ABOVE */

/*

// nextion init
SoftwareSerial nextion(4, 5); // (Nextion TX, Nextion RX)
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
int end_pos, equal_pos, on_pos, operation;
String event, page = "home";
#define BTN_SWITCH_RADIO "b_sr"
#define BTN_HORM "b_horm"
#define PIC_SW_MIN 3
#define EVT_INIT "init"
bool horm = false, minus, plus;

// clock and alarm init
DS3231 clock;
RTCDateTime dt;
#define SNOOZE_INTERVAL 5 // minutes
int alarmInterruptPin = 2, minute = 0, alarmHour = 6, alarmMinute = 20, snooze = 0, minusPin = 6, plusPin = 7;
unsigned long ms = 0;
String aString;
bool alarmOn = false;
long milli = millis(); // used in order to fetch time

// radio init
#define F_MIN 875  // Frequence min 87.5
#define F_MAX 1080 // Frequence max 108
#define BAND F_MIN-F_MAX
bool radioState = false;
int int_freq = 1000; // initial frequency multiplied by 10
float float_freq = int_freq / 10.0f;
char freq_display[6]; // XXX.X\0
//TEA5767N radio = TEA5767N();*/

RadioController radio;

SoftwareSerial nextion(4, 5); // (Nextion TX, Nextion RX) 4, 5
Nextion myN(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
NextionDisplay myDisplay;

Clock myClock(2, 6, 7); // interruptPin, minusPin, plusPin

void setup() {
  Serial.begin(9600);

  radio.init(98.4);

  myClock.init(&MyDisplay);
  
  myDisplay.init(&myN, &myClock, 20);
  myDisplay.setFrequency(98.4);
}

void loop() {
  myDisplay.listen();
  radio.setRadioState(myDisplay.radioState);
  
  //radio.checkFrequency(NextionDisplay.getFrequency());

  myClock.updateClock();
  myDisplay.updateDisplay();

  //myDisplay.updateTemperature(myClock.temperature);
  
  myDisplay.updateTemperature();

  
  /*String inString = myNextion.listen(); //check for message
  if (inString != "") { // if a message is received...
    #ifdef DEBUG
    Serial.println("raw : " + inString);
    #endif
    end_pos = inString.indexOf("|");
    if (end_pos > -1) {// another event...
      event = inString.substring(0, end_pos);
      on_pos = event.indexOf("on_");
      if(inString.indexOf(EVT_INIT) > -1) {
          displayDate();
          setRadioFrequency();
          #ifdef DEBUG
          Serial.println("init of nextion");
          #endif
      }
      if(on_pos > -1) { // page preloaded
        page = event.substring(3, end_pos);
        myNextion.sendCommandString("freq.sr.pic=" + String(PIC_SW_MIN+radioState*1));
        myNextion.sendCommandString("time.horm.pic=" + String(PIC_SW_MIN+horm*1));
      } else { // button pressed
        
        #ifdef DEBUG
        Serial.println(event + " button pressed!");
        #endif
        
        if(event == BTN_SWITCH_RADIO) {
          switchRadio();
          myNextion.sendCommandString("freq.sr.pic=" + String(PIC_SW_MIN+radioState*1));
        }
        else if(event == BTN_HORM) {
          horm = 1 - horm;
          myNextion.sendCommandString("time.horm.pic=" + String(PIC_SW_MIN+horm*1));
        }
      }
    }
  }
  
  if (millis() - milli > 60000) {
    milli = millis();
    dt = clock.getDateTime();
    minute = dt.minute;
  }

  // buttons interactions
  minus = digitalRead(minusPin);
  plus = digitalRead(plusPin);
  if(minus || plus) {
    
    operation = plus*1 - minus*1;
    
    if(page == "home") {
      if(alarmOn && millis()-ms > SNOOZE_INTERVAL*60000) {
        ms = millis();
        moreSnooze();
      }
    } else if(page == "time") { // horm false means hour and vice versa
      if(alarmOn) { // alarm
        
        if(horm) { alarmMinute = limit(alarmMinute, 59, operation); }
        else { alarmHour = limit(alarmHour, 23, operation); }
        setAlarm();
        
      } else { // clock
        if(horm) {  dt.minute = limit(dt.minute, 59, operation); }
        else { dt.hour = limit(dt.hour, 23, operation); }
      }
      setDate();
      displayDate();
    } else if(page == "freq") {
      int_freq = (int_freq + operation < F_MIN) ? F_MIN : (int_freq+operation)%(F_MAX+1);
      setRadioFrequency();
      updatePage();
    }
    delay(200);
  }*/
}

/*
int limit(int number, int maxi, int operation) {
  return (operation == 1) ? (number < maxi)*(number+1) : ((number > 0) ? number-1 : maxi);
}

void moreSnooze() {
  snooze++;
  Serial.println("Snooze please : " + String(SNOOZE_INTERVAL*snooze));
  setAlarm();
}

void setAlarm() {
  //setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
  clock.setAlarm1(0, alarmHour, alarmMinute + snooze*SNOOZE_INTERVAL, 0, DS3231_MATCH_H_M_S, false);
}

void setDate() {
  // setDateTime(Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, 0);
}
void displayDate() {
  aString = String(getDayOfWeek(dt.dayOfWeek) + " " + dt.day + " " + getMonth(dt.month) + " " + dt.year);
  //aString = "caca";
  myNextion.setComponentText("home.date", aString);
  aString = String(clock.dateFormat("H:i", dt));
  if(alarmOn) { aString = " " + aString + "."; }
  myNextion.setComponentText("home.hour", aString);
  myNextion.setComponentText("time.hour", aString);
  Serial.println(aString);
  myNextion.setComponentText("home.temperature", String(clock.readTemperature()));
  updatePage();
}

void setRadioFrequency() {
  float_freq = int_freq / 10.0f;

  // remove one decimal to float
  dtostrf(float_freq, 4, 1, freq_display );

  //Sets the radio station
  radio.selectFrequency(float_freq);
  #ifdef DEBUG
  Serial.println(freq_display);
  Serial.println(float_freq);
  #endif
  // update nextion component
  myNextion.setComponentText("home.frequency", freq_display);
  myNextion.setComponentText("freq.frequency", String(freq_display) + " MHz");
}
void switchRadio() {
  radioState = 1 - radioState;
  if(radioState) {
    radio.setStandByOff();
  } else {
    radio.setStandByOn();
  }
  #ifdef DEBUG
  Serial.println("set radio state : " + String(radioState));
  #endif
}
void updatePage() {
  myNextion.sendCommandString("page" + page);
  #ifdef DEBUG
  Serial.println("page refreshed : " + page);
  #endif
}*/

