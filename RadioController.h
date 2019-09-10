#ifndef H_RADIOCONTROLLER
#define H_RADIOCONTROLLER
#include <TEA5767N.h> // https://github.com/mroger/TEA5767

class RadioController{
  private:
    TEA5767N *radio;
    float freq;
  
  public:
    RadioController() {
    }
    
    void begin(float defaultFrequency) {
      radio = new TEA5767N();
      radio->setStandByOn();
      radio->setStereoNoiseCancellingOn();
      radio->selectFrequency(defaultFrequency);
    }
    
    bool setRadioState() {
      if(radio->isStandBy()) {
        radio->setStandByOff();
      } else {
        radio->setStandByOn();
      }
      
      return radio->isStandBy();
    }

    float getFrequency() {
      return radio->readFrequencyInMHz();
    }
};
  
#endif
