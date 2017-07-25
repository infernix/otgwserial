#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>


// Edit to your liking in config.h.example and copy to config.h
#include "config.h"

SoftwareSerial swSer(GPIO_RX, GPIO_TX, false, 256);

#define UART_BAUD 9600
#define packTimeout 5 // ms (if nothing more on UART, then send packet)
#define bufferSize 8192

#define MODE_STA // ESP connects to WiFi router

#define PROTOCOL_TCP

const int port = 23;

WiFiServer server(port);
WiFiClient client;

uint8_t buf1[bufferSize];
uint8_t i1=0;

uint8_t buf2[bufferSize];
uint8_t i2=0;



void setup() {

  delay(50);

  swSer.begin(UART_BAUD);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_KEY);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  ArduinoOTA.begin();

  server.begin(); // start TCP server

}


void loop() {
  ArduinoOTA.handle();

  if(!client.connected()) { // if client not connected
    client = server.available(); // wait for it to connect
    return;
  }

  // here we have a connected client

  if(client.available()) {
    while(client.available()) {
      buf1[i1] = (uint8_t)client.read(); // read char from client 
      if(i1<bufferSize-1) i1++;
    }
    // now send to UART:
    swSer.write(buf1, i1);
    i1 = 0;
  }

  if(swSer.available()) {

    // read the data until pause:

    while(1) {
      if(swSer.available()) {
        buf2[i2] = (char)swSer.read(); // read char from UART
        if(i2<bufferSize-1) i2++;
      } else {
        //delayMicroseconds(packTimeoutMicros);
        delay(packTimeout);
        if(!swSer.available()) {
          break;
        }
      }
    }

    // now send to WiFi:
    client.write((char*)buf2, i2);
    i2 = 0;
  }

}
