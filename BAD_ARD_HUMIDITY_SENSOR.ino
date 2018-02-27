/*
Author:	expp
*/

/*@Brief This is the Soil Humidity Sensor Pin (The sensor is connected to 5V)*/
const int PIN_SMHS = A0;

/*@Brief This is the Photo Resistor Pin(connected to 5V)*/
const int PIN_PHOTO_REST = A1;

/*@Brief This is Pump for watering the pot(connected to 12V)*/
const int PIN_PUMP = A2;

/*@Brief This is Lamp that is going to give light to the plant because the plant does't like direct sunlight*/
const int PIN_LED_LAMP=A3;

/*@Storing the value of the SMHS in percents*/
volatile uint8_t SenPercVal_g;

/*Light Level in Percents*/
volatile uint8_t PhPercVal_g;

void light_level_map();
void watering_the_plant();
void info_output();
uint8_t smhs_read_map();
uint8_t phr_read_map();

// the setup function runs once when you press reset or power the board
void setup() {
	/*Setting up the Soil Humidity Sensor Pin to receive data */
	pinMode(PIN_SMHS, INPUT);
	/*Setting up the Photo Resistor Pin to receive data*/
	pinMode(PIN_PHOTO_REST, INPUT);
}



// the loop function runs over and over again until power down or reset
void loop()
{
	watering_the_plant();
	info_output();
	light_level_map();
}

void light_level_map()
{
	short sensor_valueL = analogRead(PIN_PHOTO_REST);
	sensor_valueL = map(sensor_valueL, 0, 1024, 240, 10);
	analogWrite(PIN_LED_LAMP, sensor_valueL);
}

void watering_the_plant()
{
	SenPercVal_g = smhs_read_map();
	if (SenPercVal_g < 50)
	{
		for (uint8_t i = 0; i < 226; i++)
		{
			analogWrite(PIN_PUMP, i);
		}
	}
	else
	{
		for (uint8_t i = 225; i >= 0; i--)
		{
			analogWrite(PIN_PUMP, i);
		}
	}
}

void info_output()
{
	SenPercVal_g = smhs_read_map();
	PhPercVal_g = phr_read_map();
	Serial.print("The Humudity in the soil is: ");
	Serial.print(SenPercVal_g);
	Serial.println("%");
	Serial.print("Light Percentage: ");
	Serial.print(PhPercVal_g);
	Serial.println("%");
}

uint8_t smhs_read_map()
{
	unsigned short TmpValL;
	TmpValL = analogRead(PIN_SMHS);
	TmpValL = map(TmpValL, 0, 1024, 100, 0);
	return (uint8_t)(255 & TmpValL);

}
uint8_t phr_read_map()
{
	unsigned short TmpValL;
	TmpValL = analogRead(PIN_PHOTO_REST);
	TmpValL = map(TmpValL, 0, 1024, 0, 100);
	return (uint8_t)(255 & TmpValL);
}