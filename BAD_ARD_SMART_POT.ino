/*
Author:	expp
*/

/*@Brief This is the Soil Humidity Sensor Pin (The sensor is connected to 5V)*/
const int PIN_SMHS = A0;

/*@Brief This is the Photo Resistor Pin(connected to 5V)*/
const int PIN_PHOTO_REST = A1;

/*@Brief This is Pump for watering the pot(connected to 12V)*/
const int PIN_PUMP = A2;

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
void watering_the_plant()
{
	SenPercVal_g = smhs_read();
	if (SenPercVal_g < 50)
	{
		for (uint8_t i = 0; i < 226; i++)
		{
			analogWrite(PIN_PUMP, i);
		}
	}
	else
	{
		for (uint8_t i = 225; i > 0; i--)
		{
			analogWrite(PIN_PUMP, i);
		}
	}
}
void info_output()
{
	SenPercVal_g = smhs_read();
	PhPercVal_g = phr_read();
	Serial.print("The Humudity in the soil is: ");
	Serial.print(SenPercVal_g);
	Serial.println("%");
	Serial.print("Light Percentage: ");
	Serial.print(PhPercVal_g);
	Serial.println("%");
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	watering_the_plant();
	info_output();
	



}
