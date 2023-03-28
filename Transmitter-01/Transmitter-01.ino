#include <RH_NRF24.h>
#include "DHT.h"
#include <EEPROM.h>
 
 RH_NRF24 nrf24(2, 4); 
 
#define DHTPIN 5

 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
int deviceID = EEPROM.read(1);
int Rohit, temperature;
float h;

 
void setup()
{
  Serial.begin(115200);
   dht.begin();
  while (!Serial)
    ; 
  if (!nrf24.init()) 
  {
    Serial.println("init failed");
 
  }
  
  if (!nrf24.setChannel(3)) 
  {
    Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("setRF failed");
  }
  Serial.println("Transmitter started");
}
 
void loop() 
  {
  h = analogRead(A0);
  h= h/1023*100;
  Rohit = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(Rohit) || isnan(temperature||isnan(h)))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.println("Sending to gateway");
  uint8_t data[5];
  
  data[0] = Rohit;
  data[1] = temperature;
  data[2] = h;
  data[3] = deviceID;
  
  
  
  Serial.println("------------- Measurements -------------");
  Serial.print("Humidity: ");
  Serial.print(data[0]);
  
  Serial.print(", Temperature: ");
  Serial.print(data[1]);
  
  Serial.print(",Carbon Monoxide: ");
  Serial.print(data[2]);
   
  Serial.print(", ID: ");
  Serial.print(data[3]);
  Serial.println();
 
  
  Serial.println();
 
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
 
  
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  if (nrf24.waitAvailableTimeout(1000))
  {
   
    if (nrf24.recv(buf, &len))
    {
     
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
     
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply.");
    
  }
  delay(5000);
}
