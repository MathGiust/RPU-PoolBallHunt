/*********************************************************************


    Player State Source File


*********************************************************************/

#include "PlayerState.h"

#include "PinballMachineBase.h"

#include "GameModes.h"
#include "Scoring.h"

#include <Arduino.h>

PlayerState::PlayerState()
    : score(0),
      bonusMultiplier(1),
      bonus(0),
      modePlayed{},
      modeQualified{},
      kickerSpinnerLit(false),
      superBonus(false),
      eightBallQualified(false),
      topRollOversCompleted(false),
      plainBallsLit{},
      stripBallsLit{},
      bankShotProgress(0) {}

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
            if(DEBUG_MESSAGES) Serial.write("Mode Qualified\n\r");
            modeQualified[modeInCheck] = true;
        }
        else modeQualified[modeInCheck] = false;
    }
}
boolean PlayerState::getAnyModeQualified() const {
    boolean returnState = false;
    for(bool modeInCheck : modeQualified) if(modeInCheck) returnState = modeInCheck;
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
    score = 0;
    bonusMultiplier = 1;
    bonus = 0;

    kickerSpinnerLit = false;
    superBonus = false;
    eightBallQualified = false;
    topRollOversCompleted = false;
    bankShotProgress = 0;
    for(byte count = 0; count < NUMBER_OF_MODES; count++) {
        modeQualified[count] = false;
        modePlayed[count] = false;
    }
}
void PlayerState::initNewBall() {
    bonusMultiplier = 1;
    kickerSpinnerLit = false;
    bankShotReset();
    if (checkSuperBonus()) {
        superBonus = true;
    }
}

//
//  Setters :
//
void PlayerState::setPlainBallsLit(byte index, boolean value) {
    plainBallsLit[index] = value;
}
void PlayerState::setStripBallsLit(byte index, boolean value) {
    stripBallsLit[index] = value;
}
void PlayerState::setKickerSpinnerLit(boolean value) {
    kickerSpinnerLit = value;
}
void PlayerState::setSuperBonus(boolean value) {
    PlayerState::superBonus = value;
}
void PlayerState::setEightBallQualified(boolean value) {
    PlayerState::eightBallQualified = value;
}
void PlayerState::setTopRollOversCompleted(boolean value) {
    PlayerState::topRollOversCompleted = value;
}
void PlayerState::setBankShotProgress(byte value) {
    PlayerState::bankShotProgress = value;
}

//
//  Getters
//

boolean PlayerState::getKickerSpinnerLit() const {
    return kickerSpinnerLit;
}
byte PlayerState::getBankShotProgress() const {
    return bankShotProgress;
}
boolean PlayerState::getTopRollOversCompleted() const {
    return topRollOversCompleted;
}
boolean PlayerState::getEightBallQualified() const {
    return eightBallQualified;
}
boolean PlayerState::getSuperBonus() const {
    return superBonus;
}
boolean PlayerState::getPlainBallsLit(byte index) {
    return plainBallsLit[index];
}
boolean PlayerState::getStripBallsLit(byte index) {
    return stripBallsLit[index];
}

void PlayerState::bankShotAdvance() {
    switch (bankShotProgress) {
    case 0:
        Scoring::addToHundredStack(3);
        break;
    case 1:
        Scoring::addToHundredStack(6);
        bonusMultiplier = 2;
        break;
    case 2:
        Scoring::addToHundredStack(9);
        bonusMultiplier = 3;
        break;
    case 3:
        Scoring::addToThousandsStack(1);
        Scoring::addToHundredStack(2);
        bonusMultiplier = 5;
        break;
    case 4:
        Scoring::addToThousandsStack(1);
        Scoring::addToHundredStack(5);
        break;
    case 5:
        Scoring::addToThousandsStack(5);
        break;
    }
    if (bankShotProgress < 5) bankShotProgress++;
}
void PlayerState::bankShotReset() {
    bankShotProgress = 0;
}
boolean PlayerState::checkSuperBonus() const {
    return (bonus == 8);
}
void PlayerState::resetPoolBallsLit() {
    for (byte count = 0; count <= 6; count++) {
        plainBallsLit[count] = false;
        stripBallsLit[count] = false;
    }
}
void PlayerState::randomisePoolBallsLit() {
    for (byte count = 0; count <= 6; count++) {
        // Reset ball value
        plainBallsLit[count] = false;
        stripBallsLit[count] = false;

        // randomise ball value
        plainBallsLit[count] = (rand() % 2);
        stripBallsLit[count] = !plainBallsLit[count];
    }
}
