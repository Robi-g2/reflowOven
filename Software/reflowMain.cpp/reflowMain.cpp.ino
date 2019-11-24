#include "SPI.h"  // Generic SPI.
#include "string.h"
#include "Adafruit_GFX.h"  // Abstract graphics lib for adafruit displays.
#include "Adafruit_ILI9341.h"  // Display driver for displays using ILI9341.

#include "misc.h"
#include "thermocouple.h"
#include "profiles.h"
#include "buzzer.h"
#include "display.h"
#include "heater.h"
#include "tests.h"

/* Typedefs, structs, enums, and global variables*/
#define PIN_SDCARD_SS 7
Adafruit_ILI9341 tft = Adafruit_ILI9341(PIN_TFT_SS, PIN_TFT_DC, PIN_TFT_RESET);
Adafruit_ILI9341* ptft = &tft;
volatile ENCODER_DIRECTIONS encoderMovement = DIR_NONE;
volatile int debounceCount = 0;

/* Functions */
//
int main_menu(Adafruit_ILI9341* ptft)
{
  int menuItem = 0;
  int buttonPushed = 0;
  int temperature = 0;
  int tempToggle = 0;
  ptft->fillScreen(ILI9341_BLACK);
  interrupts();  // Enable interrupts.
  while (buttonPushed == 0) {
    temperature = get_temperature_reading();
    draw_screen_main_menu(ptft, menuItem, temperature);
    // Check for rotary encoder movement.
    if (encoderMovement != DIR_NONE) {
      if (encoderMovement == DIR_CW) {
        encoderMovement = DIR_NONE;
        menuItem += 1;
        menuItem %= 2;
      } else {
        encoderMovement = DIR_NONE;
        menuItem += 1;  // Don't even go near -ve numbers.
        menuItem %= 2;
      }
    }
    // Check for button push.
    if (digitalRead(PIN_ROT_SW) == LOW) {
      delay(6);  // 6ms to wait for the switch to calm down.
      buttonPushed = 1;
    }
  }
  noInterrupts();
  buttonPushed = 0;
  while (digitalRead(PIN_ROT_SW) == LOW) {
    delay(10);
  }
  if (menuItem == 0) {
    return CHOOSEPROFILE;
  } else if (menuItem == 1) {
    return LASTRUN;
  } else {
    return MAINMENU;
  }
}

//
int choose_profile(Adafruit_ILI9341* ptft, int* selectedProfileNum)
{
  int menuItem = 0;
  int buttonPushed = 0;
  *selectedProfileNum = 0;
  ptft->fillScreen(ILI9341_BLACK);
  draw_screen_choose_profile(ptft, *selectedProfileNum, allProfiles, NUM_PROFILES);
  interrupts();
  while (buttonPushed == 0) {
    if (encoderMovement != DIR_NONE) {
      if (encoderMovement == DIR_CW) {
        encoderMovement = DIR_NONE;
        *selectedProfileNum += 1;
        *selectedProfileNum %= NUM_PROFILES;
      } else {
        encoderMovement = DIR_NONE;
        *selectedProfileNum += (NUM_PROFILES - 1); // Don't even go near -ve numbers.
        *selectedProfileNum %= NUM_PROFILES;
      }
      draw_screen_choose_profile(ptft, *selectedProfileNum, allProfiles, NUM_PROFILES);
    }
    if (digitalRead(PIN_ROT_SW) == LOW) {
      delay(6);  // 6ms to wait for the switch to calm down.
      buttonPushed = 1;
    }
  }
  noInterrupts();
  buttonPushed = 0;
  while (digitalRead(PIN_ROT_SW) == LOW) {
    delay(10);
  }
  return CONFIRMPROFILE;
}

//
int confirm_profile(Adafruit_ILI9341* ptft, int* selectedProfileNum)
{
  int menuItem = 0;
  int buttonPushed = 0;
  ptft->fillScreen(ILI9341_BLACK);
  draw_screen_chosen_profile(ptft, menuItem, allProfiles[*selectedProfileNum]);
  interrupts();
  while (buttonPushed == 0) {
    if (encoderMovement != DIR_NONE) {
      encoderMovement = DIR_NONE;
      menuItem = (menuItem == 0) ? 1 : 0;
      draw_screen_chosen_profile(ptft, menuItem, allProfiles[*selectedProfileNum]);
    }
    if (digitalRead(PIN_ROT_SW) == LOW) {
      delay(6);  // 6ms to wait for the switch to calm down.
      buttonPushed = 1;
    }
  }
  buttonPushed = 0;
  noInterrupts();
  while (digitalRead(PIN_ROT_SW) == LOW) {
    delay(10);
  }
  if (menuItem == 0) {
    return MAINMENU;
  } else if (menuItem == 1) {
    return RUNPROFILE;
  } else {
    return CONFIRMPROFILE;
  }
}

