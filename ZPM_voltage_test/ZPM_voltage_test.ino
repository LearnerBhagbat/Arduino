/*
  ZMPT101B - AC Voltage sensor
  Show AC wava in serial plotter

  modified on 7 Sep 2020
  by Mohammad Reza Akbari @ Electropeak
  Home
*/


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A0));

  delay(100);
}
