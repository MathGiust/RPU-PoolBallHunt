/*********************************************************************


    Display Source File
    Version : 1.10


*********************************************************************/

#include "Display.h"

#include "MachineState.h"
#include "RPU.h"
#include "RPU_Config.h"
#include "Time.h"

static boolean       updateLastTimeAnimated;
static boolean       scoreOverrideStatus[4];
static boolean       overrideFlashStatus[4];
static score_t       scoreOverrideValue[4];
static byte          lastScrollPhase;
static unsigned long lastFlashOrDash;
static unsigned long lastTimeOverrideAnimated;
static unsigned long overrideAnimationSeed;

static byte getDisplayMask(byte numDigits);
static byte magnitudeOfScore(score_t score);
static byte numberOfDigits(score_t score);
static void dashScore(byte displayNumber, score_t score);
static void flashScore(byte displayNumber, score_t score);
static void scrollScore(byte displayNumber, score_t score);
static void showAnimatedPlayerScore(byte displayNumber, score_t score);
static void showScoreOverride(byte displayNumber);

static MachineState* machineState = nullptr;

//
//  Public :
//

void DisplayHelper::blankAllDisplays() {
    for(byte displayNumber = 0; displayNumber <= 3; displayNumber++) {
        RPU_SetDisplayBlank(displayNumber, 0);
    }
}

void DisplayHelper::displayHelperInitialize(MachineState& machineStatePtr) {
    machineState = &machineStatePtr;
}

void DisplayHelper::overrideScoreDisplay(byte displayNum, score_t score, boolean flash) {
    scoreOverrideStatus[displayNum] = true;
    scoreOverrideValue[displayNum] = score;
    overrideFlashStatus[displayNum] = flash;
}

void DisplayHelper::showPlayerScore(byte displayNumber, score_t score, boolean flashCurrent, boolean dashCurrent) {
    if (flashCurrent) {
        flashScore(displayNumber, score);
    } else if (dashCurrent) {
        dashScore(displayNumber, score);
    } else {
        RPU_SetDisplay(displayNumber, score, true, 2);
    }
}

void DisplayHelper::showPlayerScores() {
    for (byte displayNumber = 0; displayNumber <= 3; displayNumber++) {
        if (scoreOverrideStatus[displayNumber]) showPlayerScore(displayNumber, scoreOverrideValue[displayNumber], overrideFlashStatus[displayNumber]);
        else if (displayNumber < machineState->getNumberOfPlayers()) {
            showPlayerScore(displayNumber, machineState->getPlayerScore(displayNumber));
        } else RPU_SetDisplayBlank(displayNumber, 0);
    }
}

void DisplayHelper::stopDisplayOverride() {
    for (byte displayNumber = 0; displayNumber <= 3; displayNumber++) {
        scoreOverrideStatus[displayNumber] = false;
        overrideFlashStatus[displayNumber] = false;
        scoreOverrideValue[displayNumber] = 0;
    }
    showPlayerScores();
}

void DisplayHelper::setDisplayFlashing(byte displayNumber, boolean flash) {
    overrideFlashStatus[displayNumber] = flash;
}

//
//  Private :
//

byte getDisplayMask(byte numDigits) {
    byte displayMask = 0;
    for (byte digitCount = 0; digitCount < numDigits; digitCount++) {
#ifdef RPU_USE_7_DIGIT_DISPLAYS
        displayMask |= (0x40 >> digitCount);
#else
        displayMask |= (0x20 >> digitCount);
#endif
    }
    return displayMask;
}

byte magnitudeOfScore(score_t score) {
    if (score == 0)
        return 0;

    byte magnitudeOfScore = 0;
    while (score > 0) {
        score = score / 10;
        magnitudeOfScore += 1;
    }

    return magnitudeOfScore;
}

byte numberOfDigits(score_t score) {
    if (score == 0)
        return 1;

    return magnitudeOfScore(score);
}

void dashScore(byte displayNumber, score_t score) {
    unsigned long dashSeed = Time::getCurrentTime() / 50;
    byte          displayMask = 0x7F;

    if (dashSeed != lastFlashOrDash) {
        lastFlashOrDash = dashSeed;
        byte dashPhase = (Time::getCurrentTime() / 60) % 36;
        byte numDigits = magnitudeOfScore(score);

        if (dashPhase < 12) {
            displayMask = getDisplayMask((numDigits == 0) ? 2 : numDigits);
            if (dashPhase < 7) {
                for (byte maskCount = 0; maskCount < dashPhase; maskCount++) {
                    displayMask &= ~(0x01 << maskCount);
                }
            } else {
                for (byte maskCount = 12; maskCount > dashPhase; maskCount--) {
                    displayMask &= ~(0x60 >> (maskCount - dashPhase - 1));
                }
            }
            RPU_SetDisplay(displayNumber, score);
            RPU_SetDisplayBlank(displayNumber, displayMask);
        } else {
            RPU_SetDisplay(displayNumber, score, true, 2);
        }
    }
}

