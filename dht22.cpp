#include "dht22.h"

DHTSensor::DHTSensor(uint8_t pin_) : pin(pin_),
									 emperature(0.0),
									 humidity(0.0),
									 elapsed(2000)
{}

float DHTSensor::getTemperature()
{
	return temperature;
}

float DHTSensor::getHumidity()
{
	return humidity;
}

DHTSensorCodes DHTSensor::readDataFromSensor()
{
	//According to specs sensor can be queried only after 2secs.
	if ( millis() -  elapsed >= 2000 ) {
		elapsed = millis();
		DHTSensorCodes err = read();
		if ( err == DHTSENSOR_OK) {
			//Check checksum
			uint8_t sum = data[0] + data[1] + data[2] + data[3];
			if ( sum == data[4]) {
				//Humidity is first two bits divided by 10
				uint16_t t_hum = data[0];
				t_hum <<= 8;
				t_hum |= data[1];
				humidity = static_cast<float>(t_hum) * 0.1f;
				//Check if first bit of temperature is one -> minus degree
				if( data[2] & 128) {
					uint16_t t_temp = data[2] & 127;
					t_temp <<= 8;
					t_temp |= data[3];
					temperature =  -0.1f * static_cast<float>(t_temp);
				} else {
					uint16_t t_temp = data[2];
					t_temp <<= 8;
					t_temp |= data[3];
					temperature = static_cast<float>(t_temp) * 0.1f;
				}
				return DHTSENSOR_OK;
			} else {
				return DHTSENSOR_CURRUPTION;
			}
		};
		return err;
	}
	return DHTSENSOR_READTOFAST;
}


DHTSensorCodes DHTSensor::read()
{
	//Set all databits to zero
	for ( uint8_t i = 0; i < 5; ++i ) {
		data[i] = 0;
	}

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
	//therefore we need 400 loop steps to detect a timeout.
	pinMode(pin,INPUT);
	int loop_count = 400;
	while ( digitalRead(pin) == LOW ) {
		if ( --loop_count == 0 ) {
			return DHTSENSOR_TIMEOUT;
		}
	}			
	//DHT22 pulls up bus for 80us for preparation to send data.
	loop_count = 400;
	while( digitalRead(pin) == HIGH ){
		if( --loop_count == 0 ) {
			return DHTSENSOR_TIMEOUT;
		}
	}
	//DHT22 sends 40bits to mcu:
	//First 16Bit: Humidity
	//Second 16Bit: Temperature
	//Last 8Bit:  Checksum
	uint8_t mask = 128;
	uint8_t idx = 0;
	for ( uint8_t i = 0; i < 40; i++ ) {

		//When DHT22 is sending data to mcu, every transmission begins
		//with a low signal which last for 50us
		loop_count = 400;
		while ( digitalRead(pin) == LOW ) {
			if ( --loop_count == 0) {
				return DHTSENSOR_TIMEOUT;
			}
		}
		
		//The following high-voltage signal decides if bit is 0 or 1.
		//Signal is high for 26-28us => 0
		//Signal is high for 70us    => 1
		//We measure, how long the signal is high. For more then 40us 
		//we set the current bit to 1.
		unsigned long t = micros();
		loop_count = 400;
		while ( digitalRead(pin) == HIGH ) {
			if ( --loop_count == 0 ) {
				return DHTSENSOR_TIMEOUT;
			}
		}
		unsigned long e = micros() - t;
		
		//When signal is HIGH for more then 40us we set the current
		//bit to one.
		if ( e > 40 ) data[idx] |= mask;
		//Shift the mask by one bit to the right
		mask >>= 1;
		//if mask equals to zero one byte is complete
		if ( mask == 0 ) {
			mask = 128;
			idx++;
		}
	}
	return DHTSENSOR_OK;
}

