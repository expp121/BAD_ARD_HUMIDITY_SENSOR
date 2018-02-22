/*
 Name:		BAD_ARD_HUMIDITY_SENSOR.ino
 Created:	2/22/2018 6:57:21 PM
 Author:	expp
*/
/*@Brief This is the Soil Humidity Sensor Pin (The sensor is connected to 5V)*/
const int PIN_SMHS = A0;
/*@Storing the value of the SMHS in percents*/
volatile uint8_t SenPercVal_g;

uint8_t sensor_read()
{
	short TmpVal_L;
	TmpVal_L = analogRead(PIN_SMHS);
	TmpVal_L = map(TmpVal_L, 0, 1024, 100, 0);
	return (uint8_t)(255 & TmpVal_L);

}

// the setup function runs once when you press reset or power the board
void setup() {

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
