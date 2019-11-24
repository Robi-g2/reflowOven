#ifndef _THERMOCOUPLE_H
#define _THERMOCOUPLE_H

#include "Arduino.h"
#include "SPI.h"  // Generic SPI.
#include "misc.h"
#include "buzzer.h"
#include "heater.h"
#include "string.h"
#include "display.h"

int setup_temperature(void);
int get_temperature_reading(void);

#endif
