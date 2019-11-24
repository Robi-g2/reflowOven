#include "misc.h"
#include "tests.h"


// Cool down after running a reflow profile.
int cool_down(Adafruit_ILI9341* theDisplay) {
  int coolEnough = 0;
  int temperature = 0;
  int buttonPushed = 0;
  theDisplay->fillScreen(ILI9341_BLACK);
  switch_heater(HEATER_BOTH, HEATER_OFF);
  while (buttonPushed == 0)
  {
    // Check for button push.
    if (digitalRead(PIN_ROT_SW) == LOW) {
      delay(6);  // 6ms to wait for the switch to calm down.
      return 0;
    }
    temperature = get_temperature_reading();
    if (temperature <= SAFE_TEMPERATURE) {
      coolEnough = 1;
    } else {
      coolEnough = 0;
    }
    draw_screen_profile_finished(theDisplay, temperature, coolEnough);
  }
}

// Run a reflow profile.
int rflw_run_profile(Adafruit_ILI9341* theDisplay, ReflowProfile* profile, int* sdCardPresent) {
  while (digitalRead(PIN_ROT_SW) == LOW) { delay(6); }
  // Stage 1 - Pre-heat the board.
  buzz_profileRunning();
  theDisplay->fillScreen(ILI9341_BLACK);
  if( heater_PID_run(profile->preHeatInitialTemperature, PID_REACH_TEMP, profile->preHeatInitialMinimumTime, profile->preHeatInitialRate,
                     sdCardPresent, theDisplay, STAGE_PREHEAT) != 1) {
    // Stage 2 - maintain heat, slowly raising it.
    buzz_profileRunning();
    theDisplay->fillScreen(ILI9341_BLACK);
    if( heater_PID_run(profile->preHeatFinalTemperature, PID_REACH_TEMP, profile->preHeatFinalMinimumTime, profile->preHeatFinalRate,
                       sdCardPresent, theDisplay, STAGE_PREHEATMAINTAIN) != 1) {
        // Stage 2 - Ramp up dat' heat
        buzz_profileRunning();
        theDisplay->fillScreen(ILI9341_BLACK);
        if( heater_PID_run(profile->rampUpInitialTemperature, PID_REACH_TEMP, profile->rampUpInitialMinimumTime, profile->rampUpInitialRate,
                           sdCardPresent, theDisplay, STAGE_RAMPUP) != 1) {
          // Stage 3 - Reflow stage
          buzz_profileRunning();
          theDisplay->fillScreen(ILI9341_BLACK);
          heater_PID_run(profile->rampUpFinalTemperature, PID_REACH_TEMP, profile->rampUpFinalMinimumTime, profile->rampUpFinalRate,
                         sdCardPresent, theDisplay, STAGE_REFLOW);
        }
      }
    }
  // Stage 4 - Cool down
  while (digitalRead(PIN_ROT_SW) == LOW) { delay(6); }
  theDisplay->fillScreen(ILI9341_BLACK);
  cool_down(theDisplay);
  while (digitalRead(PIN_ROT_SW) == LOW) { delay(6); }
  // Stage 5 - Properly finished now.
  return 0;
}
