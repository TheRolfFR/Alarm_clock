#include <TEA5767N.h> // https://github.com/mroger/TEA5767

class RadioController{
  public:
    RadioController();
    
    TEA5767N radio;
    bool state = true;
    float freq;
    
    void init(float);
    void setRadioState(bool);
    void update();
};
