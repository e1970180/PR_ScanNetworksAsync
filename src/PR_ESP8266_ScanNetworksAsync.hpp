#pragma once

#include <arduino.h>
#include "ESP8266WiFi.h"

static const char  F_NOTSTARTED[] PROGMEM	=	"Scanning has not been triggered";
static const char  F_INPROGRESS[] PROGMEM	=	"Scanning in progress";
static const char  F_NOTFOUND[] PROGMEM	=		"No networks find";

static const char  F_P[] PROGMEM	=	"<p>";
static const char  F_PP[] PROGMEM	=	"</p>";

static const char  F_SCANWIFI_TABLE[] PROGMEM	= "\
<table> \
<caption>{tableheader}</caption> \
 <tbody> \
  <tr> \
   <th>No</th> \
   <th>SSID</th> \
   <th>Channel</th> \
   <th>RSSI (dBm)</th> \
   <th>Encript</th> \
  </tr> \
 </tbody> \
 <tbody> \
  <!--row--> \
 </tbody> \
</table> ";

#define  TABLE_HEARED_SGN			"{tableheader}"
#define  NO_SGN			"{no}"
#define  SSID_SGN		"{ssid}"
#define  CHANNEL_SGN	"{channel}"
#define  RSSI_SGN		"{rssi}"
#define  ENCRIPT_SGN	"{encrypt}"

#define  ROW_SGN		"<!--row-->"

static const char  F_SCANWIFI_ROW5[] PROGMEM	= " \
<tr> \
 <td>{no}</td> \
 <td>{ssid}</td> \
 <td>{channel}</td> \
 <td>{rssi}</td> \
 <td>{encrypt}</td> \
</tr> \
<!--row-->";


class	PR_ScanNetworksAsyncClass {
	
	public:
		bool	start();
		bool	resultPrint();
		bool	resultTXT( String& scanResultTXT);
		bool	resultHTML( String& scanResultHTML);
		void	clearResult();
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

bool	PR_ScanNetworksAsyncClass::resultPrint() {
	
	int	n = WiFi.scanComplete();
	switch ( n ) {
		case -2: //Scanning has not been triggered
			Serial.println(FPSTR(F_NOTSTARTED));
			break;
		case -1: //Scanning still in progress
			Serial.println(FPSTR(F_INPROGRESS));
			break;
		case 0: //no networks find
			Serial.println(FPSTR(F_NOTFOUND));
			break;
		default: { // we have results
		
			Serial.printf("%d network(s) found\n", n);
			for (int i = 0; i < n; i++)
			{
			  Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");		
			}	
		}
	}//switch	
}

bool	PR_ScanNetworksAsyncClass::resultTXT( String& scanResultTXT ) {
	
	int	n = WiFi.scanComplete();
	switch ( n ) {
		case -2: //Scanning has not been triggered
			scanResultTXT = FPSTR(F_NOTSTARTED);
			break;
		case -1: //Scanning still in progress
			scanResultTXT = FPSTR(F_INPROGRESS);
			break;
		case 0: //no networks find
			scanResultTXT = FPSTR(F_NOTFOUND);
			break;
		default: { // we have results
		
			scanResultTXT = String(n) + " network(s) found\n";
			for (int i = 0; i < n; i++)
			{
			  scanResultTXT += String(i+1) + "\t" ;
			  scanResultTXT += WiFi.SSID(i) + "\t ch:";
			  scanResultTXT += String( WiFi.channel(i) ) + "\t";
			  scanResultTXT += String( WiFi.RSSI(i) ) + "\t";
			  scanResultTXT += WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "secure" ;
			  scanResultTXT += "\n";
			}		
		}
	}//switch	
}

bool	PR_ScanNetworksAsyncClass::resultHTML( String& scanResultHTML) {
	
	int	n = WiFi.scanComplete();
	switch ( n ) {
		case -2: //Scanning has not been triggered
			scanResultHTML = FPSTR(F_P);
			scanResultHTML += FPSTR(F_NOTSTARTED);
			scanResultHTML += FPSTR(F_PP);
			break;
		case -1: //Scanning still in progress
			scanResultHTML = FPSTR(F_P);
			scanResultHTML += FPSTR(F_INPROGRESS);
			scanResultHTML += FPSTR(F_PP);
			break;
		case 0: //no networks find
			scanResultHTML = FPSTR(F_P);
			scanResultHTML += FPSTR(F_NOTFOUND);
			scanResultHTML += FPSTR(F_PP);
			break;
		default: { // we have results
		
			scanResultHTML = FPSTR(F_SCANWIFI_TABLE);
			scanResultHTML.replace(TABLE_HEARED_SGN, String(n) + " network(s) found");
			
			for (int i = 0; i < n; i++)
			{
				String	rowHTML;
				rowHTML.reserve(255);
				rowHTML = FPSTR(F_SCANWIFI_ROW5);
				
				rowHTML.replace(NO_SGN, String(i+1));
				rowHTML.replace(SSID_SGN, String(WiFi.SSID(i)));
				rowHTML.replace(CHANNEL_SGN, String(WiFi.channel(i)));
				rowHTML.replace(RSSI_SGN, String(WiFi.RSSI(i)));
				rowHTML.replace(ENCRIPT_SGN, (WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "secure"));
				scanResultHTML.replace(ROW_SGN, rowHTML);
			}
		}
	}//switch	
}

void	PR_ScanNetworksAsyncClass::clearResult() {
	
	WiFi.scanDelete();
}