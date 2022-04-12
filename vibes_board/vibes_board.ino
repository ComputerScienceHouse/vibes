/***************************************************
  CSH Vibes
  Author: Willard Nilges for Computer Science House
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "credentials.h"

// GPIO Pins
#define GOOD_VIBES_SIGN 1
#define BAD_VIBES_SIGN 3

WiFiClient client;
PubSubClient mqtt_client(MQTT_SERVER, MQTT_SERVERPORT, mqtt_callback, wifiClient);

int bad_glow = 10;
int good_glow = 10;

void bad_vibes_callback(uint32_t duty_cycle) {
  Serial.println("Got bad vibes.");
  Serial.println(duty_cycle);
  analogWrite(BAD_VIBES_SIGN, 0);
  delay(30);
  analogWrite(BAD_VIBES_SIGN, 100);
  delay(30);
  analogWrite(BAD_VIBES_SIGN, 0);
  delay(30);
  analogWrite(BAD_VIBES_SIGN, 100);
  if (duty_cycle > 255)
      duty_cycle = 255;
  bad_glow = (int) duty_cycle;
}

void good_vibes_callback(uint32_t duty_cycle) {
  Serial.println("Got good vibes.");
  Serial.println(duty_cycle);
  analogWrite(GOOD_VIBES_SIGN, 0);
  delay(30);
  analogWrite(GOOD_VIBES_SIGN, 100);
  delay(30);
  analogWrite(GOOD_VIBES_SIGN, 0);
  delay(30);
  analogWrite(GOOD_VIBES_SIGN, 100);
  if (duty_cycle > 255)
      duty_cycle = 255;
  good_glow = (int) duty_cycle;
}

void mqtt_callback(char* topic, byte* payload, unsigned int len) {
  
  String tmp=topic;

  Serial.print("mqtt_callback topic=");
  Serial.println(tmp);

  payload[length] = '\0'; // Make payload a string by NULL terminating it.
  int pwmVal = atoi((char *)payload);
  
  if(tmp.indexOf("vibes/good")>=0){
    Serial.println("Good vibes");
    Serial.println(pwmVal);
    analogWrite(GOOD_VIBES_SIGN, pwmVal);
  }
  
  if(tmp.indexOf("vibes/bad")>=0){
    Serial.println("Bad vibes");
    Serial.println(pwmVal);
    analogWrite(BAD_VIBES_SIGN, pwmVal);
  }
}



void mqtt_setup() {
  if (!mqtt_client.connected()) {
    uint8_t mac[6];
    WiFi.macAddress(mac);

    String clientName;
    clientName += "esp8266-";
    clientName += macToStr(mac);


    if (mqtt_client.connect( (char*) clientName.c_str())) {
      Serial.println("mqtt connected");
      if (mqtt_client.subscribe("/murilo/esp/rele/on")) {
        Serial.println("subcribe /murilo/esp/rele/on ok");
      } else {
        Serial.println("subcribe /murilo/esp/rele/on fail");
      }
      if (mqtt_client.subscribe("/murilo/esp/rele/off")) {
        Serial.println("subcribe /murilo/esp/rele/off ok");
      } else {
        Serial.println("subcribe /murilo/esp/rele/off fail");
      }
    } else {
      Serial.println("mqtt connect fail");
    }

  } 
  else {
    static int contador=0;
    contador++;
    String payload="";
    payload = contador;
    
     mqtt_client.publish("/murilo/esp/rele/count",(char*)payload.c_str());
  }
}


void setup() {
  
  // Set up the LEDs that control the sign
  pinMode(BAD_VIBES_SIGN, OUTPUT);
  pinMode(GOOD_VIBES_SIGN, OUTPUT);

  //0-255
  analogWrite(BAD_VIBES_SIGN, 50);
  analogWrite(GOOD_VIBES_SIGN, 50);

  delay(5000);

  Serial.begin(115200);

  Serial.println("CSH Vibes v0.1");

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // If it gets here, then it's probably just about ready to go.
  for (int i = 0; i < 255; i++){
    analogWrite(3, i);
    delay(10);
  }

  for (int i = 0; i < 255; i++){
    analogWrite(1, i);
    delay(10);
   }

  for (int i = 255; i > 0; i--){
    analogWrite(3, i);
    delay(10);
  }

  for (int i = 255; i > 0; i--){
    analogWrite(1, i);
    delay(10);
  }
  // Set up the LEDs that control the sign
  pinMode(3, OUTPUT);
  pinMode(1, OUTPUT);

}

void loop() {

  mqtt_setup();
  mqtt_client.loop();

  Serial.println("Dude what the fuck.");
  analogWrite(3, 0);
  analogWrite(1, 0);
  delay(5000);
  analogWrite(3, 200);
  analogWrite(1, 200);
  delay(5000);
  analogWrite(3, 0);
  analogWrite(1, 0);
  delay(5000);

  int wait_time = 50;
  for (int i = 0; i < 255; i++){
    analogWrite(3, i);
    delay(wait_time);
   }

  for (int i = 0; i < 255; i++){
    analogWrite(1, i);
    delay(wait_time);
  }
  for (int i = 255; i > 0; i--){
    analogWrite(3, i);
    delay(wait_time);
  }

  for (int i = 255; i > 0; i--){
    analogWrite(1, i);
    delay(wait_time);
  }
  
}
