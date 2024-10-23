/*********************************************************************


    Player State Source File


*********************************************************************/

#include "PlayerState.h"

#include "GameModes.h"
#include "PinballMachineBase.h"
#include "Scoring.h"

#include <Arduino.h>

PlayerState::PlayerState()
    : score(0),
      bonusMultiplier(1),
      bonus(0),
      modePlayed{},
      modeQualified{},
      gateLit(false),
      leftSpinnerBonusAdvanceLit(false),
      globeLetters{},
      superBonus(0),
      leftSpinnerValue(0),
      centerSpinnerAdv(0) {}

// Setters:
void PlayerState::setScore(score_t newScore) {
    this->score = newScore;
}

void PlayerState::setBonusValue(byte newBonus) {
    bonus = newBonus;
}

// Getters :
score_t PlayerState::getScore() const {
    return score;
}
byte PlayerState::getBonusMultiplier() const {
    return bonusMultiplier;
}
byte PlayerState::getBonus() const {
    return bonus;
}

void PlayerState::decreaseBonus() {
    if (bonus > 0) bonus -= 1;
}
void PlayerState::decreaseBonus(byte amountToSubtract) {
    if (bonus > 0) bonus -= amountToSubtract;
}
void PlayerState::decreaseBonusMultiplier() {
    if (bonusMultiplier > 1) bonusMultiplier -= 1;
}
void PlayerState::increaseBonus(byte amountToAdd) {
    bonus += amountToAdd;
}
void PlayerState::increaseBonusMultiplier(byte newBonusX) {
    bonusMultiplier = newBonusX;
}
void PlayerState::increaseScore(score_t amountToAdd) {
    score += amountToAdd;
}
void PlayerState::overridePlayerScore(score_t value) {}

// Game Modes
void PlayerState::setModePlayed(byte modeNumber, boolean value) {
    modePlayed[modeNumber] = value;
}

void PlayerState::setModeQualified(byte modeNumber, boolean value) {
    modeQualified[modeNumber] = value;
}

static byte modeThresholdValues[] = {
        8, 23, 37
};

void PlayerState::checkModeQualified() {
    for (byte modeInCheck = 0; modeInCheck < NUMBER_OF_MODES; modeInCheck++) {
        if ((this->bonus >= modeThresholdValues[modeInCheck]) && (!modePlayed[modeInCheck])) {
            if (DEBUG_MESSAGES) Serial.write("Mode Qualified\n\r");
            modeQualified[modeInCheck] = true;
        } else modeQualified[modeInCheck] = false;
    }
}
boolean PlayerState::getAnyModeQualified() const {
    boolean returnState = false;
    for (bool modeInCheck : modeQualified)
        if (modeInCheck) returnState = modeInCheck;
    return returnState;
}

boolean PlayerState::getModeQualified(byte modeNumber) const {
    return modeQualified[modeNumber];
}
boolean PlayerState::getModePlayed(byte modeNumber) const {
    return modePlayed[modeNumber];
}

//
//  Game specific :
//

void PlayerState::resetPlayer() {
    // TODO : Reset player for new game
    score = 0;
    bonusMultiplier = 1;
    bonus = 0;

    // Game specific variables
    gateLit = false;
    superBonus = false;
    for (byte count = 0; count < NUMBER_OF_MODES; count++) {
        modeQualified[count] = false;
        modePlayed[count] = false;
    }
}
void PlayerState::initNewBall() {
    // TODO : Init new ball
    bonusMultiplier = 1;
    gateLit = false;
    leftSpinnerBonusAdvanceLit = true;
}

void PlayerState::switchLitBonusAdvance() {
    if(leftSpinnerBonusAdvanceLit) leftSpinnerBonusAdvanceLit = false;
    else leftSpinnerBonusAdvanceLit = true;
}


//
//  Setters :
//

void PlayerState::setGateLit(boolean value) {
    gateLit = value;
}
void PlayerState::setSuperBonus(byte value) {
    PlayerState::superBonus = value;
}

//
//  Getters
//

boolean PlayerState::getGateLit() const {
    return gateLit;
}

byte PlayerState::getLeftSpinnerValue() const {
    return leftSpinnerValue;
}

bool PlayerState::getLeftSpinnerBonusAdvanceLit() const {
    return leftSpinnerBonusAdvanceLit;
}
