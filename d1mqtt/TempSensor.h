/*
 * TempSensor.h
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include <WString.h>
#include <DallasTemperature.h>

#include "DgHundreths.h"

class TempSensor {
public:
	TempSensor();
	TempSensor(const char* label, const uint8_t deviceAddress[8]);
	const char* _label;
	DeviceAddress _deviceAddress;
	DgHundreths value;
private:
	static uint8_t dummyDeviceAddress[8];
	void copyAddress(const DeviceAddress src, DeviceAddress dest) {
		for (int i = 0; i < 8; i++) dest[i] = src[i];
	}
};

#endif /* TEMPSENSOR_H_ */
