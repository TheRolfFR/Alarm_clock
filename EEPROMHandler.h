#ifndef H_EEPROM_HANDLER
#define H_EEPROM_HANDLER

#include <EEPROMex.h>

class EEPROMHandler {
  private:
    float minFreq, maxFreq;
    
    int   radioFreqAdress;
    int   alarmHourAdress;
    int alarmMinuteAdress;

  protected:
    
    void _writeInt(int adress, int val, int mini, int maxi) {
      EEPROM.updateInt(adress, constrain(adress, mini, maxi));
    }
    
    void _writeFloat(int adress, float val, float mini, float maxi) {
      EEPROM.updateFloat(adress, constrain(val, mini, maxi));
    }
    
    float _readFloat(int adress, float mini, float maxi) {
      return constrain(EEPROM.readFloat(adress), mini, maxi);
    }
    
    int _readInt(int adress, int mini, int maxi) {
      return constrain(EEPROM.readInt(adress), mini, maxi);
    }

    void _waitUntilReady() {
      while (!EEPROM.isReady()) {}
    }
    
  public:
    EEPROMHandler(float mini, float maxi) : minFreq(mini), maxFreq(maxi) {
      radioFreqAdress   = EEPROM.getAddress(sizeof(float));
      alarmHourAdress   = EEPROM.getAddress(sizeof(int));
      alarmMinuteAdress = EEPROM.getAddress(sizeof(int));
    }
    
    void setDefaults(float radioDefault, int alarmHourDefault, int alarmMinuteDefault) {
      _waitUntilReady();
      this->setRadioFrequency(radioDefault);
      this->setAlarm(alarmHourDefault, alarmMinuteDefault);
    }

    void setRadioFrequency(float radioFreq) {
      _waitUntilReady();
      _writeFloat(radioFreqAdress, radioFreq, minFreq, maxFreq);
    }

    void setAlarm(int alarmHour, int alarmMinute) {
      setAlarmHour(alarmHour);
      setAlarmMinute(alarmMinute);
    }

    void setAlarmHour(int val) {
      _waitUntilReady();
      _writeInt(alarmHourAdress, val, 0, 23);
    }
    void setAlarmMinute(int val) {
      _waitUntilReady();
      _writeInt(alarmMinuteAdress, val, 0, 59);
    }

    float getRadioFrequency() {
      _waitUntilReady();
      return _readFloat(radioFreqAdress, minFreq, maxFreq);
    }

    int getAlarmHour() {
      _waitUntilReady();
      return _readInt(alarmHourAdress, 0, 23);
    }

    int getAlarmMinute() {
      _waitUntilReady();
      return _readInt(alarmMinuteAdress, 0, 59);
    }
};

#endif
