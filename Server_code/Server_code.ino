#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

//======================================WIFI CREDENTIALS for Hotspot==================================
const char* ssid = "server1";
const char* password = "qwertyuiop";
int max_connection = 8;
//====================================================================================================
String URL;
int httpCode;
String payload;
String ID;
String state;
//=============================================define the objects======================================
ESP8266WebServer server(80);
HTTPClient http;
//===============================================dafine the IP Address of the hotspot==================
IPAddress ip (10, 10, 10, 1);
IPAddress gateway (10, 10, 10, 1);
IPAddress subnet (255, 255, 255, 0);
//=========================================SETUP PART==================================================
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  Serial.println("Setup Access point");
  Serial.println("Disconnect from any other modes");
  WiFi.disconnect();
  Serial.println("stating access point with SSID" + String(ssid));
  WiFi.softAP(ssid, password, 1, false, max_connection);
  WiFi.softAPConfig(ip, gateway, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/", handleroot);
  server.on("/feed", feed);
  server.begin();
}
//=====================================handle root page================================================
void handleroot() {

  server.send(200, "text/plain", "hello");
}
//===================================handle door sensor page===========================================
void feed() {
  String data2 = server.arg("state");
  String data1 = server.arg("ID");
  server.send(200, "text/plain", "Response");
  Serial.println(data1 + "=" + data2);
}

//==============================================LOOP===================================================
void loop () {

  server.handleClient();
}
