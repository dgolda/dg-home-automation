/*
 * Panel.h
 *
 *  Created on: 22 maj 2018
 *      Author: Damian
 */

#ifndef PANEL_H_
#define PANEL_H_

#include "Adafruit_GFX.h"

class Panel {
public:
	Panel(int x, int y, int w, int h);
	void move(int parentX, int parentY, int parentW, int parentH);
	void draw(Adafruit_GFX& display);

	void addChild(Panel& s);
	void addSibling(Panel& s);
	int _x;
	int _y;
	int _w;
	int _h;
	int _absoluteX;
	int _absoluteY;
	int _absoluteW;
	int _absoluteH;
	Panel* _siblings;
	Panel* _children;

protected:
	virtual void drawSelf(Adafruit_GFX& display);
};

#endif /* PANEL_H_ */
