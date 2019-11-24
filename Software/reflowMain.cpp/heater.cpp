/*
   Author: Rob Gower
   Created: 17/Oct/2019
   Purpose: Any function that uses the SD Card.
*/

#include "heater.h"

// Solid State Relay Outputs
int setup_heater(void) {
  pinMode(PIN_SSR_TOP, OUTPUT);  // Top elements
  pinMode(PIN_SSR_BOTTOM, OUTPUT);  // Bottom elements
  switch_heater(HEATER_BOTH, HEATER_OFF);
  return 0;
}

// Turn the heaters on or off
int switch_heater(HEATER_ELEMENT heater, HEATER_STATE onoff)
{
  interrupts();
  if (onoff == HEATER_ON) {
    digitalWrite(PIN_SSR_TOP, LOW);
    digitalWrite(PIN_SSR_BOTTOM, LOW);
  } else {
    digitalWrite(PIN_SSR_TOP, HIGH);
    digitalWrite(PIN_SSR_BOTTOM, HIGH);
  }
  return 0;
}

// One loop around the heater PID
int heater_PID_step(int targetTemperature, int* filterTracker,
                    float targetRamp,
                    int* temperatureArray, int* sumTemperatures,
                    float* tempDifferential, int* tempIntegral,
                    int* averageTemperature, int* onCounter,
                    int* sdCardPresent, Adafruit_ILI9341* tft,
                    PROFILE_STAGE stage, int* avgInitialCount) {
  int P = 0;
  int I = 0;
  int D = 0;
  float tempDiff = 0;
  float absTempDiff = 0;
  int pidValue = 0;
  int z = 0;
  int j = 0;
  // Get a reading, and calculate some of the statistics.
  if (*avgInitialCount >= PID_FILTER_LENGTH) {
    *sumTemperatures -= temperatureArray[*filterTracker];
  } else {
    *avgInitialCount += 1;
  }
  temperatureArray[*filterTracker] = get_temperature_reading();
  *sumTemperatures += temperatureArray[*filterTracker];
  *averageTemperature = *sumTemperatures / PID_FILTER_LENGTH;  // Divide by filter length.
  // Work out the PID parts of the PID.
  // P
  P = targetTemperature - temperatureArray[*filterTracker];
  // I
  *tempIntegral = temperatureArray[*filterTracker] + temperatureArray[(*filterTracker + PID_FILTER_LENGTH - 1) % PID_FILTER_LENGTH];
  I = (2 * targetTemperature) - *tempIntegral;
  // D
  *tempDifferential = 0.0;
  for(j = 0; j < PID_FILTER_LENGTH-1 ; j++){
    *tempDifferential += temperatureArray[(*filterTracker + PID_FILTER_LENGTH - j) % PID_FILTER_LENGTH] 
						 - temperatureArray[(*filterTracker + PID_FILTER_LENGTH - (j+1)) % PID_FILTER_LENGTH];
  }
  *tempDifferential /= (PID_FILTER_LENGTH - 1);
  *tempDifferential *= (1000.00 / PID_SAMPLE_TIME_MS);  //1.55;  // Get the degC/sample to a degC/second value
  D = *tempDifferential;
  // Work out the weightings of each term.
  P *= PID_P_WEIGHT;
  I *= PID_I_WEIGHT;
  D *= PID_D_WEIGHT;
  // See if we turn the heater on or off or not.
  //pidValue = P + I + D;
  pidValue = P;
  if (*tempDifferential > (targetRamp + 0.1)) {
    switch_heater(HEATER_BOTH, HEATER_OFF);
    *onCounter = 0;
    draw_screen_running_profile(tft, stage, temperatureArray[*filterTracker], targetTemperature, *tempDifferential, HEATER_OFF);
  } else if (pidValue > PID_ON_THRESHOLD && *onCounter > MAX_HEATER_SWITCH_PERIOD_MS) {
    switch_heater(HEATER_BOTH, HEATER_ON);
    *onCounter = 0;
    draw_screen_running_profile(tft, stage, temperatureArray[*filterTracker], targetTemperature, *tempDifferential, HEATER_ON);
    //Serial.println("Turning on");
  } else if (pidValue > PID_ON_THRESHOLD) {
    draw_screen_running_profile(tft, stage, temperatureArray[*filterTracker], targetTemperature, *tempDifferential, HEATER_ON);
    // Keeps the heater on if it's already on.
    *onCounter += PID_SAMPLE_TIME_MS;
    //Serial.println("Staying on");
  } else {
    //Serial.println("OFF");
    draw_screen_running_profile(tft, stage, temperatureArray[*filterTracker], targetTemperature, *tempDifferential, HEATER_OFF);
    switch_heater(HEATER_BOTH, HEATER_OFF);
    *onCounter += PID_SAMPLE_TIME_MS;
  }
  // Wait and do some admin.
  *filterTracker += 1;
  *filterTracker %= PID_FILTER_LENGTH;
  return 0;
}

