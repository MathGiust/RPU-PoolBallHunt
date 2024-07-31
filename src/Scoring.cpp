/*********************************************************************


    Scoring Source File
    Version : 1.00


*********************************************************************/

#include "Scoring.h"

#include "sound.h"
#include "Time.h"

#include "RPU.h"
#include "PinballMachineBase.h"

static byte tensStack = 0;
static byte hundredsStack = 0;
static byte thousandsStack = 0;
static byte tensOfThousandStack = 0;

void Scoring::scoring(MachineState& machineState) {
    if( (Time::getCurrentTime() - Time::getLastScoringTime()) > 100 ) {
        Time::updateLastScoringTime();
        if (tensOfThousandStack) {
            machineState.increaseCurrentPlayerScore(10000);
            tensOfThousandStack -= 1;
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_10000_CHIME, false);
        } else if (thousandsStack) {
            machineState.increaseCurrentPlayerScore(1000);
            thousandsStack -= 1;
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_1000_CHIME, false);
        } else if (hundredsStack) {
            machineState.increaseCurrentPlayerScore(100);
            hundredsStack -= 1;
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_100_CHIME, false);
        } else if (tensStack) {
            machineState.increaseCurrentPlayerScore(10);
            tensStack -= 1;
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_10_CHIME, false);
        }
    }
}

void Scoring::emptyAllStacks() {
    tensStack = 0;
    hundredsStack = 0;
    thousandsStack = 0;
    tensOfThousandStack = 0;
}

//
//  Add to stacks
//
void Scoring::addToTensStack(byte amountToAdd) {
    tensStack += amountToAdd;
}

void Scoring::addToHundredStack(byte amountToAdd) {
    hundredsStack += amountToAdd;
}

void Scoring::addToThousandsStack(byte amountToAdd) {
    thousandsStack += amountToAdd;
}

void Scoring::addToTensOfThousandStack(byte amountToAdd) {
    tensOfThousandStack += amountToAdd;
}

void Scoring::increaseCurrentPlayerScore(score_t valueToAdd, MachineState& machineState) {
    machineState.increaseCurrentPlayerScore(valueToAdd);
}

//
//  Get stacks
//
byte Scoring::getToTensStack(byte amountToAdd) {
    return tensStack;
}

byte Scoring::getToHundredStack(byte amountToAdd) {
    return hundredsStack;
}

byte Scoring::getToThousandsStack(byte amountToAdd) {
    return thousandsStack;
}

byte Scoring::getToTensOfThousandStack(byte amountToAdd) {
    return tensOfThousandStack;
}
