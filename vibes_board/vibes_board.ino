/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/***************************************************
  CSH Vibes
  Author: Willard Nilges for Computer Science House
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "credentials.h"

#define BAD_VIBES_SIGN 0
#define GOOD_VIBES_SIGN 1

/************ Global State ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Subscribe bad_vibes_duty_cycle = Adafruit_MQTT_Subscribe(&mqtt, "vibes/bad");
Adafruit_MQTT_Subscribe good_vibes_duty_cycle = Adafruit_MQTT_Subscribe(&mqtt, "vibes/good");

/*************************** Sketch Code ************************************/

void bad_vibes_callback(uint32_t duty_cycle) {
  if (duty_cycle > 255)
      duty_cycle = 255;
  analogWrite(BAD_VIBES_SIGN, duty_cycle);
}

void good_vibes_callback(uint32_t duty_cycle) {
  if (duty_cycle > 255)
      duty_cycle = 255;
  analogWrite(GOOD_VIBES_SIGN, duty_cycle);
}

void setup() {
  
  // Set up the LEDs that control the sign
  pinMode(BAD_VIBES_SIGN, OUTPUT);
  pinMode(GOOD_VIBES_SIGN, OUTPUT);

  //0-255
  analogWrite(BAD_VIBES_SIGN, 0);
  analogWrite(GOOD_VIBES_SIGN, 0);

  Serial.begin(115200);

  Serial.println(F("CSH Vibes v0.1"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("MAC Address is ");
  Serial.print(WiFi.macAddress());
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  bad_vibes_duty_cycle.setCallback(bad_vibes_callback);
  good_vibes_duty_cycle.setCallback(good_vibes_callback);
  
  // Setup MQTT subscription for feed.
  mqtt.subscribe(&bad_vibes_duty_cycle);
  mqtt.subscribe(&good_vibes_duty_cycle);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(10000);
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
