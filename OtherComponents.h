#ifndef H_OTHER_COMPONENTS
  #define H_OTHER_COMPONENTS

  #define MEASURES 20

class OtherComponents {
  private:
    byte radioPotentiometerPin;
    int total;
    int values[MEASURES];
    float lastValue;
    int index;
    
  public:
    OtherComponents() {
    }

    void begin(byte rpPin) {
      radioPotentiometerPin = rpPin;

      int raw = analogRead(radioPotentiometerPin);
      total = 0;
      for(int i = 0; i < MEASURES; i++) {
        values[i] = raw;
        total += raw;
      }
      index = 0;
      lastValue = raw;
    }

    void readRadioPotentiometer() {
      total -= values[index];
      values[index] = analogRead(radioPotentiometerPin);
      total += values[index];
      index = (index+1)%MEASURES;

      lastValue = (float) total / MEASURES;
    }

    float getRadioPotentiometer() {
      return lastValue;
    }

    void update() {
      readRadioPotentiometer();
    }
};

#endif
