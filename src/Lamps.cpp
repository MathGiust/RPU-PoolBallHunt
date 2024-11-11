/*********************************************************************


    Lamps Source File
    Version : 1.00


*********************************************************************/

#include "Lamps.h"

#include "RPU.h"
#include "RPU_Config.h"

#include <Arduino.h>

static byte* lampsPointer(byte lampCollection);
static byte  getLampCollectionSize(byte lampCollectionNumber);
byte**       animationPointer(int index);

//
//  LAMP COLLECTIONS
//

static byte descendingBonusValues[] = {
        40, 30, 20, 10,
        9, 8, 7, 6,
        5, 4, 3, 2,
        1
};

static byte descendingBonusLamps[] = {
        LAMP_BONUS_40K, LAMP_BONUS_30K, LAMP_BONUS_20K, LAMP_BONUS_10K,
        LAMP_BONUS_9K, LAMP_BONUS_8K, LAMP_BONUS_7K, LAMP_BONUS_6K,
        LAMP_BONUS_5K, LAMP_BONUS_4K, LAMP_BONUS_3K, LAMP_BONUS_2K,
        LAMP_BONUS_1K, LAMP_TERMINATOR
};

static byte centerGlobe[] = {
        LAMP_CENTER_G, LAMP_CENTER_L, LAMP_CENTER_O, LAMP_CENTER_B, LAMP_CENTER_E, LAMP_TERMINATOR
};

static byte saucerGlobe[] = {
    LAMP_SAUCER_G, LAMP_SAUCER_L, LAMP_SAUCER_O, LAMP_SAUCER_B, LAMP_SAUCER_E, LAMP_TERMINATOR
};

static byte sideTargets[] = {
    LAMP_SIDE_TARGET_1, LAMP_SIDE_TARGET_2, LAMP_SIDE_TARGET_3, LAMP_SIDE_TARGET_4, LAMP_SIDE_TARGET_5, LAMP_TERMINATOR
};

static byte spinnerValues[] = {
    LAMP_LEFT_SPINNER_100, LAMP_LEFT_SPINNER_200, LAMP_LEFT_SPINNER_1000, LAMP_LEFT_SPINNER_2000, LAMP_TERMINATOR
};

static byte centerSpinnerLamps[] = {
    LAMP_SPINNERS_1, LAMP_SPINNERS_2, LAMP_SPINNERS_3, LAMP_SPINNERS_4, LAMP_SPINNERS_5, LAMP_TERMINATOR
};

static byte dunkShotLamps[] = {
    LAMP_DUNK_SHOT_EB, LAMP_DUNK_SHOT_GATE, LAMP_DUNK_SHOT_5K, LAMP_DUNK_SHOT_SPECIAL, LAMP_TERMINATOR
};

//
//  END OF LAMP COLLECTIONS
//

// Turn off all lamps
void LampsHelper::hideAllLamps() {
    RPU_TurnOffAllLamps();
}

// Turn off playfield lamps --> Doesn't touch head lamps
void LampsHelper::hideAllPlayfieldLamps() {
    for (int count = 0; count < RPU_MAX_LAMPS; count++) {
        if (count == LAMP_BALL_IN_PLAY)
            continue;
        if (count == LAMP_CREDIT_INDICATOR)
            continue;
        if (count == LAMP_GAME_OVER)
            continue;
        if (count == LAMP_HIGH_SCORE)
            continue;
        if (count == LAMP_MATCH)
            continue;
        if (count == LAMP_SAME_PLAYER_SHOOTS_AGAIN)
            continue;
        if (count == LAMP_TILT)
            continue;

        RPU_SetLampState(count, 0);
    }
}

// Hide single Lamp
void LampsHelper::hideLamp(byte lamp) {
    RPU_SetLampState(lamp, 0);
}

// Show single lamp
void LampsHelper::showLamp(byte lamp, byte dim, byte flashPeriod) {
    RPU_SetLampState(lamp, 1, dim, flashPeriod);
}

// Hide lamp collection
void LampsHelper::hideLamps(byte lampCollection) {
    byte* pointer = lampsPointer(lampCollection);
    for (byte lamp = 0; pointer[lamp] != LAMP_TERMINATOR; lamp++) {
        RPU_SetLampState(pointer[lamp], 0);
    }
}

// Show lamp collection
void LampsHelper::showLamps(byte lampCollection, byte flashPeriod) {
    byte* pointer = lampsPointer(lampCollection);
    for (byte lamp = 0; pointer[lamp] != LAMP_TERMINATOR; lamp++) {
        RPU_SetLampState(pointer[lamp], 1, 0, flashPeriod);
    }
}

