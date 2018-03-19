/*
Author:	expp
*/


#include <CayenneMQTTWiFi.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "";
char password[] = "";
char clientID[] = "";

/*Your Wifi Name and password for the cayenne to connect to the internet*/
char ssid[] = "";
char wifipass[] = "";
/**@Brief This is the Soil Humidity Sensor Pin (The sensor is connected to 5V)*/
const int PIN_SMHS = A0;

/**@Brief This is the Photo Resistor Pin(connected to 5V)*/
const int PIN_PHOTO_REST = A1;

/**@Brief This is Pump for watering the pot(connected to 12V)*/
const int PIN_PUMP = A2;

/**@Brief This is Lamp that is going to give light to the plant because the plant does't like direct sunlight*/
const int PIN_LED_LAMP=A3;

/**@Brief This is the interval by wich the watering plant function will execute [min]  */
const int IntervalWatering_g = 30;

/**@Brief This is the interval by wich the adjusment of the intesity of the Led lamp will execute[min] */
const int IntervalAdjustLight_g = 5;

/**@Storing the value of the SMHS in percents*/
volatile uint8_t SenPercVal_g;

/**Light Level in Percents*/
volatile uint8_t PhPercVal_g;

/**@Brief This functions controls the interval that other functions will execute.
* @return Void.
*/
void proc_handler();

/**@Brief Read a light level from photo resistor and set the brightness level for a lamp.
* @return Void.
*/
void adjust_lights();

/** @Brief Send more stable data from the sensors.
* @param uint8_t Delay, Time to wait until next ramp up[ms].
* @param uint8_t pin, Set the ramp to any pin.
* @param uint8_t tests,how many time to read from the sensor(the higher the number the more stable data you will get).
* @return int.
*/
uint16_t avrg_filter(uint8_t pin,uint8_t Delay,uint8_t tests);

/** @Brief Slowly ramp up value.
* @param uint8_t rampto, max Value to ramp to [0,255].
* @param uint8_t Delay, Time to wait until next ramp up[ms].
* @param uint8_t Pin, Set the ramp to any pin.
* @param uint8_t ifValmin,The minimum value in the if statement[0,255].
* @param uint8_t ifValmax,The maximum value in the if statement[0,255].
* @return Void.
*/
void analog_ramp(uint8_t rampTo, uint8_t Delay, uint8_t pin, uint8_t ifValmin, uint8_t ifValmax);

/**@Brief If the humidity in the soil is below certain percentage slowly start a pump,else slowly turn off the pump.
* @return Void.
*/
void watering_the_plant();

/**@Brief Display information for the user.
* @return Void.
*/
void info_output();

/**@Brief Read from Soil Humidity sensor,map it in percents and send it back.
* @return uint8_t.
*/
uint8_t smhs_read_map();

/**@Brief Read from Photo Resistor,map the data and send it back.
* @return uint8_t.
*/
uint8_t phr_read_map();

// the setup function runs once when you press reset or power the board
void setup() {

	Cayenne.begin(username, password, clientID, ssid, wifipass);

	/*Setting up the Soil Humidity Sensor Pin to receive data */
	pinMode(PIN_SMHS, INPUT);
	/*Setting up the Photo Resistor Pin to receive data*/
	pinMode(PIN_PHOTO_REST, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	Cayenne.loop();
	CayenneInfo();
	//handle proccesses.
	proc_handler();

	//output information about all sensors for the user to see.
	info_output();
}

//Adjusting the brightness of the lamp
void adjust_lights()
{
	//Read the sensor.
	uint16_t sensor_valueL = avrg_filter(PIN_PHOTO_REST, 100, 10);

	//Map it.
	sensor_valueL = map(sensor_valueL, 0, 1024, 240, 10);

	//Set the mapped value for the brightness of the lamp.
	analogWrite(PIN_LED_LAMP, sensor_valueL);
}

//Water the plant on certain intevals.
void watering_the_plant()
{
	//Read and map the value from sensor 
	SenPercVal_g = smhs_read_map();
	
	//Turn on slowly the pump if the humidity in the soil is under 40%.
	analog_ramp(230, 10, PIN_PUMP, SenPercVal_g,40);
}

//Output information for the user in the Serial Monitor
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

/*Send information about the sensors to the cayenne*/
void CayenneInfo();

uint8_t smhs_read_map()
{
	uint16_t TmpValL;
	TmpValL = avrg_filter(PIN_SMHS, 100, 10);
	TmpValL = map(TmpValL, 0, 1024, 100, 0);
	return (uint8_t)(255 & TmpValL);

}

uint8_t phr_read_map()
{
	uint16_t TmpValL;
	TmpValL = avrg_filter(PIN_PHOTO_REST, 100, 10);
	TmpValL = map(TmpValL, 0, 1024, 0, 100);
	return (uint8_t)(255 & TmpValL);
}

//Slowly turn on/off something in certain conditions.
void analog_ramp(uint8_t rampTo, uint8_t Delay,uint8_t pin,uint8_t ifValmin,uint8_t ifValmax)
{
	if (ifValmin < ifValmax)
	{
		for (uint8_t fadeValue = 0; fadeValue <= rampTo; fadeValue++)
		{
			analogWrite(pin, fadeValue);
			delay(Delay);
			if (fadeValue == rampTo) break;
		}
	}
	else
	{
		for (uint8_t fadeValue = rampTo; fadeValue >= 0; fadeValue--)
		{
			analogWrite(pin, fadeValue);
			delay(Delay);
			if (fadeValue == 0)break;
		}
	}
}

//Read more Stable data from a sensor.
uint16_t avrg_filter(uint8_t pin, uint8_t Delay, uint8_t tests)
{
	int outputValL = 0;
	for (uint8_t sample = 0; sample < tests; sample++)
	{
		outputValL += analogRead(pin);
		delay(Delay);
	}
	return (uint16_t)(outputValL / tests);
}

void proc_handler()
{
	static unsigned long PreviousTimeWL = 0;

	static unsigned long PreviousTImeAdL = 0;

	static unsigned long CurrentTimeL = 0;

	//get the current miliseconds from wich the controler is turnned on.
	CurrentTimeL = millis();
	
	//water the plant if the  CurrentTimeL-PreviousTimeWL is greater or equal to IntervalWatering_g * 60000;
	if (CurrentTimeL - PreviousTimeWL >= IntervalWatering_g * 60000)
	{
		PreviousTimeWL = CurrentTimeL;
		watering_the_plant();
	}

	if (CurrentTimeL - PreviousTImeAdL >= IntervalAdjustLight_g * 60000)
	{
		PreviousTImeAdL = CurrentTimeL;
		adjust_lights();
	}
}

void CayenneInfo()
{
	SenPercVal_g = smhs_read_map();
	PhPercVal_g = phr_read_map();
	Cayenne.virtualWrite(0,SenPercVal_g);
	Cayenne.virtualWrite(1,PhPercVal_g);
}