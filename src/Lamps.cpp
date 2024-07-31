/*********************************************************************


    Lamps Source File
    Version : 1.00


*********************************************************************/

#include "Lamps.h"

#include "Arduino.h"
#include "LampAnimations.h"
#include "RPU.h"
#include "RPU_Config.h"

static byte* lampsPointer(byte lampCollection);
static byte  getLampCollectionSize(byte lampCollectionNumber);
byte**       animationPointer(int index);

// LAMP COLL 0
static byte singleDigitBonusLamps[] = {
        LAMP_UNUSED,
        LAMP_BIG_1,
        LAMP_BIG_2,
        LAMP_BIG_3,
        LAMP_BIG_4,
        LAMP_BIG_5,
        LAMP_BIG_6,
        LAMP_BIG_7,
        LAMP_BIG_8,
        LAMP_BIG_9,
        LAMP_TERMINATOR
};

// LAMP COLL 1
static byte bigBallsLamps[] = {
        LAMP_BIG_1,
        LAMP_BIG_2,
        LAMP_BIG_3,
        LAMP_BIG_4,
        LAMP_BIG_5,
        LAMP_BIG_6,
        LAMP_BIG_7,
        LAMP_BIG_8,
        LAMP_BIG_9,
        LAMP_BIG_10,
        LAMP_BIG_11,
        LAMP_BIG_12,
        LAMP_BIG_13,
        LAMP_BIG_14,
        LAMP_BIG_15,
        LAMP_TERMINATOR
};

// LAMP COLL 2
static byte smallBallsLamps[] = {
        LAMP_SMALL_1,
        LAMP_SMALL_2,
        LAMP_SMALL_3,
        LAMP_SMALL_4,
        LAMP_SMALL_5,
        LAMP_SMALL_6,
        LAMP_SMALL_7,
        LAMP_SMALL_8,
        LAMP_SMALL_9,
        LAMP_SMALL_10,
        LAMP_SMALL_11,
        LAMP_SMALL_12,
        LAMP_SMALL_13,
        LAMP_SMALL_14,
        LAMP_SMALL_15,
        LAMP_TERMINATOR
};

// LAMP COLL 3
static byte evenPlayerBigBallsLamps[] = {
        LAMP_BIG_1,
        LAMP_BIG_2,
        LAMP_BIG_3,
        LAMP_BIG_4,
        LAMP_BIG_5,
        LAMP_BIG_6,
        LAMP_BIG_7,
        LAMP_BIG_8,
        LAMP_TERMINATOR
};

// LAMP COLL 4
static byte oddPlayerBigBallsLamps[] = {
        LAMP_BIG_9,
        LAMP_BIG_10,
        LAMP_BIG_11,
        LAMP_BIG_12,
        LAMP_BIG_13,
        LAMP_BIG_14,
        LAMP_BIG_15,
        LAMP_BIG_8,
        LAMP_TERMINATOR
};

// LAMP COLL 5
static byte evenPlayerSmallBallsLamps[] = {
        LAMP_SMALL_1,
        LAMP_SMALL_2,
        LAMP_SMALL_3,
        LAMP_SMALL_4,
        LAMP_SMALL_5,
        LAMP_SMALL_6,
        LAMP_SMALL_7,
        LAMP_SMALL_8,
        LAMP_TERMINATOR
};

// LAMP COLL 6
static byte oddPlayerSmallBallsLamps[] = {
        LAMP_SMALL_9,
        LAMP_SMALL_10,
        LAMP_SMALL_11,
        LAMP_SMALL_12,
        LAMP_SMALL_13,
        LAMP_SMALL_14,
        LAMP_SMALL_15,
        LAMP_SMALL_8,
        LAMP_TERMINATOR
};

// LAMP COLL 7
static byte upperAlleyBallsLamps[] = {
        LAMP_SMALL_1,
        LAMP_SMALL_2,
        LAMP_SMALL_3,
        LAMP_SMALL_4,
        LAMP_SMALL_9,
        LAMP_SMALL_10,
        LAMP_SMALL_11,
        LAMP_SMALL_12,
        LAMP_TERMINATOR
};

// LAMP COLL 8
static byte bankShotAllLamps[] = {
        LAMP_BANK_SHOT_300,
        LAMP_BANK_SHOT_600,
        LAMP_BANK_SHOT_900,
        LAMP_BANK_SHOT_1200,
        LAMP_BANK_SHOT_1500,
        LAMP_BANK_SHOT_5000,
        LAMP_LEFT_ARROW,
        LAMP_RIGHT_ARROW,
        LAMP_TERMINATOR
};

// LAMP COLL 9
static byte bankShotLampsAlleyLamps[] = {
        LAMP_BANK_SHOT_300,
        LAMP_BANK_SHOT_600,
        LAMP_BANK_SHOT_900,
        LAMP_BANK_SHOT_1200,
        LAMP_BANK_SHOT_1500,
        LAMP_BANK_SHOT_5000,
        LAMP_TERMINATOR
};

// LAMP COLL 10
static byte bankShotArrowsLamps[] = {
        LAMP_LEFT_ARROW,
        LAMP_RIGHT_ARROW,
        LAMP_TERMINATOR
};

// LAMP COLL 11
static byte bonusXLamps[] = {
        LAMP_2X_BONUS,
        LAMP_3X_BONUS,
        LAMP_5X_BONUS,
        LAMP_TERMINATOR
};

