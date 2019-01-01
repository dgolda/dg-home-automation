/*
 * Hundreths.h
 *
 *  Created on: 9 mar 2018
 *      Author: Damian
 */

#ifndef DGHUNDRETHS_H_
#define DGHUNDRETHS_H_

#include <print.h>

class DgHundreths: public Printable {
public:
	DgHundreths();

	bool newValue(float value, uint32_t timestamp);

	bool differs(int newValueTimes100, uint32_t newTimestamp);

	virtual size_t printTo(Print& p) const;

	void print(char* buf);

	int _valueTimes100;
	uint32_t _timestamp;
private:
	static const int FORCE_REFRESH_MILLIS = 60 * 1000;
};

#endif /* DGHUNDRETHS_H_ */
