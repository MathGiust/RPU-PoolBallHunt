/*********************************************************************


Game Modes Source File
Version : 1.00


*********************************************************************/

#include "GameModes.h"

#include "Display.h"
#include "Lamps.h"
#include "PinballMachineBase.h"
#include "Scoring.h"
#include "Time.h"
#include "sound.h"

BaseMode::BaseMode() : modeTimer(0),
                       modeValue(0) {
}

void BaseMode::manageNewMode() {
    if (DEBUG_MESSAGES) Serial.write("Method parent\n\r");
    setModeTimer(15);
}

void BaseMode::increaseModeTimer(int valueToAdd) {
    this->modeTimer += valueToAdd;
}

void BaseMode::decreaseModeTimer(int valueToAdd) {
    this->modeTimer -= valueToAdd;
}

void BaseMode::setModeTimer(int value) {
    this->modeTimer = value;
}

int BaseMode::getModeTimer() const {
    return this->modeTimer;
}

byte BaseMode::manageModeTimer(byte currentGameMode) {
    if (modeTimer <= 0) {
        DisplayHelper::stopDisplayOverride();
        return GAME_MODE_UNSTRUCTURED_PLAY;
    }
    if ((Time::getCurrentTime() - Time::getLastModeTimerUpdateTime()) >= 1000) {
        Time::updateLastModeTimerUpdateTime();
        modeTimer--;
    }
    return currentGameMode;
}

score_t BaseMode::getModeValue() {
    return modeValue;
}

void BaseMode::manageSwitchHit(byte switchHit, MachineState& machineState) {
}

/******************************************
 *
 * Spinner Mode
 *
 ******************************************/

// Define all spinner values and progression
const static int spinnerValues[]{
        200,
        400,
        800,
        1000,
        1500,
        2000
};

void SpinnerMode::manageNewMode() {
    setModeTimer(30);
    resetSpinnerValue();
}

score_t SpinnerMode::getModeValue() {
    return spinnerValues[currentSpinnerValue];
}

void SpinnerMode::manageSwitchHit(byte switchHit, MachineState& machineState) {
    // Manage switch hit
    if (switchHit == SW_SPINNER) Scoring::increaseCurrentPlayerScore(getModeValue(), machineState);
    else if (switchHit == SW_8_BALL) {
        ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_INCREASE_SPINNER, false);
        increaseSpinnerValue();
        increaseModeTimer(10);
        if (getModeTimer() > 30) setModeTimer(31);
    }
}

SpinnerMode::SpinnerMode() : currentSpinnerValue(0) {
}

void SpinnerMode::resetSpinnerValue() {
    currentSpinnerValue = 0;
}

void SpinnerMode::increaseSpinnerValue() {
    if (currentSpinnerValue < sizeof(spinnerValues)) currentSpinnerValue++;
}
// End of spinner Mode

/******************************************
 *
 * Pop Mode
 *
 ******************************************/

static int popValues[]{
        200,
        400,
        800,
        1000,
        1500,
        2000
};

PopMode::PopMode() : currentPopValue(0),
                     hurryUpValue(0){
                     };

void PopMode::manageNewMode() {
    setModeTimer(30);
}
score_t PopMode::getModeValue() {
    return hurryUpValue;
}
void PopMode::manageSwitchHit(byte switchHit, MachineState& machineState) {
    if (switchHit == SW_BANK_SHOT) {
        machineState.currentPlayer->increaseScore(this->getModeValue());
        ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_INCREASE_SPINNER, false);
    }
}
byte PopMode::manageModeTimer(byte currentGameMode) {
    if (getModeTimer() <= 0) {
        DisplayHelper::stopDisplayOverride();
        return GAME_MODE_UNSTRUCTURED_PLAY;
    }
    if ((Time::getCurrentTime() - Time::getLastModeTimerUpdateTime()) > 1000) {
        Time::updateLastModeTimerUpdateTime();
        decreaseModeTimer(1);
    }
    if (((Time::getCurrentTime() - Time::getLastSubModeTimerUpdateTime()) >= 100) && (hurryUpValue >= 10000)) {
        Time::updateLastSubModeTimerUpdateTime();
        hurryUpValue -= 100;
    }
    return currentGameMode;
    return currentGameMode;
}

void PopMode::increasePopValue() {
    if (currentPopValue < sizeof(popValues)) currentPopValue++;
}
void PopMode::increaseHurryUpValue() {
    if(DEBUG_MESSAGES) {
        Serial.print("Pop  value = "); Serial.print(getPopValue(), DEC); Serial.write("\n\r");
        Serial.print("Mode value = "); Serial.print(getModeValue(), DEC); Serial.write("\n\r");
    }
    hurryUpValue += getPopValue();
}
void PopMode::resetPopValue() {
    currentPopValue = 0;
}
score_t PopMode::getPopValue() const {
    return popValues[currentPopValue];
}

// End of spinner Mode