static byte descendingBonusValues[] = {
        15,
        14,
        13,
        12,
        11,
        10,
        9,
        8,
        7,
        6,
        5,
        4,
        3,
        2,
        1,
};

static byte descendingBonusLamps[]{
        LAMP_BIG_15,
        LAMP_BIG_14,
        LAMP_BIG_13,
        LAMP_BIG_12,
        LAMP_BIG_11,
        LAMP_BIG_10,
        LAMP_BIG_9,
        LAMP_BIG_8,
        LAMP_BIG_7,
        LAMP_BIG_6,
        LAMP_BIG_5,
        LAMP_BIG_4,
        LAMP_BIG_3,
        LAMP_BIG_2,
        LAMP_BIG_1,
        LAMP_TERMINATOR
};

static byte bottomHeadLamps[]{
        LAMP_1_PLAYER,
        LAMP_2_PLAYER,
        LAMP_3_PLAYER,
        LAMP_4_PLAYER,
        LAMP_SAME_PLAYER_SHOOTS_AGAIN,
        LAMP_TILT,
        LAMP_TERMINATOR
};

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
        if (count == LAMP_SHOOT_AGAIN)
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
        hideLamps(LAMP_COLL_BONUS_X);
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
    case LAMP_COLL_BONUS_ALL:
        pointer = singleDigitBonusLamps;
        break;
    case LAMP_COLL_BIG_BALLS:
        pointer = bigBallsLamps;
        break;
    case LAMP_COLL_SMALL_BALLS:
        pointer = smallBallsLamps;
        break;
    case LAMP_COLL_EVEN_PLAYER_BIG_BALLS:
        pointer = evenPlayerBigBallsLamps;
        break;
    case LAMP_COLL_ODD_PLAYER_BIG_BALLS:
        pointer = oddPlayerBigBallsLamps;
        break;
    case LAMP_COLL_EVEN_PLAYER_SMALL_BALLS:
        pointer = evenPlayerSmallBallsLamps;
        break;
    case LAMP_COLL_ODD_PLAYER_SMALL_BALLS:
        pointer = oddPlayerSmallBallsLamps;
        break;
    case LAMP_COLL_UPPER_ALLEY_BALLS:
        pointer = upperAlleyBallsLamps;
        break;
    case LAMP_COLL_BANK_SHOT_ALL:
        pointer = bankShotAllLamps;
        break;
    case LAMP_COLL_BANK_SHOT_ALLEY:
        pointer = bankShotLampsAlleyLamps;
        break;
    case LAMP_COLL_BANK_SHOT_ARROWS:
        pointer = bankShotArrowsLamps;
        break;
    case LAMP_COLL_BONUS_X:
        pointer = bonusXLamps;
        break;
    case LAMP_COLL_BONUS_DESCENDING:
        pointer = descendingBonusLamps;
        break;
    case LAMP_COLL_BOTTOM_HEAD:
        pointer = bottomHeadLamps;
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
    case LAMP_COLL_BONUS_ALL:
        lampCollectionSize = sizeof(singleDigitBonusLamps);
        break;
    case LAMP_COLL_BIG_BALLS:
        lampCollectionSize = sizeof(bigBallsLamps);
        break;
    case LAMP_COLL_SMALL_BALLS:
        lampCollectionSize = sizeof(smallBallsLamps);
        break;
    case LAMP_COLL_EVEN_PLAYER_BIG_BALLS:
        lampCollectionSize = sizeof(evenPlayerBigBallsLamps);
        break;
    case LAMP_COLL_ODD_PLAYER_BIG_BALLS:
        lampCollectionSize = sizeof(oddPlayerBigBallsLamps);
        break;
    case LAMP_COLL_EVEN_PLAYER_SMALL_BALLS:
        lampCollectionSize = sizeof(evenPlayerSmallBallsLamps);
        break;
    case LAMP_COLL_ODD_PLAYER_SMALL_BALLS:
        lampCollectionSize = sizeof(oddPlayerSmallBallsLamps);
        break;
    case LAMP_COLL_UPPER_ALLEY_BALLS:
        lampCollectionSize = sizeof(upperAlleyBallsLamps);
        break;
    case LAMP_COLL_BANK_SHOT_ALL:
        lampCollectionSize = sizeof(bankShotAllLamps);
        break;
    case LAMP_COLL_BANK_SHOT_ALLEY:
        lampCollectionSize = sizeof(bankShotLampsAlleyLamps);
        break;
    case LAMP_COLL_BANK_SHOT_ARROWS:
        lampCollectionSize = sizeof(bankShotArrowsLamps);
        break;
    case LAMP_COLL_BONUS_X:
        lampCollectionSize = sizeof(bonusXLamps);
        break;
    case LAMP_COLL_BONUS_DESCENDING:
        lampCollectionSize = sizeof(descendingBonusLamps);
        break;
    case LAMP_COLL_BOTTOM_HEAD:
        lampCollectionSize = sizeof(bottomHeadLamps);
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
    byte  lampCollectionSize = getLampCollectionSize(lampColNum);
    byte  currentStep = ((Time::getCurrentTime() - Time::getLastLampAnimUpdateTime()) / stepTime) % lampCollectionSize;
    if(currentStep < (lampCollectionSize - 1)) showLampFromCollection(lampColNum, currentStep, 0, 0);
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
