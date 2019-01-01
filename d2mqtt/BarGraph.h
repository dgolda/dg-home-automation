/*
 * BarGraph.h
 *
 *  Created on: 22 maj 2018
 *      Author: Damian
 */

#ifndef BARGRAPH_H_
#define BARGRAPH_H_

#include "Panel.h"

class BarGraph: public Panel {
public:
	BarGraph(int x, int y, int w, int h, int chunks, int value, int range);
	void setValue(int value, int trend);
	void nextFrame();
	int _chunks;
	int _value;
	int _trend;
	int _minValue;
	int _maxValue;
	int _frame;
	int _visibleChunks;
	int _showChunks; //for this frame
protected:
	virtual void drawSelf(Adafruit_GFX& display);

};

#endif /* BARGRAPH_H_ */
