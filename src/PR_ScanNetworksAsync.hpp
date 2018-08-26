#pragma once

#include <arduino.h>
#include "ESP8266WiFi.h"

class	PR_ScanNetworksAsyncClass {
	
	public:
		bool	start();
		bool	result();
		bool	result( String& resStr);
	protected:
	
};


bool	PR_ScanNetworksAsyncClass::start() {

	if ( WiFi.getMode() == WIFI_OFF ) {
		Serial.println("\n WiFi if off ");
		return false;
	}

	//if will enable STA mode inside  Wifi.scanNetworks and disconnect
	if ( WiFi.scanComplete() <0 ) {
		WiFi.scanNetworks(true, true);		//start async and show hidden
		Serial.println("\nScan start ... ");
	}
	else {
		Serial.println("\nScan started already ");
	}
	return true;
}	

bool	PR_ScanNetworksAsyncClass::result() {
	
	int	n = WiFi.scanComplete();
	switch ( n ) {
		case -2: //Scanning has not been triggered
			Serial.println("Scanning has not been triggered");
			break;
		case -1: //Scanning still in progress
			Serial.println("Scanning still in progress");
			break;
		case 0: //no networks find
			Serial.println("no networks find");
			break;
		default: { // we have results
		
			Serial.printf("%d network(s) found\n", n);
			for (int i = 0; i < n; i++)
			{
			  Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
			
			}
			WiFi.scanDelete();		
		}
	}//switch	
}

bool	PR_ScanNetworksAsyncClass::result(String& resStr) {
	
	int	n = WiFi.scanComplete();
	switch ( n ) {
		case -2: //Scanning has not been triggered
			resStr = "Scanning has not been triggered";
			break;
		case -1: //Scanning still in progress
			resStr = "Scanning in progress";
			break;
		case 0: //no networks find
			resStr = "no networks find";
			break;
		default: { // we have results
		
			resStr = String(n) + "network(s) found\n";
			for (int i = 0; i < n; i++)
			{
			  resStr += String(i+1) + "\t" ;
			  resStr += WiFi.SSID(i) + "\t ch:";
			  resStr += String( WiFi.channel(i) ) + "\t";
			  resStr += String( WiFi.RSSI(i) ) + "\t";
			  resStr += WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "secure" ;
			  resStr += "\n";
			}
			WiFi.scanDelete();		
		}
	}//switch	
}