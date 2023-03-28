#include <RH_NRF24.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String apiKey = "6SH8DMZKM5ONSO6S";

const char* ssid = "CUTM";
const char* password = "cit@cutm";

WiFiClient client;
int gatewayID = EEPROM.read(0);

const char* server = "api.thingspeak.com";

RH_NRF24 nrf24(2, 4); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather

void setup()
{
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  Serial.print("Receiver Started, ID: ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(gatewayID);
  Serial.println();
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to ");
  display.setTextSize(2);
  display.print(ssid);
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("WiFi connected");
  display.display();
  delay(4000);
  nrf24.init();
  nrf24.setChannel(3);
  nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm);
}

void loop()
{
  if (nrf24.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {

      // Send a reply
      uint8_t sdata[] = "Data Received.";
      nrf24.send(sdata, sizeof(sdata));
      nrf24.waitPacketSent();

      int humidity = buf[0];
      int temperature = buf[1];
      int deviceID = buf[2];
      float h = buf[3];

      Serial.println("--- Data retrieved from device ---");
      if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(temperature);
        postStr += "&field2=";
        postStr += String(humidity);
        postStr += "&field3=";
        postStr += String(gatewayID);
        postStr += "&field4=";
        postStr += String(deviceID);
        postStr += "\r\n\r\n";
        postStr += "&field5=";
        postStr += String(h/1023*100);
        postStr += "r\n";


        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

        Serial.println("---- Data sent to Thingspeak ----");
        Serial.print("Device ID: ");
        Serial.print(deviceID);
        Serial.print(", Temperature:");
        Serial.print(temperature);
        Serial.print(" C");
        Serial.print(", Humidity:");
        Serial.println(humidity);
        Serial.print("Carbon Monoxide:");
        Serial.println(h);
        display.clearDisplay();
        display.setCursor(10,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println("--Data Received--");
   
 
        display.setCursor(0,15);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Temperature:");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(temperature);
        display.display();
        display.setCursor(0,30);  //oled display
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Humidity:");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(humidity);
        display.display();
        display.setCursor(0,45);  //oled display
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Carbon Monoxide:");
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(h);
        display.display();
       
      }
      delay(1000);
      client.stop();
    }
  }
  else
  {

    Serial.print(".\n");
  }
  delay(15000);
}
