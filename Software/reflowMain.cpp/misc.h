#ifndef _MISC_H
#define _MISC_H


// Heating
#define PIN_SSR_TOP 5
#define PIN_SSR_BOTTOM 6
#define THERMOCOUPLE_SAMPLE_WAIT 230
// Thermocouple
#define PIN_THERM_SS 4
#define TEMP_SAMPLES 2  // Number of temperature samples that constitute a single reading.
#define PID_SAMPLE_TIME_MS ((THERMOCOUPLE_SAMPLE_WAIT * TEMP_SAMPLES) + 210)  // Approx time between PID steps.
#define HEATER_PRECISION_TARGET 1  // Actual temperature can be +/-xC of target.
#define PID_FILTER_LENGTH 8  // Save the last X readings, this value must be a power of 2!!
#define LOG2_PID_FILTER_LENGTH 3 // Log2(PID_FILTER_LENGTH).
#define PID_P_WEIGHT 20  // Relative weights of PID components.
#define PID_I_WEIGHT 0
#define PID_D_WEIGHT 10
#define PID_ON_THRESHOLD 30  // If the sum of P, I, & D are above this, we try to turn the heater on.
#define MAX_HEATER_SWITCH_PERIOD_MS  2500  // Stop relay chatter
#define MAX_SAFE_TEMPERATURE 250  // 260 is when glue starts failing inside the oven.
#define SAFE_TEMPERATURE 40
#define MAX_TEST_MODE_TEMPERATURE 100
// Display
#define PIN_TFT_DC 8
#define PIN_TFT_RESET 9
#define PIN_TFT_SS 10
#define MENU_OPTION_HEIGHT 30  // 30 pixels high.
/* Pin definitions */
#define PIN_ROT_SW 1
#define PIN_ROT_A 2
#define PIN_ROT_B 3

typedef enum {HEATER_OFF=1, HEATER_ON} HEATER_STATE;
typedef enum {HEATER_TOP=1, HEATER_BOTTOM, HEATER_BOTH} HEATER_ELEMENT;
typedef enum {PID_REACH_TEMP=1, PID_MAINTAIN_TEMP} PID_MODE;

typedef enum {DIR_NONE = 0, DIR_CW, DIR_ACW} ENCODER_DIRECTIONS;
typedef struct {
  char* profileName;
  int preHeatInitialTemperature; // degC
  float preHeatInitialRate;      // degC/second
  int preHeatInitialMinimumTime; // seconds
  int preHeatFinalTemperature; // degC
  float preHeatFinalRate;      // degC/second
  int preHeatFinalMinimumTime; // seconds
  int rampUpInitialTemperature;  // degC
  float rampUpInitialRate;       // degC/second
  int rampUpInitialMinimumTime;  // seconds
  int rampUpFinalTemperature;  // degC
  float rampUpFinalRate;       // degC/second
  int rampUpFinalMinimumTime;  // seconds
} ReflowProfile;

typedef enum {STAGE_PREHEAT, STAGE_PREHEATMAINTAIN, STAGE_RAMPUP, STAGE_REFLOW} PROFILE_STAGE;
typedef enum {MENU_OPTION_FULL, MENU_OPTION_HALF, MENU_OPTION_SMALL} MENU_OPTION_SIZE;
typedef enum {MENU_OPTION_SELECTED, MENU_OPTION_NOT_SELECTED} MENU_OPTION_SELECT;
typedef enum {MENU_OPTION_BOTTOM = 0,
              MENU_OPTION_LOWERMID,
              MENU_OPTION_MID,
              MENU_OPTION_UPPERMID,
              MENU_OPTION_TOPISH,
              MENU_OPTION_TOP,
              MENU_OPTION_ABOVE_TOP,
              MENU_OPTION_TOP_TOP
             } MENU_OPTION_POSN;
typedef enum {ERROR_TOO_HOT, ERROR_NO_PROBE} ERRORS;
typedef enum {MAINMENU, CHOOSEPROFILE, CONFIRMPROFILE, LASTRUN, RUNPROFILE, ERRORHAPPENED} SCREENS;

#endif
