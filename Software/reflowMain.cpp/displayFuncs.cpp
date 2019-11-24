/*
   Author: Rob Gower
   Created: 17/Oct/2019
   Purpose: Any function that uses the Display.
*/

#include "display.h"

// Set up the hardware and stuff for the display.
int setup_display(Adafruit_ILI9341* tft) {
  pinMode(PIN_TFT_DC, OUTPUT);  // TFT D/C
  pinMode(PIN_TFT_RESET, OUTPUT);  // TFT RESET
  pinMode(PIN_TFT_SS, OUTPUT);  // TFT CS
  digitalWrite(PIN_TFT_SS, HIGH);  // We don't want to talk to this.
  // Display
  tft->begin(24000000);
  tft->fillScreen(ILI9341_BLACK);
  tft->setRotation(0);  // Set screen mode to portrait.
}

// Draw title text at the top of a screen.
int draw_menu_title(Adafruit_ILI9341* tft, const char* text) {
  tft->setTextSize(3);
  tft->setCursor(5, 5);
  tft->fillRect(0, 0, tft->width(), TITLE_BAR_HEIGHT, ILI9341_RED);
  tft->setTextColor(ILI9341_WHITE);
  tft->println(text);
  tft->drawFastHLine(0, TITLE_BAR_HEIGHT, tft->width(), ILI9341_WHITE);
  return 0;
}

drawScrollBar(int location, int numMenuItems, Adafruit_ILI9341* tft) {
  tft->fillRect((tft->width() - SCROLLBAR_WIDTH_PX),
                TITLE_BAR_HEIGHT,
                SCROLLBAR_WIDTH_PX,
                (tft->height() - TITLE_BAR_HEIGHT),
                ILI9341_BLACK);
  tft->fillRect((tft->width() - (SCROLLBAR_WIDTH_PX - 2)),
                TITLE_BAR_HEIGHT,
                (SCROLLBAR_WIDTH_PX - 4),
                (tft->height() - TITLE_BAR_HEIGHT),
                ILI9341_DARKGREY);
  tft->fillRect((tft->width() - (SCROLLBAR_WIDTH_PX - 1)),
                TITLE_BAR_HEIGHT + (((tft->height() - TITLE_BAR_HEIGHT) / (numMenuItems + 1)) * location),
                (SCROLLBAR_WIDTH_PX - 2),
                ((tft->height() - TITLE_BAR_HEIGHT) / (numMenuItems + 1)),
                ILI9341_RED);
  return 0;
}

// Draw a thing to click on
int draw_option_box(Adafruit_ILI9341* tft, const char* text, MENU_OPTION_SIZE optSize, MENU_OPTION_SELECT optSel, MENU_OPTION_POSN optPosn) {
  int w = tft->width();
  int h = tft->height();
  int boxWidth = 0;
  int startX = 5;
  int startY = 0;
  switch (optPosn) {
    case (MENU_OPTION_BOTTOM):
      startY = h - (startX + MENU_OPTION_HEIGHT);
      break;
    case (MENU_OPTION_LOWERMID):
      startY = h - ((startX + MENU_OPTION_HEIGHT) << 1);
      break;
    case (MENU_OPTION_MID):
      startY = h - ((startX + MENU_OPTION_HEIGHT) * 3);
      break;
    case (MENU_OPTION_UPPERMID):
      startY = h - ((startX + MENU_OPTION_HEIGHT) << 2);
      break;
    case (MENU_OPTION_TOPISH):
      startY = h - ((startX + MENU_OPTION_HEIGHT) * 5);
      break;
    case (MENU_OPTION_TOP):
      startY = h - ((startX + MENU_OPTION_HEIGHT) * 6);
      break;
    case (MENU_OPTION_ABOVE_TOP):
      startY = h - ((startX + MENU_OPTION_HEIGHT) * 7);
      break;
    case (MENU_OPTION_TOP_TOP):
      startY = h - ((startX + MENU_OPTION_HEIGHT) << 3);
      break;
    default:
      startY = h - (startX + MENU_OPTION_HEIGHT);
      break;
  }
  switch (optSize) {
    case (MENU_OPTION_FULL):
      boxWidth = w - (startX << 1);
      break;
    case (MENU_OPTION_HALF):
      boxWidth = (w >> 1) - (startX << 1);
      break;
    case (MENU_OPTION_SMALL):
      boxWidth = (w / 3) - (startX << 1);
      break;
    default:
      boxWidth = w - (startX << 1);
      break;
  }
  // Draw a rectangle and some text over the rectangle
  tft->setTextSize(2);
  tft->setCursor(startX + 5, startY + (MENU_OPTION_HEIGHT / 3));
  if (optSel == MENU_OPTION_SELECTED) {
    tft->drawRect(startX, startY, boxWidth, MENU_OPTION_HEIGHT, ILI9341_WHITE);
    tft->fillRect(startX + 1, startY + 1, boxWidth - 1, MENU_OPTION_HEIGHT - 1, ILI9341_MAGENTA);
    tft->setTextColor(ILI9341_WHITE);
    tft->println(text);
  } else if (optSel == MENU_OPTION_NOT_SELECTED) {
    tft->fillRect(startX, startY, boxWidth, MENU_OPTION_HEIGHT, ILI9341_WHITE);
    tft->setTextColor(ILI9341_BLACK);
    tft->println(text);
  }
  return 0;
}

