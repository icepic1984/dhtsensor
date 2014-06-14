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
	if ( DHTSENSOR_OK == dht.readDataFromSensor() ) {
		Serial.print("Temperature: ");
		Serial.print(dht.getTemperature());
		Serial.print("C ");
		Serial.print(" Humidity: ");
		Serial.print(dht.getHumidity());
		Serial.println("%");
	} else {
		Serial.println("ERROR reading sensor");
	}
	delay(2000);

}