// Control switching of the SSRs to reach a target temperature.
int heater_PID_run(int targetTemperature, PID_MODE mode, int time_s, float targetRate_Cs,
                   int* sdCardPresent, Adafruit_ILI9341* tft, PROFILE_STAGE stage) {
  // Local variables
  int buttonPushed = 0;
  int filterTracker = 0;
  int* pfilterTracker = &filterTracker;
  int temperatureReadings[PID_FILTER_LENGTH] = {0};
  int* ptemperatureReadings = temperatureReadings;
  float tempDifferential;
  float* ptempDifferential = &tempDifferential;
  int tempIntegral;
  int* ptempIntegral = &tempIntegral;
  int averageTemperature = 0;
  int* paverageTemperature = &averageTemperature;
  int sumTemperatures = 0;  // Sum of last n temperature readings.
  int* psumTemperatures = &sumTemperatures;
  int onCounter = MAX_HEATER_SWITCH_PERIOD_MS;  // Stop relay chatter.
  int* ponCounter = &onCounter;
  int avgInitCount = 0;
  int* pAvgInitCount = &avgInitCount;
  int timeSpent = 0;
  int i = 0;
  int tempTemp = get_temperature_reading();
  for(i=0 ; i<PID_FILTER_LENGTH ; i++) {
    temperatureReadings[i] = tempTemp;
  }
  // If we just want to get to a temperature from another.
  if (mode == PID_REACH_TEMP) {
    for (i = 0 ; i < ( (time_s * 1000.00) / PID_SAMPLE_TIME_MS) ; i++) {
      if (digitalRead(PIN_ROT_SW) == LOW) {
        delay(6);  // 6ms to wait for the switch to calm down.
        return 1;
      }
      //Serial.println("PIDstep");
      heater_PID_step(targetTemperature, pfilterTracker,
                      targetRate_Cs,
                      ptemperatureReadings, psumTemperatures,
                      ptempDifferential, ptempIntegral,
                      paverageTemperature, ponCounter,
                      sdCardPresent, tft, stage, pAvgInitCount);
    }
    // We have now heated for the minimum time, anything after this is
    // down to the oven being too slow.
    while (buttonPushed == 0) {
      if (digitalRead(PIN_ROT_SW) == LOW) {
        delay(6);  // 6ms to wait for the switch to calm down.
        return 1;
      }
      heater_PID_step(targetTemperature, pfilterTracker,
                      targetRate_Cs,
                      ptemperatureReadings, psumTemperatures,
                      ptempDifferential, ptempIntegral,
                      paverageTemperature, ponCounter,
                      sdCardPresent, tft, stage, pAvgInitCount);
      if ( averageTemperature >= (targetTemperature - HEATER_PRECISION_TARGET) ) {
        return 0;  // Success.
      }
    }
  }
  // If we are already at a temperature, and want to maintain this
  // for a while.
  else if (mode == PID_MAINTAIN_TEMP) {
    for (i = 0 ; i < ( (time_s * 1000) / PID_SAMPLE_TIME_MS) ; i++) {
      if (digitalRead(PIN_ROT_SW) == LOW) {
        delay(6);  // 6ms to wait for the switch to calm down.
        return 1;
      }
      //buzz_profileRunning();
      heater_PID_step(targetTemperature, pfilterTracker,
                      targetRate_Cs,
                      ptemperatureReadings, psumTemperatures,
                      ptempDifferential, ptempIntegral,
                      paverageTemperature, ponCounter,
                      sdCardPresent, tft, stage, pAvgInitCount);
    }
  }
  else {
    return -1;  // Error, no valid mode passed.
  }
  return 0;
}
