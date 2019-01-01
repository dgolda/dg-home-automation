/*
 * TempSensors.cpp
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#include "TempSensors.h"
#include <print.h>

extern Print* debug;

TempSensors::TempSensors(DallasTemperature& asensors): dallasTemperature(asensors) {
	// TODO Auto-generated constructor stub

}

void TempSensors::addSensor(const char* label, const DeviceAddress deviceAddress) {
	if(sensorCount < MAX_SENSORS) {
		_tempSensors[sensorCount] = TempSensor(label, deviceAddress);
		sensorCount++;
	}
}

bool TempSensors::readTemp(int index) {
	DeviceAddress& deviceAddress = _tempSensors[index]._deviceAddress;
	float t = dallasTemperature.getTempC(deviceAddress);
	return _tempSensors[index].value.newValue(t, millis());
}

void TempSensors::setup() {
	dallasTemperature.setWaitForConversion(false);
	dallasTemperature.requestTemperatures();
	debug->println("TempSensors:");
	for (int i = 0; i < sensorCount; i++) {
		debug->print(i);
		debug->print(" ");
		debug->print(_tempSensors[i]._label);
		debug->print(" ");
		printAddress(_tempSensors[i]._deviceAddress);
		debug->print(" ");
		getAndPrintTemperature(_tempSensors[i]._deviceAddress);
		debug->println();
	}
}

void TempSensors::test() {
	// Start up the library
	debug->println("Setup sensors");
	diagSensors();
	// call sensors.requestTemperatures() to issue a global temperature
	// request to all devices on the bus
	debug->println("Requesting temperatures...");
	dallasTemperature.requestTemperatures(); // Send the command to get temperatures
	debug->println("DONE");

	// Loop through each device, print out temperature data
	for (int i = 0; i < numberOfDevices; i++) {
		// Search the wire for address
		if (dallasTemperature.getAddress(tempDeviceAddress, i)) {
			// Output the device ID
			debug->print("Temperature for device: ");
			debug->print(i, DEC);
			debug->print("   ");

			// It responds almost immediately. Let's print out the data
			getAndPrintTemperature(tempDeviceAddress); // Use a simple function to print out the data
		}
		//else ghost device! Check your power requirements and cabling

	}
}

void TempSensors::diagSensors() {
	// Start up the library
	dallasTemperature.begin();
	// Grab a count of devices on the wire
	numberOfDevices = dallasTemperature.getDeviceCount();
	// locate devices on the bus
	debug->println("Locating devices...");
	debug->print("Found ");
	debug->print(numberOfDevices, DEC);
	debug->println(" devices.");
	// report parasite power requirements
	debug->print("Parasite power is: ");
	if (dallasTemperature.isParasitePowerMode())
		debug->println("ON");
	else
		debug->println("OFF");

	// Loop through each device, print out address
	for (int i = 0; i < numberOfDevices; i++) {
		// Search the wire for address
		if (dallasTemperature.getAddress(tempDeviceAddress, i)) {
			debug->print("Found device ");
			debug->print(i, DEC);
			debug->print(" with address: ");
			printAddress(tempDeviceAddress);
			debug->println();
			debug->print("Setting resolution to ");
			debug->println(TEMPERATURE_PRECISION, DEC);
			// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
			dallasTemperature.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
			debug->print("Resolution actually set to: ");
			debug->print(dallasTemperature.getResolution(tempDeviceAddress), DEC);
			debug->println();
		} else {
			debug->print("Found ghost device at ");
			debug->print(i, DEC);
			debug->print(" but could not detect address. Check power and cabling");
		}
	}
}

// function to print a device address
void TempSensors::printAddress(DeviceAddress deviceAddress) {
	for (uint8_t i = 0; i < 8; i++) {
		if (deviceAddress[i] < 16)
			debug->print("0");
		debug->print(deviceAddress[i], HEX);
	}
}

// function to print the temperature for a device
void TempSensors::getAndPrintTemperature(DeviceAddress deviceAddress) {
	float tempC = dallasTemperature.getTempC(deviceAddress);
	debug->print("Temp C: ");
	debug->print(tempC);
	debug->println();
}

