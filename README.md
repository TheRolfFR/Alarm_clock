# Arduino connected alarm clock 
Arduino Alarm Clock with FM radio, Real-Time Clock module and a touch screen
## *Work in progress*

# List of components
- Arduino development board
- PAM8403 mini 5V digital amplifier board : [Ebay link](https://www.ebay.com/itm/PAM8403-Mini-5V-Digital-Small-Power-Amplifier-Board-with-Switched-Potentiometer/401937591144?hash=item5d9558eb68:g:l4EAAOSwiXNdkWe3)
- TEA5767 FM Stereo Radio Module : [Ebay link](https://www.ebay.fr/itm/TEA5767-FM-Stereo-Radio-Module-for-Arduino-76-108MHZ-With-Free-Cable-Antenna/263280085434?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2060353.m2749.l2649)
- DS3231 Precision Real-Time Clock Module : [Ebay link](https://www.ebay.com/itm/Arduino-DS3231-AT24C32-IIC-Module-Precision-RTC-Real-Time-Clock-Memory-ST537/362413657616?epid=7028399073&hash=item5461898e10:g:RyAAAOSwxZZbcosf)
- Nextion smart display : [Official shop link](https://www.itead.cc/nextion-nx4827t043.html)
# The wiring
The FM radio and RTC module are working via I²C so you need to connect their SDA and SCL pins to the ones corresponding to the configuration of your arduino development board

The potentiometer can bee hooked on any analog pin and the button on any digital pin.

All the modifiable pins are constants that can be changed in the main ino code file.