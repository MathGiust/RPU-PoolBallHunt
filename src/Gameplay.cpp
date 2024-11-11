/*********************************************************************


    Gameplay Source File
    Version : 1.20


*********************************************************************/

#include "Gameplay.h"

#include "Display.h"
#include "GameModes.h"
#include "Lamps.h"
#include "PinballMachineBase.h"
#include "RPU.h"
#include "Scoring.h"
#include "Time.h"
#include "sound.h"

static byte      gameMode = GAME_MODE_INITIALIZE;
static boolean   curModeChanged = false;
static BaseMode* currentGameMode;

static void manageNewState();
static void manageLights(MachineState& machineState);
static int  runGameLoop(MachineState& machineState);
static int  manageBallInThrough(MachineState& machineState);
static int  manageDefaultScoringLogic(byte switchHit, MachineState& machineState);

//
//  Public :
//

int Gameplay::run(boolean currentStateChanged, MachineState& machineState) {
    if (currentStateChanged) manageNewState();

    int returnState = runGameLoop(machineState);

    // Check if ball is in through
    if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
        returnState = manageBallInThrough(machineState);
    } else {
        Time::resetBallEnteredTroughTime();
    }

    return returnState;
}

void Gameplay::setGameMode(byte newGameMode) {
    gameMode = newGameMode;
}

//
//  Private :
//

void manageNewState() {
    if (DEBUG_MESSAGES) Serial.write("Entering Game Mode Loop\n\r");

    // SOUND

    // TIME

    // GAME MODE
    gameMode = GAME_MODE_UNSTRUCTURED_PLAY;
}

int runGameLoop(MachineState& machineState) {
    char buf[128];
    int  returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
    byte switchHit;
    byte loopGameMode = gameMode;

    // Manage switch hit
    while ((switchHit = RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY) {
        if (switchHit == MachineState::getMostRecentSwitchHit() && (Time::getCurrentTime() - Time::getMostRecentSwitchHitTime() < 70)) {
            if (DEBUG_MESSAGES) {
                sprintf(buf, "Skipped Switch Hit = %d\n", switchHit);
                Serial.print(buf);
            }
        } else {
            if (DEBUG_MESSAGES) {
                sprintf(buf, "Switch Hit = %d\n", switchHit);
                Serial.print(buf);
            }
            Time::updateMostRecentSwitchHitTime();

            // Default scoring logic
            returnState = manageDefaultScoringLogic(switchHit, machineState);
            // Game modes scoring logic
            if (gameMode != GAME_MODE_UNSTRUCTURED_PLAY) currentGameMode->manageSwitchHit(switchHit, machineState);
        }
    }

    if (loopGameMode != gameMode) {
        if (DEBUG_MESSAGES) {
            Serial.write("New Mode = ");
            Serial.print(gameMode, DEC);
            Serial.write("\n\r");
        }
        curModeChanged = true;
    } else curModeChanged = false;

    if (curModeChanged) {
        if (DEBUG_MESSAGES) Serial.write("Mode Changed\n\r");
        DisplayHelper::stopDisplayOverride();
        currentGameMode->manageNewMode();
    }

    // Display mode Timer and Value
    if (gameMode != GAME_MODE_UNSTRUCTURED_PLAY && !curModeChanged) {
        gameMode = currentGameMode->manageModeTimer(gameMode);
        switch (machineState.getCurrentPlayerNumber()) {
        case 0:
            DisplayHelper::overrideScoreDisplay(1, currentGameMode->getModeValue(), true);
            DisplayHelper::overrideScoreDisplay(2, currentGameMode->getModeTimer());
            DisplayHelper::overrideScoreDisplay(3, currentGameMode->getModeTimer());
            break;
        case 1:
            DisplayHelper::overrideScoreDisplay(0, currentGameMode->getModeValue(), true);
            DisplayHelper::overrideScoreDisplay(2, currentGameMode->getModeTimer());
            DisplayHelper::overrideScoreDisplay(3, currentGameMode->getModeTimer());
            break;
        case 2:
            DisplayHelper::overrideScoreDisplay(3, currentGameMode->getModeValue(), true);
            DisplayHelper::overrideScoreDisplay(0, currentGameMode->getModeTimer());
            DisplayHelper::overrideScoreDisplay(1, currentGameMode->getModeTimer());
            break;
        case 3:
            DisplayHelper::overrideScoreDisplay(2, currentGameMode->getModeValue(), true);
            DisplayHelper::overrideScoreDisplay(0, currentGameMode->getModeTimer());
            DisplayHelper::overrideScoreDisplay(1, currentGameMode->getModeTimer());
            break;
        }
    }

    // TODO : implement scoring count
    Scoring::scoring(machineState);

    // TODO : Implement specific game routine

    // TODO : Display routine
    // update score display
    DisplayHelper::showPlayerScores();

    // TODO : Lights routine
    // Update Lights
    manageLights(machineState);

    return returnState;
}

int manageBallInThrough(MachineState& machineState) {
    int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;

    if (!Time::getBallEnteredTroughTime()) {
        if (DEBUG_MESSAGES) Serial.write("Trough Activated\n\r");
        Time::updateBallEnteredTroughTime();
        return MACHINE_STATE_NORMAL_GAMEPLAY;
    }

    // Ball need to settle 750ms to be counted has drained
    if ((Time::getCurrentTime() - Time::getBallEnteredTroughTime()) > 1250) {
        if (!machineState.getPlayfieldValidated()) {
            RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, SOL_OUTHOLE_KICKER_STRENGTH, Time::getCurrentTime());
            RPU_PushToTimedSolenoidStack(SOL_DROP_TARGET_RESET, SOL_DROP_RESET_STRENGTH, Time::getCurrentTime() + 150);
            Time::resetBallEnteredTroughTime();
            Time::updateLastOutholeKickTime();
            returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
        } else {
            // currentGameMode->setModeTimer(0);
            // currentGameMode->manageModeTimer(GAME_MODE_UNSTRUCTURED_PLAY);
            returnState = MACHINE_STATE_COUNTDOWN_BONUS;
        }
    }
    return returnState;
}

