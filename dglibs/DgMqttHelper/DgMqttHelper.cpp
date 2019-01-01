/*
 * MqttHelper.cpp
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#include "DgMqttHelper.h"

DgMqttHelper::DgMqttHelper() :
		_clientId(NULL), _topicPrefix(NULL) {
}

void DgMqttHelper::setup(Client& client, const char* clientId,
		const char * domain, uint16_t port) {
	_mqttClient = client;
	_topicPrefix = clientId;
	_clientId = clientId;
	_mqttClient.setClient(client);
	_mqttClient.setServer(domain, port);
	Serial.print("mqttClient state: ");
	Serial.println(_mqttClient.state());
	tryReconnect();
}

const char * DgMqttHelper::randomClientId() {
	String clientId = "ESP8266Client-";
	clientId += String(random(0xffff), HEX);
	return clientId.c_str();
}

void DgMqttHelper::setOnMessageHandler(MQTT_CALLBACK_SIGNATURE) {
	_mqttClient.setCallback(callback);
}

void DgMqttHelper::setOnConnectedHandler(MQTT_CONNECTED_SIGNATURE) {
	this->onConnectedHandler = onConnectedHandler;
}

boolean DgMqttHelper::publish(const char* topic, const char* payload) {
	return publish(topic, (const uint8_t*) payload, strlen(payload), false);

}

boolean DgMqttHelper::publish(const char* topic, const char* payload,
		boolean retained) {
	return publish(topic, (const uint8_t*) payload, strlen(payload), retained);
}

boolean DgMqttHelper::publish(const char* topic, const uint8_t* payload,
		unsigned int plength, boolean retained) {
	char buf[MQTT_MAX_PACKET_SIZE];
	strcpy(buf, _topicPrefix);
	strcat(buf, "/");
	strcat(buf, topic);
	return _mqttClient.publish(buf, payload, plength, retained);
}

boolean DgMqttHelper::subscribe(const char* topic) {
	Serial.print("subscribe ");
	Serial.print(topic);
	Serial.print(": ");
	boolean result = _mqttClient.subscribe(topic);
	Serial.println(result);
	return result;
}

boolean DgMqttHelper::subscribe(const char* topic, uint8_t qos) {
	return _mqttClient.subscribe(topic, qos);
}

void DgMqttHelper::loop() {
	if (!_mqttClient.connected()) {
		tryReconnect();
	}
	_mqttClient.loop();
}

boolean DgMqttHelper::tryReconnect() {
	Serial.print("Attempting MQTT connection...");
	// Create a random client ID
	// Attempt to connect
	if (_mqttClient.connect(_clientId)) {
		Serial.println("connected");
		if (onConnectedHandler != NULL) {
			Serial.println("calling onConnectedHandler");
			onConnectedHandler(*this);
		}else {
			Serial.println("no onConnectedHandler");
		}
		return true;
	} else {
		Serial.print("failed, rc=");
		Serial.print(_mqttClient.state());
		Serial.println(" try again in 5 seconds");
		// Wait 5 seconds before retrying
		delay(5000);
		return false;
	}

}

