/*
   Author: Rob Gower
   Created: 17/Oct/2019
   Purpose: Any function that uses the SD Card.
*/

#include "thermocouple.h"

int setup_temperature(void)
{
  // SPI for thermometer.
  pinMode(PIN_THERM_SS, OUTPUT);  // Thermocouple CS
  digitalWrite(PIN_THERM_SS, HIGH);  // We don't want to talk to this.
}

// Get a temperature reading
/*int get_temperature_reading(void) {
  int i = 0;
  delay(1000);
  return 1;
}*/
int get_temperature_reading(void) {
  interrupts();
  int temperature = 0;
  int tempVout = 0;
  int i = 0;
  SPI.begin();
  //delay(230);
  // 100kHz (chip maximum = 4.3MHz).
  SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
  for (i = 0 ; i < TEMP_SAMPLES ; i++) {
    delay(THERMOCOUPLE_SAMPLE_WAIT);  // Max conversion time = 0.22s.
    digitalWrite(PIN_THERM_SS, LOW);
    // Communicate with temperature chip.
    // All 0's = 0C, all 1's = 1023.75C.
    tempVout = SPI.transfer(0x00);
    tempVout <<= 8;
    tempVout |= SPI.transfer(0x00);
    digitalWrite(PIN_THERM_SS, HIGH);
    // Check for open probe.
    if (tempVout & 0x04) {
      return 999;  // Error.
    }
    // Get the bits relating to temperature.
    temperature += ((tempVout & 32760) >> 3);  // Bits [14:3].
  }
  SPI.endTransaction();
  temperature /= TEMP_SAMPLES;  // Divide total by numSamples to get average.
  temperature >>= 2;  // We need to divide by 4, else we'll be reading in 0.25C steps.
  if (temperature > MAX_SAFE_TEMPERATURE) {
    buzz_error();
    switch_heater(HEATER_BOTH, HEATER_OFF);
  }
  return temperature;
}
