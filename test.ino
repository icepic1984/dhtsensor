//#include "DHT.h"
#include "dht22.h"

#define DHTPIN 10
#define DHTTYPE DHT22

//DHT dht(DHTPIN, DHTTYPE);

DHTSensor dht(DHTPIN);

void setup() 
{
	Serial.begin(9600);
	Serial.println("DHT22 test");
}

void loop() 
{
	delay(2000);
	if ( DHTSENSOR_OK == dht.readDataFromSensor() ) {
		Serial.println(dht.getTemperature());
		Serial.println(dht.getHumidity());
	} else {
		Serial.print("ERROR reading sensor");
	}
}