// Draw information about the selected profile under the title, and above 2 option boxes.
int draw_profile_info_text(Adafruit_ILI9341* tft, ReflowProfile* selectedProfile) {
  int vertPosn = 220;
  tft->setTextSize(2);
  tft->setCursor(5, 45);
  tft->setTextColor(ILI9341_WHITE);
  tft->print("Name: ");
  tft->println(selectedProfile->profileName);
  // Number information.
  tft->setCursor(5, vertPosn);
  tft->print("Rflw 2 Time:  ");
  tft->print(selectedProfile->rampUpFinalMinimumTime);
  tft->println(" s");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("Rflw 2 Temp:  ");
  tft->print(selectedProfile->rampUpFinalTemperature);
  tft->println(" C");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("Rflw 1 Time:  ");
  tft->print(selectedProfile->rampUpInitialMinimumTime);
  tft->println(" s");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("Rflw 1 Temp:  ");
  tft->print(selectedProfile->rampUpInitialTemperature);
  tft->println(" C");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("PreH 2 Time:  ");
  tft->print(selectedProfile->preHeatFinalMinimumTime);
  tft->println(" s");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("PreH 2 Temp:  ");
  tft->print(selectedProfile->preHeatFinalTemperature);
  tft->println(" C");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("PreH 1 Time:  ");
  tft->print(selectedProfile->preHeatInitialMinimumTime);
  tft->println(" s");
  vertPosn -= 20;
  tft->setCursor(5, vertPosn);
  tft->print("PreH 1 Temp:  ");
  tft->print(selectedProfile->preHeatInitialTemperature);
  tft->println(" C");
  return 0;
}

// Draw the current temperature on the 'profile finished' screen.
int draw_temperature_graph(Adafruit_ILI9341* tft, int currentTemp) {
  //TODO//
  return 0;
}

// Draw reflow stats in the bottom left quarter of the screen.
int draw_reflow_stats(Adafruit_ILI9341* tft, PROFILE_STAGE stage, int currentTemp,
                      int targetTemp, float rateOfChange, HEATER_STATE heaterState) {
  tft->fillRect(10, 70, 200, 75, ILI9341_BLACK);
  tft->fillRect(12, 188, 170, 27, ILI9341_BLACK);
  tft->fillRect(88, 230, 140, 38, ILI9341_BLACK);
  tft->setTextSize(4);
  int leftOffset = 5;
  tft->setCursor(10, 70);
  tft->setTextColor(ILI9341_WHITE);
  tft->print(currentTemp);
  tft->println("C");
  tft->setCursor(115, 110);
  tft->print(targetTemp);
  tft->println("C");
  tft->drawLine(85, 125, 135, 75, ILI9341_WHITE);
  tft->setCursor(leftOffset, 165);
  tft->setTextSize(3);
  tft->print("Change Rate:  ");
  tft->print(rateOfChange);
  tft->println(" C/s");
  tft->setTextSize(2);
  tft->setCursor(leftOffset, 230);
  tft->print("Stage: ");
  switch (stage) {
    case (STAGE_PREHEAT):
      tft->println("Pre-heat");
      break;
    case (STAGE_PREHEATMAINTAIN):
      tft->println("Pre-heat 2");
      break;
    case (STAGE_RAMPUP):
      tft->println("Ramp up");
      break;
    case (STAGE_REFLOW):
      tft->println("Reflow");
      break;
    default:
      tft->println("ERR");
      break;
  }
  tft->setCursor(leftOffset, 250);
  tft->print("HEATER: ");
  if (heaterState == HEATER_OFF) {
    tft->setTextColor(ILI9341_WHITE);
    tft->println("OFF");
  } else {
    tft->setTextColor(ILI9341_RED);
    tft->println("ON");
  }
  return 0;
}

