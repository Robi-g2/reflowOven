/*
   Author: Rob Gower
   Created: 17/Oct/2019
   Purpose: Any function that uses the SD Card.
*/

#include "buzzer.h"
#define PIN_BUZZER 0

// Set up the buzzer hardware.
int setup_buzzer(void) {
  // Buzzer Output
  pinMode(PIN_BUZZER, OUTPUT);
  sound_buzzer(BUZZER_OFF);
  delay(200);
  sound_buzzer(BUZZER_ON);
  delay(200);
  sound_buzzer(BUZZER_OFF);
  return 0;
}

// Turn the buzzer on or off.
void sound_buzzer(BUZZER_STATE buzzerState) {
  if (buzzerState == BUZZER_ON) { // On
    digitalWrite(PIN_BUZZER, HIGH);
  } else {  // Off
    digitalWrite(PIN_BUZZER, LOW);
  }
}

// Continuous Tone = error.
void buzz_error(void) {
  sound_buzzer(BUZZER_ON);
}

// blipping = profile running.
void buzz_profileRunning(void) {
  sound_buzzer(BUZZER_ON);
  delay(200);
  sound_buzzer(BUZZER_OFF);
}

// beeping = board can be removed.
void buzz_coolEnough(void) {
  sound_buzzer(BUZZER_ON);
  delay(500);
  sound_buzzer(BUZZER_OFF);
  delay(200);
}
