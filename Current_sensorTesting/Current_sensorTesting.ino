#include "EmonLib.h"
// Include Emon Library
EnergyMonitor emon1;
// Create an instance
void setup()
{
  Serial.begin(9600);

 emon1.voltage(2, 234.26, 1.7);
 emon1.current(1, 60);             // Current: input pin, calibration.
}

void loop()
{

emon1.calcVI(20,2000);         // Calculate all. No.of wavelengths, time-out
emon1.serialprint();  
  
double Irms = emon1.calcIrms(1480);  // Calculate Irms only
Serial.print(Irms*230.0);         // Apparent power
  Serial.print(" ");
  Serial.println(Irms);          // Irms
}
