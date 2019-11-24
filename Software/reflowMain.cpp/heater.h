#ifndef _HEATER_H
#define _HEATER_H

#include "Arduino.h"
#include "thermocouple.h"
#include "misc.h"
#include "SD.h"
#include "Adafruit_GFX.h"  // Abstract graphics lib for adafruit displays.
#include "Adafruit_ILI9341.h"  // Display driver for displays using ILI9341.

int setup_heater(void);
int switch_heater(HEATER_ELEMENT heater, HEATER_STATE onoff);
int heater_PID_step(int targetTemperature, int* filterTracker,
                    float targetRate,
                    int* temperatureArray, int* sumTemperatures,
                    float* tempDifferential, int* tempIntegral,
                    int* averageTemperature, int* onCounter,
                    int* sdCardPresent, Adafruit_ILI9341* tft,
                    PROFILE_STAGE stage, int* avgInitialCount);
int heater_PID_run(int targetTemperature, PID_MODE mode, int time_s, float targetRate_Cs,
                   int* sdCardPresent, Adafruit_ILI9341* tft, PROFILE_STAGE stage);

#endif
