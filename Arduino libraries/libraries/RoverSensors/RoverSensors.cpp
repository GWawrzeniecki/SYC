// 
// 
// 

#include "RoverSensors.h"


RoverSensors::RoverSensors(int serialBegin)
{
	rate = serialBegin;
	howManyTimes = 0;
	indexBMPValues = 0;
	frequencyReading = 1000;
	timeReturnToStation = 360000;//360 sekund
	readFlag = true;
	canPrintFlag = false;;
	//Setting pin 2 for input for detector obstacles
	pinMode(OBSTACLE_DETECTOR_PIN, INPUT);
}

RoverSensors::~RoverSensors()
{

}



bool RoverSensors::ReadFromSensors()
{

	if (howManyTimes <= NUMBER_OF_READINGS) {

		bmpValues[indexBMPValues++] = bmpSensor.readTemperature();
		bmpValues[indexBMPValues++] = bmpSensor.readPressure();
		bmpValues[indexBMPValues++] = bmpSensor.readAltitude();
		bmpValues[indexBMPValues++] = IsDarkOrBrightly();
		bmpValues[indexBMPValues++] = dt.hour;
		bmpValues[indexBMPValues++] = dt.minute;
		bmpValues[indexBMPValues++] = dt.second;

		howManyTimes++;
	}
	else {

		canPrintFlag = true;
	}

	return true;
}


void RoverSensors::SendSensorReading()
{
	//Update of date  nonstop
	dt = clock.getDateTime();

	if (canPrintFlag && howManyTimes == NUMBER_OF_READINGS + 1) {

		readFlag = false;
		printPack();
		howManyTimes = 0;
		indexBMPValues = 0;
		canPrintFlag = false;
		readFlag = true;

	}
}


void RoverSensors::SerialBegin()
{
	Serial.begin(rate);
	//Start interface I2C
	Wire.begin();
	//Start clock
	clock.begin();
	// Set sketch compiling time
	clock.setDateTime(__DATE__, __TIME__);
	//BMP start
	if (!this->bmpSensor.begin())
	{
		Serial.println(F("Could not find a valid BMP085 sensor, check wiring!"));
		while (1) {}
	}

}


bool RoverSensors::GetReadFlag()
{
	return readFlag;
}


void RoverSensors::ReturnToStation()
{
	Serial.println(F("Returning to station"));
}

int RoverSensors::GetFrequencyReadings()
{
	return this->frequencyReading;
}

unsigned long RoverSensors::GetTimeReturnToStation()
{
	return this->timeReturnToStation;
}

void RoverSensors::SetFrequencyReadings(int time)
{
	this->frequencyReading = time;
}

void RoverSensors::SetTimeToReturnToStation(unsigned long time)
{
	this->timeReturnToStation = time;
}

void RoverSensors::printPack()
{
	Serial.println(F("Date of collecting measurements"));
	Serial.print((dt.year));
	Serial.print(F(":"));
	Serial.print((dt.month));
	Serial.print(F(":"));
	Serial.print((dt.day));
	Serial.println(F(""));


	for (int i = 0; i < 68; i += 7) {
		Serial.printf(F("The temperature is: "));
		Serial.print(bmpValues[i]);
		Serial.println(F(" *C"));
		Serial.print(F("The pressure is "));
		Serial.print(bmpValues[i + 1] / 100);
		Serial.println(F(" hPa"));
		Serial.print(F("The altiude is "));
		Serial.println(bmpValues[i + 2]);

		if (bmpValues[i + 3] == 1) {
			Serial.println(F("It's Light"));
		}
		else if (bmpValues[i + 3] == 0) {
			Serial.println(F("It's Dark"));
		}
		else if (bmpValues[i + 3] == -1) {
			Serial.println(F("It's Night"));
		}
		else {
			Serial.println(F("Level of lighting: Unknown"));
			Serial.println(bmpValues[i + 3]);
		}

		Serial.println(F("Measurement collected at :"));
		Serial.print(bmpValues[i + 4], 0);
		Serial.print(F(":"));
		Serial.print(bmpValues[i + 5], 0);
		Serial.print(F(":"));
		Serial.println(bmpValues[i + 6], 0);
	}

}

int RoverSensors::IsDarkOrBrightly()
{

	int returnValue = -100;
	//Reading for analog PIN 
	int sensorValue = analogRead(0);

	//Light
	if (sensorValue < 200) {
		returnValue = 1;
	}//Dark
	else if (sensorValue < 350) {
		returnValue = 0;
	}//Night
	else if (sensorValue > 350) {
		returnValue = -1;
	}
	else {
		returnValue = sensorValue;
	}

	return returnValue;
}

void RoverSensors::DetectObstacles()
{
	if (digitalRead(OBSTACLE_DETECTOR_PIN) == OBSTACLE) { Serial.println(F("Attention: Avoiding an obstacle")); }
}

