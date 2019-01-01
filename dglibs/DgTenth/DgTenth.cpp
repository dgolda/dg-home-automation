/*
 * Tenth.cpp
 *
 *  Created on: 9 mar 2018
 *      Author: Damian
 */

#include "DgTenth.h"

#include <math.h>

DgTenth::DgTenth() :
		_valueTimes10(-1270) {

}

DgTenth::DgTenth(float value) :
		_valueTimes10(lroundf(value * 10)) {

}

void DgTenth::print(Print& print) {
	print.print(_valueTimes10 / 10);
	print.print(".");
	print.print(_valueTimes10 % 10);
}

void DgTenth::print(char* buf) {
	sprintf(buf, "%d.%d", _valueTimes10 / 10, _valueTimes10 % 10);
}
