#include <TEA5767N.h>

class RadioController{
  public:
    RadioController();
    void setRadioState(bool);
    void init(float);
    TEA5767N radio;
    bool state = true;
    float freq;
};

