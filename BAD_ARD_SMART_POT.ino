/*
Author:	expp
*/

/*@Brief This is the Soil Humidity Sensor Pin (The sensor is connected to 5V)*/
const int PIN_SMHS = A0;

/*@Brief This is the Photo Resistor Pin(connected to 5V)*/
const int PIN_PHOTO_REST = A1;

/*@Storing the value of the SMHS in percents*/
volatile uint8_t SenPercVal_g;

volatile uint8_t PhPercVal_g;

uint8_t smhs_read()
{
	unsigned short TmpVal_L;
	TmpVal_L = analogRead(PIN_SMHS);
	TmpVal_L = map(TmpVal_L, 0, 1024, 100, 0);
	return (uint8_t)(255 & TmpVal_L);

}
uint8_t phr_read() 
{
	unsigned short TmpVal_L;
	TmpVal_L = analogRead(PIN_PHOTO_REST);
	TmpVal_L = map(TmpVal_L, 0, 1024, 0, 100);
	return (uint8_t)(255 & TmpVal_L);
}

// the setup function runs once when you press reset or power the board
void setup() {
	/*Setting up the Soil Humidity Sensor Pin to receive data */
	pinMode(PIN_SMHS, INPUT);
	/*Setting up the Photo Resistor Pin to receive data*/
	pinMode(PIN_PHOTO_REST, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {

}
