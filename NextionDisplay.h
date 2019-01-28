#ifndef H_NEXTIONDISPLAY
#define H_NEXTIONDISPLAY
#include <Nextion.h>
#include "clock.h"

class NextionDisplay {
  public:
   Nextion *myNextion;
   Clock *myClock;
   NextionDisplay();
   
   void init(Nextion*, Clock*, int light);
   String listen();
   void setFrequency(float freq);
   void GetFrequency();
   void updatePage();
   void updateTemperature();
   void update();
   
   float lastFreq = 0.0;
   bool buttonSwitchPressed = false;
   bool radioState = false;
   String page = "home";
   
};

#endif