// Draw the main menu screen
int draw_screen_main_menu(Adafruit_ILI9341* tft, int highlightedItem, int temperature) {
  draw_menu_title(tft, "Main Menu");
  if (highlightedItem == 0) {
    draw_option_box(tft, "Select Profile", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Run Default", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_BOTTOM);
  } else if (highlightedItem == 1) {
    draw_option_box(tft, "Select Profile", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Run Default", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  }
  tft->setTextSize(7);
  tft->fillRect(0, 80, 300, 100, ILI9341_BLACK);
  tft->setCursor(15, 100);
  tft->setTextColor(ILI9341_WHITE);
  tft->print(temperature);
  tft->print(" C");
  return 0;
}

int draw_screen_choose_profile(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile** allProfiles, int numProfiles) {
  int i = 0;
  if (highlightedItem < 0) {
    highlightedItem *= -1;
  }
  // Draw each option.
  for (i = 0 ; i < (MAX_SMALL_OPTIONS + 1) ; i++) {
    if (i < numProfiles) {
      if (i == 0) {
        draw_option_box(tft, allProfiles[(highlightedItem + i) % numProfiles]->profileName, MENU_OPTION_FULL, MENU_OPTION_SELECTED, i);
      } else {
        draw_option_box(tft, allProfiles[(highlightedItem + i) % numProfiles]->profileName, MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, i);
      }
    }
  }
  // Draw the scrollbar.
  drawScrollBar(highlightedItem, numProfiles - 1, tft);
  // Draw menu title over the top.
  draw_menu_title(tft, "Pick Profile");
}

// Draw the 'last run' screen
int draw_screen_last_run(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile* profile) {
  draw_menu_title(tft, "Run Default?");
  draw_profile_info_text(tft, profile);
  if (highlightedItem == 0) {
    draw_option_box(tft, "Cancel", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Confirm and Run", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_BOTTOM);
  } else {
    draw_option_box(tft, "Cancel", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Confirm and Run", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  }
  return 0;
}

// Draw the 'chosen profile' screen
int draw_screen_chosen_profile(Adafruit_ILI9341* tft, int highlightedItem, ReflowProfile* profile) {
  draw_menu_title(tft, "Confirm Pick");
  draw_profile_info_text(tft, profile);
  if (highlightedItem == 0) {
    draw_option_box(tft, "Cancel", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Confirm and Run", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_BOTTOM);
  } else {
    draw_option_box(tft, "Cancel", MENU_OPTION_FULL, MENU_OPTION_NOT_SELECTED, MENU_OPTION_LOWERMID);
    draw_option_box(tft, "Confirm and Run", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  }
  return 0;
}

// Draw the 'running profile' screen
int draw_screen_running_profile(Adafruit_ILI9341* tft, PROFILE_STAGE stage, int currentTemp, int targetTemp, float rateOfChange, HEATER_STATE heaterState) {
  draw_menu_title(tft, "Running...");
  draw_temperature_graph(tft, currentTemp);
  draw_reflow_stats(tft, stage, currentTemp, targetTemp, rateOfChange, heaterState);
  draw_option_box(tft, "Stop", MENU_OPTION_SMALL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  return 0;
}

// Draw the 'profile finished' screen
int draw_screen_profile_finished(Adafruit_ILI9341* tft, int temperature, int coolEnough) {
  // Draw the following text: "Open over door and wait until oven temperature is 40C or lower before removing board.".
  // Draw the current temperature.
  tft->setTextSize(7);
  tft->fillRect(0, 80, 300, 100, ILI9341_BLACK);
  tft->setCursor(15 , 100);
  tft->setTextColor(ILI9341_WHITE);
  tft->print(temperature);
  tft->print(" C");
  // See if it's cool enough.
  if (coolEnough == 1) {
    draw_menu_title(tft, "Cooled");
    draw_option_box(tft, "Back to Main Menu", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  } else {
    draw_menu_title(tft, "Cooling...");
    draw_option_box(tft, "Finish Early", MENU_OPTION_FULL, MENU_OPTION_SELECTED, MENU_OPTION_BOTTOM);
  }
  return 0;
}

int draw_screen_error(Adafruit_ILI9341* tft, ERRORS errorCode) {
  draw_menu_title(tft, "CRITICAL ERROR");
  // Draw the type of error
  switch (errorCode) {
    case (ERROR_TOO_HOT):
      // draw text that says 'too hot'.
      // draw current temperature.
      break;
    default:
      break;
  }
  return 0;
}