// Show all lamps
void LampsHelper::showAllLamps(int flashPeriod) {
    for (byte lamp = 0; lamp < NUMBER_OF_LAMPS; lamp++) {
        RPU_SetLampState(lamp, 1, 0, flashPeriod);
    }
}

// Show all bonus lamps
void LampsHelper::showBonusLamps(byte bonusValue) {
    for (byte itr = 0; itr < sizeof(descendingBonusValues); itr++) {
        if (bonusValue >= descendingBonusValues[itr]) {
            showLamp(descendingBonusLamps[itr]);
            bonusValue -= descendingBonusValues[itr];
        } else {
            hideLamp(descendingBonusLamps[itr]);
        }
    }
}

// Show Bonus-X lamps
void LampsHelper::showBonusMultiplierLamps(byte value, byte flashPeriod) {
    if (value < 2)
        // hideLamps(LAMP_COLL_BONUS_X);
        if (value >= 2)
            showLamp(LAMP_2X_BONUS, flashPeriod);
    if (value >= 3)
        showLamp(LAMP_3X_BONUS, flashPeriod);
    if (value >= 5)
        showLamp(LAMP_5X_BONUS, flashPeriod);
}

void LampsHelper::showLampFromCollection(byte lampCollection, byte indexToShow, byte dim, byte flashPeriod) {
    byte* pointer = lampsPointer(lampCollection);
    RPU_SetLampState(pointer[indexToShow], 1, dim, flashPeriod);
}

void LampsHelper::hideLampFromCollection(byte lampCollection, byte indexToShow) {
    byte* pointer = lampsPointer(lampCollection);
    RPU_SetLampState(pointer[indexToShow], 0);
}

void LampsHelper::showOneLampFromCollection(byte lampCollection, byte indexToShow, byte flashPeriod) {
    byte* pointer = lampsPointer(lampCollection);
    for (byte lamp = 0; pointer[lamp] != LAMP_TERMINATOR; lamp++) {
        if (lamp == indexToShow) RPU_SetLampState(pointer[lamp], 1, 0, flashPeriod);
        else RPU_SetLampState(pointer[lamp], 0);
    }
}

//
//  Private :
//

// Return the address of the lamp collection
static byte* lampsPointer(byte lampCollection) {
    byte* pointer;
    switch (lampCollection) {
    case LAMP_COLL_BONUS_LAMPS:
        pointer = descendingBonusLamps;
        break;
    case LAMP_COLL_CENTER_GLOBE:
        pointer = centerGlobe;
        break;
    case LAMP_COLL_SAUCER_GLOBE:
        pointer = saucerGlobe;
        break;
    case LAMP_COLL_SIDE_TARGET:
        pointer = sideTargets;
        break;
    case LAMP_COLL_SPINNER_VALUES:
        pointer = spinnerValues;
        break;
    case LAMP_COLL_CENTER_SPINNER:
        pointer = centerSpinnerLamps;
        break;
    case LAMP_COLL_DUNK_SHOT:
        pointer = dunkShotLamps;
        break;
    default:
        static byte EmptyCollection[]{};
        pointer = EmptyCollection;
        break;
    }
    return pointer;
}
static byte getLampCollectionSize(byte lampCollectionNumber) {
    byte lampCollectionSize;
    switch (lampCollectionNumber) {
    case LAMP_COLL_BONUS_LAMPS:
        lampCollectionSize = sizeof(descendingBonusLamps);
        break;
    case LAMP_COLL_CENTER_GLOBE:
        lampCollectionSize = sizeof(centerGlobe);
        break;
    case LAMP_COLL_SAUCER_GLOBE:
        lampCollectionSize = sizeof(saucerGlobe);
        break;
    case LAMP_COLL_SIDE_TARGET:
        lampCollectionSize = sizeof(sideTargets);
        break;
    case LAMP_COLL_SPINNER_VALUES:
        lampCollectionSize = sizeof(spinnerValues);
        break;
    case LAMP_COLL_CENTER_SPINNER:
        lampCollectionSize = sizeof(centerSpinnerLamps);
        break;
    case LAMP_COLL_DUNK_SHOT:
        lampCollectionSize = sizeof(dunkShotLamps);
        break;
    default:
        lampCollectionSize = 0;
        break;
    }
    return lampCollectionSize;
}

/******************************************
 *
 * Lamps animation
 *
 ******************************************/

