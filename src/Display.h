/*********************************************************************


    Display Header File
    Version : 1.10


*********************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "MachineState.h"
#include <Arduino.h>

#define DISPLAY_OVERRIDE_BLANK_SCORE 0xFFFFFFFF

namespace DisplayHelper {
void blankAllDisplays();
void overrideScoreDisplay(byte displayNumber, score_t score, boolean flash = false);
void stopDisplayOverride();
void showPlayerScore(byte displayNumber, score_t score, boolean flashCurrent = false, boolean dashCurrent = false);
void showPlayerScores();
void setDisplayFlashing(byte displayNumber, boolean flash = false);
void displayHelperInitialize(MachineState& machineStatePtr);
} // namespace DisplayHelper

#endif
