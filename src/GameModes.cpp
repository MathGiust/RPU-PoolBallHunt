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