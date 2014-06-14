#ifndef dht22_h
#define dht22_h
#include <Arduino.h>


// http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

class DHTSensor
{
public:
   DHTSensor(uint8_t pin_);
   float getTemperature();
   float getHumidity();
   
private:
   void read();
   uint8_t pin;
};


#endif
