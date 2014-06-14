#include "dht22.h"

DHTSensor::DHTSensor(uint8_t pin_) : pin(pin_)
{}


float DHTSensor::getTemperature()
{
	read();
	return 0.0;
}

float DHTSensor::getHumidity()
{}


void DHTSensor::read()
{
	//When communication between MCU and DHT begins MCU will pull down
	//data bus for 1~10ms. Therefor, pull down and wait for 1ms.
	pinMode(pin,OUTPUT);
	digitalWrite(pin,LOW);
	delay(1);

	//Then MCU will pull up signal and wait for 20-40us. Therefore:
	//Pull up and wait for 40ms
	digitalWrite(pin,HIGH);
	delayMicroseconds(40);

	//When DHT detects signal, it will pull down the bus for 80us.  We
	//set the timeout to 100us. For a 16Mhz cpu we have 16*10^6 clock
	//cycles. That is 16 clock cycles per us (16*10^6 / 10^6, 10^6 us
	//are one second). 100us are therefore 1600 clock cycles. One loop
	//step will need approx. 4 clock cycles (4 instructions),
	//therefore we need 400 loop steps.
	pinMode(pin,INPUT);
	int loop_count = 400;
	while(digitalRead(pin) == LOW){
		if(--loop_count == 0){
			Serial.println("Timeout");
		}
	}			
	
	//DHT22 pulls up bus for 80us for preperation to send data.
	loop_count = 400;
	while(digitalRead(pin) == HIGH){
		if(--loop_count == 0){
			Serial.println("Timeout");
		}
	}
	Serial.println("Ok");
}


	