int manageDefaultScoringLogic(byte switchHit, MachineState& machineState) {
    int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;

    if ((!machineState.getPlayfieldValidated()) && (switchHit != SW_OUTHOLE)) {
        if (DEBUG_MESSAGES) {
            Serial.write("Playfield validated.");
        }
        machineState.setPlayfieldValidated(true);
        machineState.setCurrentScoreFlashing(false);
    }

    switch (switchHit) {
        // TODO Implement scoring logic

    case (SW_CREDIT_BUTTON):
        machineState.manageCreditButton(returnState);
        break;
    case SW_COIN_1:
    case SW_COIN_2:
    case SW_COIN_3:
        machineState.manageCoinDrop(switchHit);
        RPU_PlaySoundDash51(0);
        break;
    case SW_L_SPINNER:
        Scoring::increaseCurrentPlayerScore(10, machineState);
        RPU_PlaySoundDash51(DASH51_HIGH_PITCHED_MOD);
        break;
    case SW_CL_SPINNER:
        if (machineState.currentPlayer->getLeftSpinnerBonusAdvanceLit()) {
            Scoring::increaseCurrentPlayerScore(300, machineState);
            RPU_PlaySoundDash51(DASH51_LOW_PITCH_DROP_2);
        }
        else {
            Scoring::increaseCurrentPlayerScore(10, machineState);
            RPU_PlaySoundDash51(DASH51_LOW_PITCH_DROP_2);
        }
        break;
    case SW_CR_SPINNER:
        if (!machineState.currentPlayer->getLeftSpinnerBonusAdvanceLit()) {
            Scoring::increaseCurrentPlayerScore(300, machineState);
            RPU_PlaySoundDash51(DASH51_LOW_PITCH_DROP_3);
        }
        else {
            Scoring::increaseCurrentPlayerScore(10, machineState);
            RPU_PlaySoundDash51(DASH51_LOW_PITCH_DROP_3);
        }
        break;
    case SW_LEFT_SLING:
    case SW_RIGHT_SLING:
        machineState.currentPlayer->switchLitBonusAdvance();
        RPU_PlaySoundDash51(9);
        break;
    case SW_BOTTOM_BUMPER:
    case SW_RIGHT_BUMPER:
    case SW_LEFT_BUMPER:
        machineState.currentPlayer->switchLitBonusAdvance();
        RPU_PlaySoundDash51(32);
        break;
    case SW_GLOBE_SAUCER:
        RPU_PlaySoundDash51(4);
    case SW_DROP_A:
    case SW_DROP_B:
    case SW_DROP_C:
    case SW_DROP_D:
        RPU_PlaySoundDash51(12);
        break;
    case SW_DUNK_SHOT:
        RPU_PlaySoundDash51(10);
        break;
    }

    if (RPU_ReadSingleSwitchState(SW_DROP_C) && RPU_ReadSingleSwitchState(SW_DROP_D)) RPU_PushToTimedSolenoidStack(SOL_DROP_TARGET_RESET, SOL_DROP_RESET_STRENGTH, Time::getCurrentTime() + 300);

    MachineState::setMostRecentSwitchHit(switchHit);

    return returnState;
}

// TODO Implement lights of specific game
void manageLights(MachineState& machineState) {
    if (machineState.currentPlayer->getLeftSpinnerBonusAdvanceLit()) {
        LampsHelper::showLamp(LAMP_LEFT_SPINNER_ADV);
        LampsHelper::hideLamp(LAMP_RIGHT_SPINNER_ADV);
    } else {
        LampsHelper::hideLamp(LAMP_LEFT_SPINNER_ADV);
        LampsHelper::showLamp(LAMP_RIGHT_SPINNER_ADV);
    }
}

//
// Game specific
//
