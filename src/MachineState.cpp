/*********************************************************************


    Machine State Source File
    Version : 1.20


*********************************************************************/

#include "MachineState.h"

#include "Display.h"
#include "Gameplay.h"
#include "GameModes.h"
#include "Lamps.h"
#include "PinballMachineBase.h"
#include "PlayerState.h"
#include "RPU.h"
#include "RPU_Config.h"
#include "Scoring.h"
#include "sound.h"
#include "Time.h"

#include <Arduino.h>

#define SOLENOID_BUFFER_TIME 250

byte MachineState::mostRecentSwitchHit = 0xFF;

MachineState::MachineState()
    : players{},
      machineStateId(MACHINE_STATE_DEBUG),
      machineStateChanged(false),
      freeplay(false),
      currentScoreFlashing(false),
      credits(RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE)),
      currentBallInPlay(0),
      numberOfCoinsPerCredit(0),
      numberOfPlayers(0),
      currentPlayerNumber(0xFF),
      numberOfTiltWarnings(0),
      numberOfBallsInPlay(0),
      playfieldValidated(false),
      samePlayerShootsAgain(false),
      highScore(0) {
}

//
// Public :
//

boolean MachineState::playerCanBeAdded() const {
    // Return false if not enough credit and no freeplay
    if ((credits < 1) && (!freeplay)) return false;

    // if freeplay or enough credit --> return true
    Serial.write("Player can be added.\n\r");
    return true;
}
void MachineState::increaseNumberOfPlayers() {
    // if no freeplay --> decrease number of credits
    if (!freeplay) {
        credits -= 1;
        RPU_SetDisplayCredits(credits);
        RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, credits);
    }

    // Reset newly added player and increase number of players
    players[numberOfPlayers].resetPlayer();
    DisplayHelper::showPlayerScore(getNumberOfPlayers(), getPlayerScore(getNumberOfPlayers()));
    numberOfPlayers++;
    char buf[128];
    sprintf(buf, "Number of players : %d\n\r", getNumberOfPlayers());
    Serial.print(buf);
}
void MachineState::resetAllPlayers() {
    for (byte player = 0; player < 3; player++) {
        players[player].resetPlayer();
    }
}
void MachineState::resetCurrentBallInPlay() {
}

void MachineState::exitGame() {
    numberOfPlayers = 0;
    currentBallInPlay = 0xFF;
}

void MachineState::setMachineState(int machineState) {
    if (machineState != this->machineStateId) {
        this->machineStateId = machineState;
        this->machineStateChanged = true;
    } else {
        this->machineStateChanged = false;
    }
}
void MachineState::manageCoinDrop(byte switchHit) {
    credits += 1;
    RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, credits);
    RPU_SetDisplayCredits(credits);

    unsigned long nbOfChute;
    switch(switchHit) {
    case SW_COIN_1 :
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE, nbOfChute);
        break;
    case SW_COIN_2 :
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_2_COINS_START_BYTE, nbOfChute);
        break;
    case SW_COIN_3 :
        nbOfChute = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE);
        RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, nbOfChute);
        break;
    default:
        break;
    }
}
byte MachineState::manageCreditButton(byte state) {
    if (DEBUG_MESSAGES) Serial.write("Start game button pressed\n\r");
    if (playerCanBeAdded()) {
        if (currentBallInPlay != 1) return MACHINE_STATE_RESTART_GAME;
        increaseNumberOfPlayers();
    }
    return state;
}
byte MachineState::restartGame() {
    if (getMachineStateChanged()) {
        RPU_SetDisableFlippers(true);
        resetAllPlayers();
        numberOfPlayers = 0;
        increaseNumberOfPlayers();
        currentPlayer = &players[0];
        RPU_SetDisplayBlank(0, 0);
        RPU_SetDisplayBlank(1, 0);
        RPU_SetDisplayBlank(2, 0);
        RPU_SetDisplayBlank(3, 0);
        ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_GAME_START, true);
    }
    return MACHINE_STATE_INIT_GAMEPLAY;
}
byte MachineState::initGamePlay() {
    currentBallInPlay = 1;
    currentPlayerNumber = 0;

    return MACHINE_STATE_INIT_NEW_BALL;
}
byte MachineState::initNewBall(boolean currentStateChanged) {

    if (currentStateChanged) {
        // Reset game variables
        players[currentPlayerNumber].initNewBall();
        Scoring::emptyAllStacks();
        RPU_SetDisplayBallInPlay(currentBallInPlay, true);

        // Reset lights
        LampsHelper::hideAllLamps();
        LampsHelper::showLamp(LAMP_BALL_IN_PLAY);
        Time::updateBallEnteredTroughTime();

        // Reset Displays
        DisplayHelper::blankAllDisplays();

        // Reset Game modes
        Gameplay::setGameMode(GAME_MODE_UNSTRUCTURED_PLAY);
    }

    // Reset drop targets
    // RPU_PushToTimedSolenoidStack(SOL_LEFT_SLING, SOL_SLING_STRENGTH, Time::getBallEnteredTroughTime());
    // RPU_PushToTimedSolenoidStack(SOL_RIGHT_SLING, SOL_SLING_STRENGTH, Time::getBallEnteredTroughTime() + 150);

    // Send ball to shooter lane
    RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, SOL_OUTHOLE_KICKER_STRENGTH, Time::getBallEnteredTroughTime() + 1100);
    Time::resetBallEnteredTroughTime();

    setPlayfieldValidated(false);
    RPU_SetDisableFlippers(false);
    RPU_EnableSolenoidStack();
    currentScoreFlashing = true;

    // TODO Game specific new ball routine
    currentPlayer->randomisePoolBallsLit();

    return MACHINE_STATE_NORMAL_GAMEPLAY;
}

