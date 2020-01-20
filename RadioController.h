#ifndef H_RADIOCONTROLLER
#define H_RADIOCONTROLLER
#include <TEA5767N.h> // https://github.com/mroger/TEA5767
#include "OtherComponents.h"

extern EEPROMHandler eHandler;
extern OtherComponents myOtherComponents;

class RadioController{
  private:
    TEA5767N *radio;
    float freq;

    float minFreq, maxFreq;
  
  public:
    RadioController(float mini, float maxi) : minFreq(mini), maxFreq(maxi) {
    }
    
    void begin(float defaultFrequency) {
      radio = new TEA5767N();
      radio->setStandByOn();
      setRadioFrequency(defaultFrequency);
      // radio->setStereoNoiseCancellingOn();
    }
    
    bool triggerRadioState() {
      return this->setRadioState(this->isStandBy());
    }

    bool setRadioState(bool state) {
      if(state) { // turn on
        radio->setStandByOff();
        radio->turnTheSoundBackOn();
      } else { // turn off
        radio->mute();
        radio->setStandByOn();
      }
      
      return this->isStandBy();
    }
    
    bool isStandBy() {
      return radio->isStandBy();
    }

    float getFrequency() {
      return freq;
    }

    void modifyFrequency(float variation) {
      this->setRadioFrequency(constrain(freq + variation, minFreq, maxFreq));
    }

    void setRadioFrequency(float frequence) {
      freq = frequence;
      eHandler.setRadioFrequency(frequence);
      radio->selectFrequency(freq);
    }
};
  
#endif
