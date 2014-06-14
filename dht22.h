#ifndef dht22_h
#define dht22_h
#include <Arduino.h>


// http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf
enum DHTSensorCodes {
  DHTSENSOR_OK,
  DHTSENSOR_TIMEOUT,
  DHTSENSOR_CURRUPTION,
  DHTSENSOR_READTOFAST
};

class DHTSensor
{
public:
   DHTSensor(uint8_t pin_);
   float getTemperature();
   float getHumidity();
   DHTSensorCodes readDataFromSensor();
   
private:
   DHTSensorCodes read();
   uint8_t pin;
   float temperature;
   float humidity;
   unsigned long elapsed;
   uint8_t data[5];
};


#endif
