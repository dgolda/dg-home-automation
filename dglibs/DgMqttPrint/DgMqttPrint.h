/*
 * MqttPrint.h
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#ifndef DGMQTTPRINT_H_
#define DGMQTTPRINT_H_

#include <Print.h>

#include "DgMqttHelper.h"

namespace {

const int MAX_MSG_SIZE = 100;

}

class DgMqttPrint: public Print {
public:
	DgMqttPrint(DgMqttHelper& mqtt);
    virtual size_t write(uint8_t byte);
private:
    DgMqttHelper& _mqtt;
    uint16_t _pos;
	uint8_t _buffer[MAX_MSG_SIZE];
    void send();
};

#endif /* DGMQTTPRINT_H_ */
