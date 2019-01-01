/*
 * Tenth.h
 *
 *  Created on: 9 mar 2018
 *      Author: Damian
 */

#ifndef DGTENTH_H_
#define DGTENTH_H_
#include <print.h>

class DgTenth {
public:
	DgTenth();
	DgTenth(float value);
	int _valueTimes10;
	void print(Print& print);
	void print(char* buf);
};

#endif /* DGTENTH_H_ */
