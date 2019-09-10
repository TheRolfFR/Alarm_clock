#ifndef H_RADIOCONTROLLER
#define H_RADIOCONTROLLER
#include <TEA5767N.h> // https://github.com/mroger/TEA5767

class RadioController{
  private:
    TEA5767N radio;
    bool state = false;
    float freq;
  
  public:
    RadioController() {
      radio = TEA5767N();
      state = false;
    }
    
    void init(float nfreq, TEA5767N theRadio) {
      radio = theRadio;
      Serial.println("RadioController started");
      radio.setStandByOn();
      Serial.println("RadioController started");
      radio.setStereoNoiseCancellingOn();
      Serial.println("RadioController started");
      radio.selectFrequency(nfreq);

      Serial.println("RadioController started");
    }
    
    bool setRadioState() {
      state = !state;
      if(state) {
        radio.setStandByOff();
      } else {
        radio.setStandByOn();
      }
      #ifdef DEBUG
      //Serial.println("set radio state : " + String(radioState));
      #endif
      
      return state;
    }
    
    void update() {
      
    }
};
  
static RadioController myRadio;
  
#endif
