/*
 * Logic.cpp
 *
 *  Created on: 5 maj 2018
 *      Author: Damian
 */

#include "Logic.h"
#include "Panel.h"
#include "BarGraph.h"

void Logic::ledOn() {
	return digitalWrite(_pinLed, LOW);
}

void Logic::ledOff() {
	return digitalWrite(_pinLed, HIGH);
}

Logic::Logic(Adafruit_ILI9341& display, uint8_t pinLed) :
		_display(display), _pinLed(pinLed) {
	// text display tests
	_display.setTextSize(1);
	_display.setTextColor(ILI9341_WHITE);
	_display.setCursor(0, 0);
	_display.println("Hello, world!");
	ledOff();
	_screen = new Panel(0, 0, _display.width(), _display.height());
	_bar = new BarGraph(32, 2, 30, -2, 6, 38, 55);
	_screen->addChild(*_bar);

}

void Logic::onZasilanieChanged(double num) {
	Serial.print("Zasilanie: ");
	Serial.println(num);
	_prevZasilanie = _zasilanie;
	_zasilanie = num;
	draw();
}

void Logic::onZasobnikGoraChanged(double num) {
	Serial.print("Zasobnik gora: ");
	Serial.println(num);
	_prevGora = _gora;
	_gora = num;
	draw();
}

void Logic::onZasobnikDolChanged(double num) {
	Serial.print("Zasobik dol: ");
	Serial.println(num);
	_prevDol = _dol;
	_dol = num;
	draw();
}

char Logic::trend(double num, double prevNum) {
	if (num > prevNum) {
		return '/';
	} else if (num < prevNum) {
		return '\\';
	}
	return '-';
}

void Logic::loop() {
	long now = millis();
	if (now - _lastMsg > 1000) {
		_lastMsg = now;
		_page++;
		_load = _zasilanie > _prevZasilanie;
		_avg = (_gora + _dol) / 2;

		draw();
	}
}

void Logic::drawPage0() {
	_display.setCursor(0, 0);
	_display.print("Z:");
	_display.print(_zasilanie);
	_display.print(" ");
	_display.print(trend(_zasilanie, _prevZasilanie));
	_display.setCursor(0, 10);
	_display.print("G:");
	_display.print(_gora);
	_display.print(" ");
	_display.print(trend(_gora, _prevGora));
	_display.setCursor(0, 20);
	_display.print("D:");
	_display.print(_dol);
	_display.print(" ");
	_display.print(trend(_dol, _prevDol));
	_display.setCursor(0, 30);
	_display.print("*");
	if (_load) {
		_display.print("lad");
	}
}


void Logic::drawPage1() {
	//64x48
//	int margin = 1;
//	_display.drawRect(0 + margin, 0 + margin, _display.width() - 2 * margin,
//			_display.height() - 2 * margin, WHITE);
//	int barHeight = _display.height() / 5;
//	int margin2 = 2;
//	_display.fillRect(0 + margin2, 0 + margin2, _display.width() - 2 * margin2,
//			barHeight, WHITE);

	_bar->setValue(_gora, _load ? 1 : 0);
	_bar->nextFrame();
	_screen->draw(_display);

//	_display.setCursor(0, 0);
//	_display.print(_page);
//	_display.setCursor(0, 10);
//	_display.print(_bar->_showChunks);
//	_display.setCursor(0, 20);
//	_display.print(_bar->_visibleChunks);
//	_display.setCursor(0, 30);
//	_display.print(".");
//	_display.print(_avg);
}

void Logic::drawPage2() {
	_display.setCursor(0, 30);
//	_display.setTextColor(ILI9341_WHITE);
//	_display.setTextSize(1);
	_display.print(".");
}

void Logic::draw() {
	_display.fillScreen(ILI9341_BLACK);
	if (_page < 10 ) {
		drawPage0();
	} else if (_page < 20) {
		drawPage1();
//	} else if (_page < 21) {
//		drawPage2();
	} else {
		_page = 0;
	}

//	_display.display();

	if (_page &1 ==1 && _load) {
		ledOn();
	} else {
		ledOff();
	}
}
