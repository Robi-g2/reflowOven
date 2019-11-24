#ifndef _TEST_H
#define _TEST_H

#include "Adafruit_GFX.h"  // Abstract graphics lib for adafruit displays.
#include "Adafruit_ILI9341.h"  // Display driver for displays using ILI9341.

#include "misc.h"
#include "buzzer.h"
#include "display.h"
#include "heater.h"
#include "thermocouple.h"


// Cool down after running a reflow profile.
int cool_down(Adafruit_ILI9341* theDisplay);

// Run a reflow profile.
int rflw_run_profile(Adafruit_ILI9341* theDisplay, ReflowProfile* profile, int* sdCardPresent);

#endif
