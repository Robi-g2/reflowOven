#ifndef _BUZZER_H
#define _BUZZER_H

#include "Arduino.h"
#include "misc.h"

typedef enum {BUZZER_ON, BUZZER_OFF} BUZZER_STATE;

int setup_buzzer(void);
void buzz_error(void);
void buzz_profileRunning(void);
void buzz_coolEnough(void);
void sound_buzzer(BUZZER_STATE buzzerState);

#endif
