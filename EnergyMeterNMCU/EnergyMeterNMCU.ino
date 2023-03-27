#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLsdM8bNWn"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "yRPYTDNlS4sk00ElrTYl9RULVlCEl3LB"
 
#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266_h>


#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <ESP8266WiFi.h>
 
EnergyMonitor emon;
#define vCalibration 106.8
#define currCalibration 0.52
BlynkTimer timer;
 
char auth[] = "yRPYTDNlS4sk00ElrTYl9RULVlCEl3LB";
 
char ssid[] = "Hotspot";
char pass[] = "Forget@0987";
 
float kWh = 0;
unsigned long lastmillis = millis();
 
void myTimerEvent() {
    emon.calcVI(20, 2000);
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    Blynk.virtualWrite(V0, emon.Vrms);
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    Blynk.virtualWrite(V1, emon.Irms);
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.print("W");
    Blynk.virtualWrite(V2, emon.apparentPower);
    Serial.print("\tkWh: ");
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
    Serial.print(kWh, 4);
    Serial.println("kWh");
    lastmillis = millis();
    Blynk.virtualWrite(V3, kWh);
}
 
void setup() {
  Serial.begin(9600);
  emon.voltage(5, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(4, currCalibration); // Current: input pin, calibration.
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, myTimerEvent);
}
 
void loop() {
  Blynk.run();
  timer.run();
}
