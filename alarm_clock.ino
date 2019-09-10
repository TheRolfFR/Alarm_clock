#include <Wire.h>
#include "Clock.h"

static ClockController myClock();

byte LED_PIN = 13;

void setup() {
  Serial.begin(9600);
  myClock.begin();

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  //Serial.println(myClock.readTemperature());
  Serial.println(myClock.getFullTime());
  //Serial.println(myClock.getFullDate());
  if(myClock.isAlarmOn()) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("alarm on");
    myClock.armAlarm(false);
    delay(200);
  } else {
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}