// Setters :
void MachineState::setHighScore(score_t score) {
    highScore = score;
}
void MachineState::setScore(score_t newScore, byte playerNumber) {
    players[playerNumber].setScore(newScore);
}
void MachineState::setMostRecentSwitchHit(byte switchHit) {
    mostRecentSwitchHit = switchHit;
}
void MachineState::setPlayfieldValidated(boolean isPlayFieldValidated) {
    this->playfieldValidated = isPlayFieldValidated;
}
void MachineState::setCurrentScoreFlashing(boolean value) {
    currentScoreFlashing = value;
}
void MachineState::setNumberOfCreditsFromEEPROM() {
    credits = RPU_ReadULFromEEProm(RPU_CREDITS_EEPROM_BYTE);
}


void MachineState::setKickerSpinner(boolean value) {
    players[currentPlayerNumber].setKickerSpinnerLit(value);
}

// Getters :
score_t MachineState::getHighScore() const {
    return highScore;
}
score_t MachineState::getPlayerScore(byte player) {
    return players[player].getScore();
}
byte MachineState::getNumberOfPlayers() const {
    return numberOfPlayers;
}
int MachineState::getMachineState() const {
    return machineStateId;
}
byte MachineState::getCredits() const {
    return credits;
}
boolean MachineState::getSamePlayerShootsAgain() const {
    return samePlayerShootsAgain;
}
boolean MachineState::getMachineStateChanged() const {
    return machineStateChanged;
}
boolean MachineState::getPlayfieldValidated() const {
    return playfieldValidated;
}
byte MachineState::getMostRecentSwitchHit() {
    return mostRecentSwitchHit;
}
byte MachineState::getCurrentPlayerNumber() const {
    return currentPlayerNumber;
}
boolean MachineState::getCurrentScoreFlashing() const {
    return currentScoreFlashing;
}

//
// Game specific :
//
boolean MachineState::isPlayerEven() const {
    return (currentPlayerNumber % 2) ? true : false;
}
void MachineState::increaseCurrentPlayerScore(score_t amountToAdd) {
    players[currentPlayerNumber].increaseScore(amountToAdd);
}
byte MachineState::increaseCurrentPlayerNumber() {
    if (currentPlayerNumber == (getNumberOfPlayers() - 1)) {
        if (currentBallInPlay >= 3) {
            if (DEBUG_MESSAGES) Serial.write("Game ended");
            return MACHINE_STATE_ATTRACT;
        } else {
            currentBallInPlay++;
            currentPlayerNumber = 0;
        }
    } else currentPlayerNumber++;

    currentPlayer = &players[currentPlayerNumber];

    if (DEBUG_MESSAGES) {
        char buf[128];
        sprintf(buf, "Ball ended.\n\rCurrent ball in play : %d\n\rCurrent player : %d\n\r", currentBallInPlay, currentPlayerNumber);
        Serial.print(buf);
    }
    return MACHINE_STATE_INIT_NEW_BALL;
}
