/*
 * TempSensors.h
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#ifndef TEMPSENSORS_H_
#define TEMPSENSORS_H_

#include <DallasTemperature.h>
#include "TempSensor.h"

#define TEMPERATURE_PRECISION 12 //9
#define MAX_SENSORS 10

class TempSensors {
public:
	TempSensors(DallasTemperature& asensors);
	void setup();
	void test();

	void diagSensors();
	void printAddress(DeviceAddress deviceAddress);
	void getAndPrintTemperature(DeviceAddress deviceAddress);
	void addSensor(const char* label, const DeviceAddress deviceAddress);
	bool readTemp(int index);

	int sensorCount = 0;

	TempSensor _tempSensors[MAX_SENSORS];
private:
	DallasTemperature& dallasTemperature;
	int numberOfDevices = 0; // Number of temperature devices found
	DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
};

#endif /* TEMPSENSORS_H_ */
