/*
 * Panel.cpp
 *
 *  Created on: 22 maj 2018
 *      Author: Damian
 */

#include "Panel.h"

Panel::Panel(int x, int y, int w, int h) :
		_x(x), _y(y), _w(w), _h(h), //
		_absoluteX(x), _absoluteY(y), _absoluteW(w), _absoluteH(h), //
		_siblings( NULL), _children(NULL) {

}

void Panel::move(int parentAbsoluteX, int parentAbsoluteY, int parentW,
		int parentH) {
	_absoluteX = parentAbsoluteX + _x;
	_absoluteY = parentAbsoluteY + _y;
	_absoluteW = _w > 0 ? _w : parentW - _x + _w;
	_absoluteH = _h > 0 ? _w : parentH - _y + _h;
	if (_siblings != NULL) {
		_siblings->move(parentAbsoluteX, parentAbsoluteY, parentW, parentH);
	}
	if (_children != NULL) {
		_children->move(_absoluteX, _absoluteY, _absoluteW, _absoluteH);
	}
}


void Panel::drawSelf(Adafruit_GFX& display) {
	//display.drawRect(_absoluteX, _absoluteY, _absoluteW, _absoluteH, 1);
}

void Panel::draw(Adafruit_GFX& display) {
	drawSelf(display);

	if (_siblings != NULL) {
		_siblings->draw(display);
	}
	if (_children != NULL) {
		_children->draw(display);
	}
}

void Panel::addSibling(Panel& s) {
	if (_siblings == NULL) {
		_siblings = &s;
	} else {
		_siblings->addSibling(s);
	}
}

void Panel::addChild(Panel& s) {
	if (_children == NULL) {
		_children = &s;
	} else {
		_children->addSibling(s);
	}
	s.move(_absoluteX, _absoluteY, _absoluteW, _absoluteH);
}
