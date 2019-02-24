/*
RoverSensors.h

*/


#ifndef RoverSensors_h
#define RoverSensors_h

#include <Timer.h>
#include <Event.h>
#include "Arduino.h"
#include "Constant.h"
#include <Adafruit_BMP085.h>
#include "Wire.h"
#include <DS3231.h>

class RoverSensors
{
public:
	RoverSensors(int serialBegin);
	~RoverSensors();
	bool ReadFromSensors();
	//Send Function
	void SendSensorReading();
	//SerialBegin, setting rate
	void SerialBegin();
	//ObstaclesSensor
	void DetectObstacles();
	//Timertick	
	bool GetReadFlag();
	void ReturnToStation();
	int GetFrequencyReadings();
	unsigned long GetTimeReturnToStation();
	void SetFrequencyReadings(int time);
	void SetTimeToReturnToStation(unsigned long time);
private:
	//bandwith
	int rate;
	//Frequnecy of readings
	int frequencyReading;
	unsigned long timeReturnToStation;
	//Counter 10 sensor reading
	int howManyTimes;
	//Counter to array with sensor readings
	int indexBMPValues;
	//FLag to read from sensor when not printing to serial.println
	bool readFlag;
	//Printing information while not reading from sensors
	bool canPrintFlag;
	//BMP100
	Adafruit_BMP085 bmpSensor;
	//Array with sensor readings
	float bmpValues[70] PROGMEM;
	//Printing 10 sensor reading to serial.println
	void printPack();
	//Determine is night, ligth or dark
	int IsDarkOrBrightly();
	//Clock
	DS3231 clock;
	//Parser
	RTCDateTime dt;

};



#endif



















