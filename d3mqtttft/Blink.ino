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

//#define PIN_RELAY     D1 //relay shield, konflikt z SCL lcd i2c !!!
//#define PIN_MYSDA D2 //SDA, lcd i2c
//#define PIN_MYSCL D3
//#define PIN_DALLAS D7
#define PIN_LED     LED_BUILTIN //led jest na D4
// SCL GPIO5
// SDA GPIO4
//#define OLED_RESET 0  // GPIO0 D3?

#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <Print.h>

//#include "DgOtaHelper.h"

#include "DgMqttHelper.h"

#include "DgMqttPrint.h"

const char* D1_TEMP_ZASOBNIK_DOL = "d1/temp/ZasobnikDol";
const char* D1_TEMP_ZASOBNIK_GORA = "d1/temp/ZasobnikGora";
const char* D1_TEMP_ZASILANIE = "d1/temp/Zasilanie";
const char* NODE_ID = "d3";

#include "DgNetwork.h"

//ESP_1B5F68	192.168.2.114	b4:e6:2d:1b:5f:68
const char* hostname = "esp8266-4";

WiFiClient espClient;
DgMqttHelper mqtt;
//DgOtaHelper otaHelper;

long lastMsg = 0;
char msg[50];
int value = 0;

Print* debug = &Serial;

#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC D8
#define TFT_CS D0

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC);

#include "Logic.h"
Logic logic = Logic();


void setupWifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
	     would try to act as both a client and an access-point and could cause
	     network-issues with your other WiFi-devices on your WiFi-network. */
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	WiFi.setAutoReconnect(true);

	int counter = 0;
	wl_status_t lastStatus = WL_NO_SHIELD;
	while (lastStatus != WL_CONNECTED) {
		wl_status_t status = WiFi.status();
		if (lastStatus != status || counter == 0) {
			counter = 120;
			Serial.println("");
			switch (status) {
			case WL_NO_SHIELD:
				Serial.println("WiFi not connected - no shield");
				break;
			case WL_IDLE_STATUS:
				Serial.println("WiFi not connected - idle status");
				break;
			case WL_NO_SSID_AVAIL:
				Serial.println("WiFi not connected - no SSID available");
				break;
			case WL_SCAN_COMPLETED:
				Serial.println("WiFi not connected - scan completed");
				break;
			case WL_CONNECTED:
				Serial.println("WiFi connected");
				break;
			case WL_CONNECT_FAILED:
				Serial.println("WiFi not connected - connect failed");
				break;
			case WL_CONNECTION_LOST:
				Serial.println("WiFi not connected - connection lost");
				break;
			case WL_DISCONNECTED:
				Serial.println("WiFi not connected - disconnected");
				break;
			}
			lastStatus = status;
		} else {
			Serial.print(".");
			delay(500);
			counter--;
		}
	}
	Serial.println("");

	  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
	    Serial.println("Connection Failed! Rebooting...");
	    delay(5000);
	    ESP.restart();
	  }

	randomSeed(micros());

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

double parsePayload(byte* payload, unsigned int length) {
	char buf[30];
	strncpy(buf, (char*) payload, length);
	buf[length] = 0;
	double num = strtod(buf, NULL);
	return num;
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (uint16_t i = 0; i < length; i++) {
		Serial.print((char) payload[i]);
	}
	Serial.println();

	if (strcmp(topic, D1_TEMP_ZASILANIE) == 0) {
		double num = parsePayload(payload, length);
		logic.onZasilanieChanged(num);
	}
	if (strcmp(topic, D1_TEMP_ZASOBNIK_GORA) == 0) {
		double num = parsePayload(payload, length);
		logic.onZasobnikGoraChanged(num);
	}
	if (strcmp(topic, D1_TEMP_ZASOBNIK_DOL) == 0) {
		double num = parsePayload(payload, length);
		logic.onZasobnikDolChanged(num);
	}
}

void onMqttConnected(DgMqttHelper& mqttHelper) {
	Serial.println("onMqttConnected");
	debug->println("welcome again");
	// ... and resubscribe
	mqttHelper.subscribe(D1_TEMP_ZASILANIE);
	mqttHelper.subscribe(D1_TEMP_ZASOBNIK_GORA);
	mqttHelper.subscribe(D1_TEMP_ZASOBNIK_DOL);
}

void setup() {
	Serial.begin(115200);
	Serial.println("setup begin");

	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(); // initialize with the I2C addr 0x3C (for the 64x48)
	display.setRotation(1);
	display.fillScreen(ILI9341_BLACK);
	display.setCursor(0, 0);
	display.setTextColor(ILI9341_WHITE);
	display.setTextSize(1);
	display.println("Starting...");
	debug = &display;
	// init done

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
//	display.display();

	setupWifi();
//	otaHelper.setup(hostname);
	mqtt.setOnConnectedHandler(onMqttConnected);
	mqtt.setup(espClient, NODE_ID, MQTT_SERVER, MQTT_PORT);
	mqtt.setOnMessageHandler(onMqttMessage);

//	pinMode(PIN_LED, OUTPUT);
//	pinMode(PIN_RELAY, OUTPUT);

	Serial.println("setup end");
	debug->println("Setup end");

	// Clear the buffer.
	display.fillScreen(ILI9341_BLACK);
	//display.display();
	logic.init(&display, PIN_LED);
}

void loop() {
	mqtt.loop();
//	otaHelper.loop();
	logic.loop();

	long now = millis();
	if (now - lastMsg > 1000) {
		lastMsg = now;
		++value;
		snprintf(msg, 75, "alive #%d", value);
		Serial.print("Publish message: ");
		Serial.println(msg);
//		display.println(msg);
		mqtt.publish("heartbeat", msg, true);
	}
}

