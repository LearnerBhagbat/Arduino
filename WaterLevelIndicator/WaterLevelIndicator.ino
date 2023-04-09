#define BLYNK_TEMPLATE_ID "TMPLsdM8bNWn"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "_m34V1UzIhpH2OG_ogsFjgO6jyI-IPoj"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial

#define APP_DEBUG

#include "BlynkEdgent.h"

#define trig 14
#define echo 12

const char* ssid    = "SUVENDU_4G";
const char* password = "9556444048";

BlynkTimer timer;
long tankDepth=24;

void waterLevel(){
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Serial.println(cm);
  double level= tankDepth-cm;
  if(level>0){
    long percentage=((level/tankDepth))*100;
    Blynk.virtualWrite(V0,percentage);
  }
  else{
    Blynk.virtualWrite(V0,0);
  }

}

void setup()
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  timer.setInterval(10L, waterLevel);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
}
