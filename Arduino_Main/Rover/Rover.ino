/*
 Name:		Rover.ino
 Created:	19.11.2018 15:59:38
 Author:	Grzegorz Wawrzeniecki
*/

// the setup function runs once when you press reset or power the board





#include "RoverSensors.h"


RoverSensors rover(9600);
Timer t;
int readings;

void setup() {
	//Starts sensors and serial
	rover.SerialBegin();
	Serial.println(F("setup"));
	//Running readings every k time
	readings = t.every(rover.GetFrequencyReadings(), ReadAllSensors);
	//After k time return to station
	t.after(rover.GetTimeReturnToStation(), ReturnToStation);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Timer update
	t.update();
	//Sends Readings
	rover.SendSensorReading();
	//Detecting obstacles
	rover.DetectObstacles();
}

void ReadAllSensors() {

	if (rover.GetReadFlag() == true) {
		rover.ReadFromSensors();
	}

}

void ReturnToStation() {
	rover.ReturnToStation();
	t.stop(readings);
}
