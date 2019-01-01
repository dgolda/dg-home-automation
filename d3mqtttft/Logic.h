/*
 * Logic.h
 *
 *  Created on: 5 maj 2018
 *      Author: Damian
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include <Print.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "Panel.h"
#include "BarGraph.h"

class Logic {
public:
	Logic();
	void init(Adafruit_ILI9341* display, uint8_t pinLed);
	void onZasilanieChanged(double num);
	void onZasobnikGoraChanged(double num);
	void onZasobnikDolChanged(double num);
	void loop();
private:
	void draw();
	char trend(double num, double prevNum);
	void ledOn();
	void ledOff();
	void drawPage0();
	void drawPage1();
	void drawPage2();

	Adafruit_ILI9341* _display;
	Panel* _screen;
	BarGraph* _bar ;
	double _zasilanie;
	double _gora;
	double _dol;
	double _prevZasilanie;
	double _prevGora;
	double _prevDol;
	unsigned long _lastMsg;
	int _page;
	bool _load;
	double _avg;
	uint8_t _pinLed;
};

#endif /* LOGIC_H_ */
