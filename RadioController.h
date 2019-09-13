#ifndef H_RADIOCONTROLLER
#define H_RADIOCONTROLLER
#include <TEA5767N.h> // https://github.com/mroger/TEA5767

#define FREQ_MAX_MHZ 108
#define FREQ_MIN_MHZ 87.5

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
      setRadioFrequency(defaultFrequency);
      radio->setStereoNoiseCancellingOn();
    }
    
    bool setRadioState() {
      if(radio->isStandBy()) {
        radio->setStandByOff();
      } else {
        radio->setStandByOn();
      }
      
      return radio->isStandBy();
    }
    bool isStandBy() {
      return radio->isStandBy();
    }

    float getFrequency() {
      return freq;
    }

    void modifyFrequency(float variation) {
      setRadioFrequency(Math.min(Math.max(freq + variation, FREQ_MIN_MHZ), FREQ_MAX_MHZ));
    }

    void setRadioFrequency(float frequence) {
      freq = frequence;
      radio->selectFrequency(freq);
    }
};
  
#endif
