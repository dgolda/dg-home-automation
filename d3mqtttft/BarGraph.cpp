/*
 * BarGraph.cpp
 *
 *  Created on: 22 maj 2018
 *      Author: Damian
 */

#include "BarGraph.h"
#include <Adafruit_ILI9341.h>

BarGraph::BarGraph(int x, int y, int w, int h, int chunks, int minValue, int maxValue) :
Panel(x, y, w, h), _chunks(chunks), _minValue(minValue), _maxValue(maxValue) {

}

void BarGraph::setValue(int value, int trend) {
	_value = value;
	_trend = trend;
	int chunkValue = (_maxValue - _minValue) / _chunks;
	if (_value > _maxValue) {
		_visibleChunks = _chunks;
	} else {
		int v = _value - _minValue;
		if (v > 0) {
			_visibleChunks = v / chunkValue;
		} else {
			_visibleChunks = 0;
		}
	}
//	_showChunks = _visibleChunks;
}

void BarGraph::nextFrame() {
	if (_trend > 0) {
		_showChunks++;
		if (_showChunks > _visibleChunks) {
			_showChunks = 0;
		}
	} else if (_trend < 0) {
		if (_showChunks == 0) {
			_showChunks = _visibleChunks;
		} else {
			_showChunks--;
		}
	} else {
		_showChunks = _visibleChunks;
	}
}

void BarGraph::drawSelf(Adafruit_GFX* display) {
	int chunkHeight = _absoluteH / _chunks;
	for (int i = 0; i < _showChunks; i++) {
		display->fillRect(_absoluteX,
				_absoluteY + _absoluteH - chunkHeight * (i + 1), _absoluteW,
				chunkHeight - 1, ILI9341_RED);
	}
	for (int i = _showChunks; i < _chunks; i++) {
		display->drawRect(_absoluteX,
				_absoluteY + _absoluteH - chunkHeight * (i + 1), _absoluteW,
				chunkHeight - 1, ILI9341_BLUE);
	}
}
