#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2019-01-01 16:15:11

#include "Arduino.h"
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
extern WiFiClient espClient;
extern DgMqttHelper mqtt;
extern DgOtaHelper otaHelper;
extern long lastMsg;
extern char msg[];
extern int value;
extern Print* debug;
#define PIN_RELAY     D1
#define PIN_MYSDA D2
#define PIN_MYSCL D3
#define PIN_DALLAS D7
#define PIN_LED     BUILTIN_LED
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

void setupWifi() ;
void onMqttMessage(char* topic, byte* payload, unsigned int length) ;
void onMqttConnected(DgMqttHelper& mqttHelper) ;
void readTemp(int i) ;
void setup() ;
void printAllTemps() ;
void loop() ;

#include "d1mqtt.ino"


#endif