/*
  There are two types of lamps animations...
  - Attract animations --> Are used in attract state, completely condition free
  - Game animations --> These animation should affect only lamps that are used in the animation.
 */

/*
byte **animationPointer(int index) {
    switch (index) {
    default:
    case 0:
        return animation1;
    case 1:
        return &animation2;
    }
}
*/

void LampsHelper::showLampAnimation(
        const byte   animationNum[][8],
        byte         frames,
        Time::time_t divisor,
        byte         subOffset,
        bool         dim,
        bool         reverse,
        byte         keepLampOn,
        bool         auxLamps
) {
    byte currentStep = ((Time::getCurrentTime() - Time::getLastLampAnimUpdateTime()) / divisor) % frames;
    if (reverse) currentStep = (frames - 1) - currentStep;

    byte lampNum = 0;

    const int max = (auxLamps) ? 2 : 8;

    for (int byteNum = 0; byteNum < max; byteNum++) {
        //  for (int byteNum = 0; byteNum < 8; byteNum++) {
        for (byte bitNum = 0; bitNum < 8; bitNum++) {

            // if there's a subOffset, turn off lights at that offset
            if (subOffset) {
                byte lampOff = true;
                lampOff = animationNum[(currentStep + subOffset) % frames][byteNum] & (1 << bitNum);
                if (lampOff && lampNum != keepLampOn) RPU_SetLampState((lampNum + ((auxLamps) ? 60 : 0)), 0);
                //        if (lampOff && lampNum != keepLampOn) RPU_SetLampState(lampNum, 0);
            }

            byte lampOn = false;
            lampOn = animationNum[currentStep][byteNum] & (1 << bitNum);
            if (lampOn) RPU_SetLampState((lampNum + ((auxLamps) ? 60 : 0)), 1, dim);
            //      if (lampOn) RPU_SetLampState(lampNum, 1, dim);

            lampNum += 1;
        }
#ifndef RPU_OS_SOFTWARE_DISPLAY_INTERRUPT
        if (byteNum % 2) RPU_DataRead(0);
#endif
    }
}

void LampsHelper::sweepLampCollection(byte lampColNum, byte stepTime, byte offset) {
    byte  turnOffStep;
    byte  lampCollectionSize = getLampCollectionSize(lampColNum) - 1;
    byte* lampCollection = lampsPointer(lampColNum);
    byte  turnOnStep = ((Time::getCurrentTime() - Time::getLastLampAnimUpdateTime()) / stepTime) % lampCollectionSize;
    if (offset > turnOnStep) turnOffStep = lampCollectionSize - (offset - turnOnStep);
    else turnOffStep = turnOnStep - offset;

    showLampFromCollection(lampColNum, turnOnStep, 0, 0);
    hideLampFromCollection(lampColNum, turnOffStep);
}

void LampsHelper::fillLampCollection(byte lampColNum, byte stepTime) {
    byte lampCollectionSize = getLampCollectionSize(lampColNum);
    byte currentStep = ((Time::getCurrentTime() - Time::getLastLampAnimUpdateTime()) / stepTime) % lampCollectionSize;
    if (currentStep < (lampCollectionSize - 1)) showLampFromCollection(lampColNum, currentStep, 0, 0);
    else hideLamps(lampColNum);
}

void LampsHelper::glowLamp(byte lamp, int stepTime) {
    byte currentStep = ((Time::getCurrentTime() - Time::getLastLampAnimUpdateTime()) / stepTime) % 16;
    switch (currentStep) {
    default:
    case 0:
    case 1:
    case 2:
        hideLamp(lamp);
        break;
    case 3:
    case 4:
        showLamp(lamp, 2, 0);
        break;
    case 5:
    case 6:
        showLamp(lamp, 1, 0);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        showLamp(lamp, 0, 0);
        break;
    case 12:
    case 13:
        showLamp(lamp, 1, 0);
        break;
    case 14:
    case 15:
        showLamp(lamp, 2, 0);
        break;
    }
}

//
//  Game animation
//

#define NUM_GAME_LAMP_ANIMATIONS 4

static boolean gameAnimationsStatus[NUM_GAME_LAMP_ANIMATIONS];

void LampsHelper::setGameAnimationStatus(byte animationNum, boolean status) {
    if (animationNum < NUM_GAME_LAMP_ANIMATIONS) gameAnimationsStatus[animationNum] = status;
}

void LampsHelper::runGameAnimations() {
    for (byte animationNum = 0; animationNum < NUM_GAME_LAMP_ANIMATIONS; animationNum++) {
        // TODO : run Animation
    }
}
