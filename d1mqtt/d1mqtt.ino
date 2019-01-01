/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
 - publishes "hello world" to the topic "outTopic" every two seconds
 - subscribes to the topic "inTopic", printing out any messages
 it receives. NB - it assumes the received payloads are strings not binary
 - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
 else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
 - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
 http://arduino.esp8266.com/stable/package_esp8266com_index.json
 - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
 - Select your ESP8266 in "Tools -> Board"

 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Print.h>

#include "DgOtaHelper.h"

#include "DgHundreths.h"

#include "DgMqttHelper.h"

#include "DgMqttPrint.h"
#include "TempSensors.h"
#include "DgNetwork.h"


WiFiClient espClient;
DgMqttHelper mqtt;
DgOtaHelper otaHelper;

long lastMsg = 0;
char msg[50];
int value = 0;

Print* debug = &Serial;

void setupWifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

//	  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//	    Serial.println("Connection Failed! Rebooting...");
//	    delay(5000);
//	    ESP.restart();
//	  }

	randomSeed(micros());

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}


void onMqttMessage(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (uint16_t i = 0; i < length; i++) {
		Serial.print((char) payload[i]);
	}
	Serial.println();

	// Switch on the LED if an 1 was received as first character
	if ((char) payload[0] == '1') {
		digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
		// but actually the LED is on; this is because
		// it is acive low on the ESP-01)
	} else {
		digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
	}
}

void onMqttConnected(DgMqttHelper& mqttHelper) {
	mqttHelper.publish("outTopic", "hello world");
	// ... and resubscribe
	mqttHelper.subscribe("inTopic");
}


// **** Define here the right pin for your ESP module ****
/**
 * Piny Wemos D1 mini
 * All of the IO pins have interrupt/pwm/I2C/one-wire support except D0.
 * https://wiki.wemos.cc/products:d1:d1_mini
 * wersja 3:
 Pin	Function	ESP-8266 Pin
 TX	TXD	TXD
 RX	RXD	RXD
 A0	Analog input, max 3.3V input	A0
 D0	IO	GPIO16
 D1	IO, SCL	GPIO5
 D2	IO, SDA	GPIO4
 D3	IO, 10k Pull-up	GPIO0
 D4	IO, 10k Pull-up, BUILTIN_LED	GPIO2
 D5	IO, SCK	GPIO14
 D6	IO, MISO	GPIO12
 D7	IO, MOSI	GPIO13
 D8	IO, 10k Pull-down, SS	GPIO15
 G	Ground	GND
 5V	5V	-
 3V3	3.3V	3.3V
 RST	Reset	RST

 wersja 2.2
 Pin
 Pin	Function	ESP-8266 Pin
 TX	TXD	TXD
 RX	RXD	RXD
 A0	Analog input, max 3.3V input	A0
 D0	IO	GPIO16
 D1	IO, SCL	GPIO5
 D2	IO, SDA	GPIO4
 D3	IO, 10k Pull-up	GPIO0
 D4	IO, 10k Pull-up, BUILTIN_LED	GPIO2
 D5	IO, SCK	GPIO14
 D6	IO, MISO	GPIO12
 D7	IO, MOSI	GPIO13
 D8	IO, 10k Pull-down, SS	GPIO15
 G	Ground	GND
 5V	5V	-
 3V3	3.3V	3.3V
 RST	Reset	RST

 */

#define PIN_RELAY     D1 //relay shield, konflikt z SCL lcd i2c !!!
#define PIN_MYSDA D2 //SDA, lcd i2c
#define PIN_MYSCL D3
#define PIN_DALLAS D7
#define PIN_LED     BUILTIN_LED //led jest na D4

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

OneWire dallasWire(PIN_DALLAS);
DallasTemperature sensors(&dallasWire);
TempSensors tempSensors(sensors);
// DeviceAddress 28F46CD30500003C
DeviceAddress address = { 0x28, 0xF4, 0x6C, 0xD3, 0x05, 0x00, 0x00, 0x3C };

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

