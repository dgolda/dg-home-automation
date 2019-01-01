/*
 * MqttPrint.cpp
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#include "DgMqttPrint.h"

DgMqttPrint::DgMqttPrint(DgMqttHelper& mqtt): _mqtt(mqtt), _pos(0) {
	// TODO Auto-generated constructor stub

}


size_t DgMqttPrint::write(uint8_t byte) {
  _buffer[_pos++] = byte;
  if(byte == '\n' || _pos == MAX_MSG_SIZE-2) {
	  _buffer[_pos] = 0;
	  send();
	  _pos = 0;
  }
  return 1;
}


void DgMqttPrint::send() {
	_mqtt.publish("debug", _buffer, _pos, true);
}
