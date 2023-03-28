#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLsdM8bNWn"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "yRPYTDNlS4sk00ElrTYl9RULVlCEl3LB"
 
#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
EnergyMonitor emon;
#define vCalibration 106.8
#define currCalibration 0.52
BlynkTimer timer;
 
char auth[] = "yRPYTDNlS4sk00ElrTYl9RULVlCEl3LB";
 
char ssid[] = "JioFiber-22";
char pass[] = "87654321";
 
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
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, myTimerEvent);
  // Clear the buffer.
  display.clearDisplay();
  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,2);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();
}
 
void loop() {
  Blynk.run();
  timer.run();
}
