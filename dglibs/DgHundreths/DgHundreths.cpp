/*
 * Hundreths.cpp
 *
 *  Created on: 9 mar 2018
 *      Author: Damian
 */

#include "DgHundreths.h"

#include <math.h>

DgHundreths::DgHundreths() :
		_valueTimes100(-12700), _timestamp(0l) {
}

bool DgHundreths::newValue(float value, uint32_t timestamp) {
	long int newValueTimes100 = lroundf(value * 100);
	if (differs(newValueTimes100, timestamp)) {
		_valueTimes100 = newValueTimes100;
		_timestamp = timestamp;
		return true;
	} else {
		return false;
	}
}

bool DgHundreths::differs(int newValueTimes100, uint32_t newTimestamp) {
	return (abs(newValueTimes100 / 10 - _valueTimes100 / 10) > 1)
			|| (newTimestamp - _timestamp > FORCE_REFRESH_MILLIS);
}

size_t DgHundreths::printTo(Print& print) const {
	if (_valueTimes100 > -12000) {
		return print.printf("%d.%02d", _valueTimes100 / 100,
				_valueTimes100 % 100);
	} else {
		return print.print("X");
	}
}

void DgHundreths::print(char* buf) {
	if (_valueTimes100 > -12000) {
		sprintf(buf, "%d.%02d", _valueTimes100 / 100, _valueTimes100 % 100);
	} else {
		sprintf(buf, "X");
	}
}
