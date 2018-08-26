#include "ESP8266WiFi.h"

#include "PR_ScanNetworksAsync.hpp"

#define BLINK_PERIOD 10000
long lastBlinkMillis;
boolean ledState;

#define SCAN_PERIOD 5000
long lastScanMillis;

PR_ScanNetworksAsyncClass	scanner;
String	answer;
void setup()
 {
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);

		//WiFi.mode(WIFI_OFF);
		//WiFi.softAP("PPPPPP");
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
}

void loop()
{
  long currentMillis = millis();

  // blink LED
  if (currentMillis - lastBlinkMillis > BLINK_PERIOD)
  {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    lastBlinkMillis = currentMillis;
	
	scanner.start();
  }

  
  scanner.result(answer);
  Serial.println(answer);
  
  Serial.println(WiFi.getMode());
  
  delay(500);
  
}