/*
 * MqttHelper.h
 *
 *  Created on: 10 mar 2018
 *      Author: Damian
 */

#ifndef DGMQTTHELPER_H_
#define DGMQTTHELPER_H_

#include <PubSubClient.h>


#ifdef ESP8266
#include <functional>
#define MQTT_CONNECTED_SIGNATURE std::function<void(DgMqttHelper& mqttHelper)> onConnectedHandler
#else
#define MQTT_CONNECTED_SIGNATURE void (*onConnectedHandler)(DgMqttHelper& mqttHelper)
#endif


class DgMqttHelper {
public:
	DgMqttHelper();
	void setup(Client& client, const char* clientId, const char * domain, uint16_t port);
	void setOnMessageHandler(MQTT_CALLBACK_SIGNATURE);
	void setOnConnectedHandler(MQTT_CONNECTED_SIGNATURE);
	void loop();
	//retained=false
	boolean publish(const char* topic, const char* payload);
	boolean publish(const char* topic, const char* payload, boolean retained);
	boolean publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);

	//qos = 0
	boolean subscribe(const char* topic);
	boolean subscribe(const char* topic, uint8_t qos);

	static const char * randomClientId();

private:
	boolean tryReconnect();
	MQTT_CONNECTED_SIGNATURE;

	PubSubClient _mqttClient;
	const char* _clientId;
	const char* _topicPrefix; //bez '/'
};

#endif /* DGMQTTHELPER_H_ */