void flashScore(byte displayNumber, score_t score) {
    unsigned long flashSeed = Time::getCurrentTime() / 250;

    if (flashSeed != lastFlashOrDash) {
        lastFlashOrDash = flashSeed;

        if (((Time::getCurrentTime() / 250) % 2) == 0)
            RPU_SetDisplayBlank(displayNumber, 0x00);
        else
            RPU_SetDisplay(displayNumber, score, true, 2);
    }
}

void scrollScore(byte displayNumber, score_t score) {
    unsigned long timeSinceLastScoreChange = Time::getCurrentTime() - Time::getMostRecentSwitchHitTime();
    byte          displayMask;
    byte          scrollPhaseChanged = false;

    byte scrollPhase = (timeSinceLastScoreChange / 200) % 16;
    if (scrollPhase != lastScrollPhase) {
        lastScrollPhase = scrollPhase;
        scrollPhaseChanged = true;
    }

    if (timeSinceLastScoreChange < 3000) {
        RPU_SetDisplay(displayNumber, (score % (RPU_OS_MAX_DISPLAY_SCORE + 1)), false);
        RPU_SetDisplayBlank(displayNumber, RPU_OS_ALL_DIGITS_MASK);
    } else {
        // Scores are scrolled 10 digits and then we wait for 6
        if (scrollPhase < 11 && scrollPhaseChanged) {
            byte numDigits = magnitudeOfScore(score);

            // Figure out top part of score
            score_t tempScore = score;

            if (scrollPhase < RPU_OS_NUM_DIGITS) {
                displayMask = RPU_OS_ALL_DIGITS_MASK;
                for (byte scrollCount = 0; scrollCount < scrollPhase; scrollCount++) {
                    score = (score % (RPU_OS_MAX_DISPLAY_SCORE + 1)) * 10;
                    displayMask = displayMask >> 1;
                }
            } else {
                score = 0;
                displayMask = 0x00;
            }

            // Add in lower part of score
            if ((numDigits + scrollPhase) > 10) {
                byte numDigitsNeeded = (numDigits + scrollPhase) - 10;
                for (byte scrollCount = 0; scrollCount < (numDigits - numDigitsNeeded); scrollCount++) {
                    tempScore /= 10;
                }
                displayMask |= getDisplayMask(magnitudeOfScore(tempScore));
                score += tempScore;
            }
            RPU_SetDisplayBlank(displayNumber, displayMask);
            RPU_SetDisplay(displayNumber, score);
        }
    }
}

void showAnimatedPlayerScore(byte displayNumber, score_t score) {
    if (overrideAnimationSeed != lastTimeOverrideAnimated) {
        byte numDigits = numberOfDigits(score);
        updateLastTimeAnimated = true;

        byte shiftDigits = (overrideAnimationSeed) % (((RPU_OS_NUM_DIGITS + 1) - numDigits) + ((RPU_OS_NUM_DIGITS - 1) - numDigits));
        if (shiftDigits >= ((RPU_OS_NUM_DIGITS + 1) - numDigits))
            shiftDigits = (RPU_OS_NUM_DIGITS - numDigits) * 2 - shiftDigits;
        byte digitCount;
        byte displayMask = getDisplayMask(numDigits);
        for (digitCount = 0; digitCount < shiftDigits; digitCount++) {
            score *= 10;
            displayMask = displayMask >> 1;
        }
        RPU_SetDisplayBlank(displayNumber, 0x00);
        RPU_SetDisplay(displayNumber, score, false);
        RPU_SetDisplayBlank(displayNumber, displayMask);
    }
}

int getDisplayNumber(int playerIndex) {
    return playerIndex;
}

void showScoreOverride(byte displayNumber) {
    unsigned long score = scoreOverrideValue[displayNumber];
    if (score == DISPLAY_OVERRIDE_BLANK_SCORE) {
        RPU_SetDisplayBlank(displayNumber, 0);
    } else {
        RPU_SetDisplay(displayNumber, score, true, 1);
    }
}
