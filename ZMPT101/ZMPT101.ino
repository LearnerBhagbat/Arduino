#define ZMPT101B_PIN 34

void setup()
{
  Serial.begin(115200);

// Analog attenuation: When using 0-1V sensors the attenuation should be decreased to improve accuracy.
//    ADC_0db: sets no attenuation. ADC can measure up to approximately 800 mV (1V input = ADC reading of 1088).
//    ADC_2_5db: The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 1100 mV. (1V input = ADC reading of 3722).
//    ADC_6db: The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 1350 mV. (1V input = ADC reading of 3033).
//    ADC_11db (default): The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 2600 mV. (1V input = ADC reading of 1575).

analogSetAttenuation(ADC_11db);

  pinMode(ZMPT101B_PIN, INPUT);
}

void loop()
{
  Serial.println(analogRead(ZMPT101B_PIN));
  delay(17);
}
