#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"
#include "WiFi.h"
#include <driver/adc.h>

// The GPIO pin were the CT sensor is connected to (should be an ADC input)
#define ADC_INPUT 34

// The voltage in our apartment. Usually this is 230V for Europe, 110V for US.
// Ours is higher because the building has its own high voltage cabin.
#define HOME_VOLTAGE 247.0

// Force EmonLib to use 10bit ADC resolution
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)

// Create instances
//AWSConnector awsConnector;
EnergyMonitor emon1;

// Wifi credentials
const char *WIFI_SSID = "SUVENDU_4G";
const char *WIFI_PASSWORD = "9556444048";

// Create instance for LCD display on address 0x27
// (16 characters, 2 lines)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Array to store 30 readings (and then transmit in one-go to AWS)
short measurements[30];
short measureIndex = 0;
unsigned long lastMeasurement = 0;
unsigned long timeFinishedSetup = 0;

void writeEnergyToDisplay(double watts, double amps){
  lcd.setCursor(3, 1); // Start from column 3, first two are broken :/

  lcd.print((int)watts);
  lcd.print("W ");
  lcd.print(amps);
  lcd.print("A    ");
}

void printIPAddress(){
  lcd.setCursor(3,0);
  lcd.print(WiFi.localIP());
}


void connectToWiFi() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WiFi...      ");

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("esp32-energy-monitor");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Only try 15 times to connect to the WiFi
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15){
    delay(500);
    Serial.print(".");
    retries++;
  }

  // If we still couldn't connect to the WiFi, go to deep sleep for a
  // minute and try again.
  if(WiFi.status() != WL_CONNECTED){
      esp_sleep_enable_timer_wakeup(1 * 60L * 1000000L);
    esp_deep_sleep_start();
  }

  // If we get here, print the IP address on the LCD
  printIPAddress();
}

void setup()
{
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  analogReadResolution(10);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  connectToWiFi();

  // Initialize emon library (30 = calibration number)
  emon1.current(ADC_INPUT, 30);

  lcd.setCursor(3, 0);
  lcd.print("AWS connect   ");
//  awsConnector.setup();

  timeFinishedSetup = millis();
}

void loop(){
  unsigned long currentMillis = millis();

  // If it's been longer then 1000ms since we took a measurement, take one now!
  if(currentMillis - lastMeasurement > 1000){
    double amps = emon1.calcIrms(1480); // Calculate Irms only
    double watt = amps * HOME_VOLTAGE;

    // Update the display
    writeEnergyToDisplay(watt, amps);

    lastMeasurement = millis();

    // Readings are unstable the first 5 seconds when the device powers on
    // so ignore them until they stabilise.
    if(millis() - timeFinishedSetup < 10000){
      lcd.setCursor(3, 0);
      lcd.print("Startup mode   ");
    }else{
      printIPAddress();
      measurements[measureIndex] = watt;
      measureIndex++;
    }
  }

//  // When we have 30 measurements, send them to AWS!
//  if (measureIndex == 30) {
//    lcd.setCursor(3,0);
//    lcd.print("AWS upload..   ");
//
//    // Construct the JSON to send to AWS
//    String msg = "{\"readings\": [";
//
//    for (short i = 0; i <= 28; i++){
//      msg += measurements[i];
//      msg += ",";
//    }
//
//    msg += measurements[29];
//    msg += "]}";
//
//    awsConnector.sendMessage(msg);
//    measureIndex = 0;
//  }
//
//  // This keeps the MQTT connection stable
//  awsConnector.loop();
}