// See which profile was last run.
int run_default(Adafruit_ILI9341* ptft)
{
  int menuItem = 0;
  int buttonPushed = 0;
  ptft->fillScreen(ILI9341_BLACK);
  draw_screen_last_run(ptft, menuItem, allProfiles[DEFAULT_PROFILE_NUM]);
  interrupts();
  while (buttonPushed == 0) {
    if (encoderMovement != DIR_NONE) {
      encoderMovement = DIR_NONE;
      menuItem = (menuItem == 0) ? 1 : 0;
      draw_screen_last_run(ptft, menuItem, allProfiles[DEFAULT_PROFILE_NUM]);
    }
    if (digitalRead(PIN_ROT_SW) == LOW) {
      delay(6);  // 6ms to wait for the switch to calm down.
      buttonPushed = 1;
    }
  }
  noInterrupts();
  buttonPushed = 0;
  while (digitalRead(PIN_ROT_SW) == LOW) {
    delay(10);
  }
  if (menuItem == 0) {
    return MAINMENU;
  } else if (menuItem == 1) {
    return RUNPROFILE;
  } else {
    return LASTRUN;
  }
}

// Run the profile.
int run_profile(Adafruit_ILI9341* ptft, int* selectedProfileNum)
{
  ptft->fillScreen(ILI9341_BLACK);
  rflw_run_profile(ptft, allProfiles[*selectedProfileNum], 0);
  return MAINMENU;
}

// Shit's fucked yo.
int error_happened(ERRORS error)
{
  interrupts();
  draw_screen_error(ptft, ERROR_TOO_HOT);
  noInterrupts();
}

// ISR to see if the rotary encoder has moved CW.
void rotaryLeft(void) {
  if (digitalRead(PIN_ROT_SW) == LOW
      || digitalRead(PIN_ROT_B) == HIGH ) {
    return;
  } else {
    for (debounceCount = 0 ; debounceCount < 60 ; debounceCount++) {
      delayMicroseconds(100);
      if (digitalRead(PIN_ROT_A) == LOW) {
        return;
      }
    }
    encoderMovement = DIR_CW;
  }
}
// ISR to see if the rotary encoder has ACW.
void rotaryRight(void) {
  if (digitalRead(PIN_ROT_SW) == LOW
      || digitalRead(PIN_ROT_A) == HIGH ) {
    return;
  } else {
    for (debounceCount = 0 ; debounceCount < 60 ; debounceCount++) {
      delayMicroseconds(100);
      if (digitalRead(PIN_ROT_B) == LOW) {
        return;
      }
    }
    encoderMovement = DIR_ACW;
  }
}

// Sets up hardware and initial machine state.
void setup() {
  /* Hardware Setup */
  setup_heater();
  setup_temperature();
  setup_buzzer();
  // Stop the SD card messing up anything.
  pinMode(PIN_SDCARD_SS, OUTPUT);
  digitalWrite(PIN_SDCARD_SS, HIGH);
  // Rotary encoder and switch Inputs
  pinMode(PIN_ROT_SW, INPUT_PULLUP);  // Switch
  pinMode(PIN_ROT_A, INPUT);  // Rot A
  attachInterrupt(digitalPinToInterrupt(PIN_ROT_A), rotaryLeft, RISING);
  pinMode(PIN_ROT_B, INPUT);  // Rot B
  attachInterrupt(digitalPinToInterrupt(PIN_ROT_B), rotaryRight, RISING);
  noInterrupts();  // Disable interrupts (for now).
  // Display Setup
  setup_display(ptft);
}

// Main.
void loop() {
  int sdCardPresent = 0;  // '1' if SD card is plugged in.
  int buttonPushed = 0;
  int menuItem = 0;
  int selectedProfileNum = DEFAULT_PROFILE_NUM;
  HEATER_STATE heaterState = HEATER_OFF;
    
  GOTO_MAINMENU:
  menuItem = main_menu(ptft);
  switch(menuItem){
    case CHOOSEPROFILE:
      goto GOTO_CHOOSEPROFILE;
      break;
    case LASTRUN:
      goto GOTO_DEFAULTPROFILE;
      break;
    default:
      goto GOTO_MAINMENU;
      break;
  }
  
  GOTO_RUNPROFILE:
  menuItem = run_profile(ptft, &selectedProfileNum);
  switch(menuItem){
    case MAINMENU:
      goto GOTO_MAINMENU;
      break;
    default:
      goto GOTO_RUNPROFILE;
      break;
  }
  
  GOTO_CHOOSEPROFILE:
  menuItem = choose_profile(ptft, &selectedProfileNum);
  switch(menuItem){
    case CONFIRMPROFILE:
      goto GOTO_CONFIRMPROFILE;
      break;
    default:
      goto GOTO_CHOOSEPROFILE;
      break;
  }
  
  GOTO_CONFIRMPROFILE:
  menuItem = confirm_profile(ptft, &selectedProfileNum);
  switch(menuItem){
    case MAINMENU:
      goto GOTO_MAINMENU;
      break;
    case RUNPROFILE:
      goto GOTO_RUNPROFILE;
      break;
    default:
      goto GOTO_CONFIRMPROFILE;
      break;
  }
  
  GOTO_DEFAULTPROFILE:
  menuItem = run_default(ptft);
  switch(menuItem){
    case MAINMENU:
      goto GOTO_MAINMENU;
      break;
    case RUNPROFILE:
      goto GOTO_RUNPROFILE;
      break;
    default:
      goto GOTO_DEFAULTPROFILE;
      break;
  }
  
  while (1) {}
}
