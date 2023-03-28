#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//=============================================MOTION SENSOR DEVICE ID==============================================
String id = "sensor 1";
//==================================================================================================================
int prevstate = LOW;
String d;
String data;
//============================================WIFI CREDENTIALS OF HUB================================================
const char* ssid = "server1";
const char* password = "qwertyuiop";
String host = "http://10.10.10.1/feed?";
//============================================defining the object====================================================
WiFiClient client;
HTTPClient http;
//============================================defining the IP Address of the sensor==================================
IPAddress ip (10, 10, 10, 32);
IPAddress gateway (10, 10, 10, 1);
IPAddress subnet (255, 255, 255, 0);

//============================================SETUP PART=============================================================
void setup() {
  pinMode(2, INPUT);
  Serial.begin(115200);
  ESP.eraseConfig();
  WiFi.persistent(false);
  Serial.println("SET ESP IN STA MODE");
  WiFi.mode(WIFI_STA);
  Serial.println("- connecting to wifi");
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println(".");
  Serial.println("- wifi connected");
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
//============================================READ THE STATE OF SENSOR================================================
void readdata() {

  int currentstate = digitalRead(2);
  if (prevstate != currentstate) {
    if (currentstate == HIGH) {
      d = "high";
      data = "ID=";
      data += String(id);
      data += "&state=";
      data += String(d);
      http.begin(host);
      http.addHeader("content-type", "application/x-www-form-urlencoded");
      http.POST(data);
      http.writeToStream(&Serial);
      http.end();
      Serial.println("data stream: " + data);
    } else {
      d = "low";
      data = "ID=";
      data += String(id);
      data += "&state=";
      data += String(d);
      http.begin(host);
      http.addHeader("content-type", "application/x-www-form-urlencoded");
      http.POST(data);
      http.writeToStream(&Serial);
      http.end();
      Serial.println("data stream: " + data);
    }
  } prevstate = currentstate;
}


//============================================LOOP PART================================================
void loop () {
  readdata();

}
