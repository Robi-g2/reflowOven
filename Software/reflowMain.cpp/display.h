#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "Arduino.h"
#include "misc.h"
#include "SPI.h"  // Generic SPI.
#include "Adafruit_GFX.h"  // Abstract graphics lib for adafruit displays.
#include "Adafruit_ILI9341.h"  // Display driver for displays using ILI9341.

#define MAX_SMALL_OPTIONS 7
#define SCROLLBAR_WIDTH_PX 10
#define TITLE_BAR_HEIGHT 31

int setup_display(Adafruit_ILI9341* tft);
int draw_menu_title(Adafruit_ILI9341* tft, const char* text);
int drawScrollBar(int location, int numMenuItems, Adafruit_ILI9341* tft);
int draw_option_box(Adafruit_ILI9341* tft, const char* text, MENU_OPTION_SIZE optSize, MENU_OPTION_SELECT optSel, MENU_OPTION_POSN optPosn);
int draw_profile_info_text(Adafruit_ILI9341* tft, ReflowProfile* profile);
int draw_temperature_graph(Adafruit_ILI9341* tft, int currentTemp);
int draw_reflow_stats(Adafruit_ILI9341* tft, PROFILE_STAGE stage, int currentTemp, int targetTemp, float rateOfChange, HEATER_STATE heaterState);
int draw_screen_main_menu(Adafruit_ILI9341* tft, int highlightedItem, int temperature);
int draw_screen_test_mode(Adafruit_ILI9341* tft, int temperature, int heaterState);
int draw_screen_choose_profile(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile** allProfiles, int numProfiles);
int draw_screen_last_run(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile* profile);
int draw_screen_chosen_profile(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile* profile);
int draw_screen_running_profile(Adafruit_ILI9341* tft, PROFILE_STAGE stage, int currentTemp, int targetTemp, float rateOfChange, HEATER_STATE heaterState);
int draw_screen_profile_finished(Adafruit_ILI9341* tft, int temperature, int coolEnough);
int draw_screen_error(Adafruit_ILI9341* tft, ERRORS errorCode);


#endif
