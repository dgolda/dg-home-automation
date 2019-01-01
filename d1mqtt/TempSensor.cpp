/*
 * TempSensor.cpp
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#include "TempSensor.h"

TempSensor::TempSensor() : _label("?")  {

}

TempSensor::TempSensor(const char* label, const uint8_t deviceAddress[8]) : _label(label) {
	copyAddress(deviceAddress, _deviceAddress);
}