uint8_t measuredDevice = 0;
uint8_t displayedDevice = 0;
uint8_t tempsChanged;

void readTemp(int i) {
	if (tempSensors.readTemp(i)) {
		tempsChanged = 0;
		char topic[20];
		char payload[20];
		sprintf(topic, "temp/%s", tempSensors._tempSensors[i]._label);
		debug->print(topic);
		debug->print(" ");
		tempSensors._tempSensors[i].value.print(payload);
		debug->print(payload);
		debug->println();
		mqtt.publish(topic, payload, true);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.println("setup begin");
	pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
	DeviceAddress address1 = { 0x28, 0xF4, 0x6C, 0xD3, 0x05, 0x00, 0x00, 0x3C };//28 F4 6C D3 05 00 00 3C
	DeviceAddress address2 = { 0x28, 0x2C, 0xE2, 0xD2, 0x05, 0x00, 0x00, 0xBF };//28 2C E2 D2 05 00 00 BF
	DeviceAddress address3 = { 0x28, 0x3C, 0x48, 0xD2, 0x05, 0x00, 0x00, 0x6A };//28 3C 48 D2 05 00 00 6A

	tempSensors.addSensor("Zasilanie", address1);
	tempSensors.addSensor("ZasobnikDol", address2);
	tempSensors.addSensor("ZasobnikGora", address3);

	setupWifi();
	otaHelper.setup("esp8266-1");
	mqtt.setOnConnectedHandler(onMqttConnected);
	mqtt.setup(espClient, "d1", mqtt_server, MQTT_PORT);
	mqtt.setOnMessageHandler(onMqttMessage);

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_RELAY, OUTPUT);
	pinMode(PIN_MYSDA, OUTPUT);
	pinMode(PIN_MYSCL, OUTPUT);
	//Wire.begin(PIN_MYSDA, PIN_MYSCL);
	lcd.init(PIN_MYSDA, PIN_MYSCL);                      // initialize the lcd
// Print a message to the LCD.
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("OK.");
	mqtt.loop();//reconnect
	debug = new DgMqttPrint(mqtt);
	tempSensors.test();
	tempSensors.setup();
	Serial.println("setup end");

}


uint32_t lastDisplay;
uint32_t lastMeasure;

void printAllTemps() {
	if (tempsChanged == 0) {
		tempsChanged = 100;
		char buf[20];
		char payload[100];
		sprintf(payload, "%ld", millis());
		for (int i = 0; i < tempSensors.sensorCount; i++) {
			strcat(payload, ",");
			tempSensors._tempSensors[i].value.print(buf);
			strcat(payload, buf);
		}
		debug->print("!");
		debug->print(payload);
		debug->println();
		mqtt.publish("tempall", payload, true);
	} else {
		//Serial.println("No changes");
		tempsChanged--;
	}
}

void loop() {
	mqtt.loop();
	otaHelper.loop();

	long now = millis();
	if (now - lastMsg > 30000) {
		lastMsg = now;
		++value;
		snprintf(msg, 75, "alive #%d", value);
		Serial.print("Publish message: ");
		Serial.println(msg);
		mqtt.publish("heartbeat", msg, true);
	}

	if (now - lastDisplay > 2000) {
		lastDisplay = now;
		lcd.setCursor(0, 0);
		lcd.print(displayedDevice);
		lcd.setCursor(2, 0);
		tempSensors._tempSensors[displayedDevice].value.printTo(lcd);
		displayedDevice++;
		if (displayedDevice >= tempSensors.sensorCount) {
			displayedDevice = 0;
		}
	}

	if (now - lastMeasure > 100) {
		lastMeasure = now;
		//odczyt temperatury min 94 ms póŸniej po requestTemp
		measuredDevice = (measuredDevice + 1) % 9; //cykl 9*110 = 990ms
		if (measuredDevice < tempSensors.sensorCount) {
			readTemp(measuredDevice);
		} else if (measuredDevice == tempSensors.sensorCount) {
			printAllTemps();
			debug->println("request temp");
			sensors.requestTemperatures();
		}
	}

}

