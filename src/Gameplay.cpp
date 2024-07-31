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
#include "sound.h"
#include "Time.h"

static byte      gameMode = GAME_MODE_INITIALIZE;
static boolean   curModeChanged = false;
static BaseMode* currentGameMode;

static SpinnerMode spinnerMode;
static PopMode popMode;

static void manageNewState();
static void manageLights(MachineState& machineState);
static int  runGameLoop(MachineState& machineState);
static int  manageBallInThrough(MachineState& machineState);
static int  manageDefaultScoringLogic(byte switchHit, MachineState& machineState);

static void managePoolBallHit(byte switchHit, MachineState& machineState);

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
        if(DEBUG_MESSAGES) {
            Serial.write("New Mode = "); Serial.print(gameMode, DEC); Serial.write("\n\r");
        }
        curModeChanged = true;
    }
    else curModeChanged = false;

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
            Time::resetBallEnteredTroughTime();
            Time::updateLastOutholeKickTime();
            returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
        } else {
            currentGameMode->setModeTimer(0);
            currentGameMode->manageModeTimer(GAME_MODE_UNSTRUCTURED_PLAY);
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
    case SW_SELF_TEST_SWITCH:
        returnState = MACHINE_STATE_TEST_LIGHTS;
        break;
    case SW_COIN_1:
    case SW_COIN_2:
    case SW_COIN_3:
        machineState.manageCoinDrop(switchHit);
        break;
    case SW_CREDIT_BUTTON:
        returnState = machineState.manageCreditButton(MACHINE_STATE_NORMAL_GAMEPLAY);
        break;
    case SW_TILT:
        // TODO Implement TILT register
        break;
    case SW_10_PTS:
        Scoring::addToTensStack(1);
        break;
    case SW_1_9_BALL:
    case SW_2_10_BALL:
    case SW_3_11_BALL:
    case SW_4_12_BALL:
    case SW_5_13_BALL:
    case SW_6_14_BALL:
    case SW_7_15_BALL:
        managePoolBallHit(switchHit, machineState);
        LampsHelper::showBonusLamps(machineState.currentPlayer->getBonus());
        break;
    case SW_8_BALL:
        // Default 8 ball scoring
        machineState.setKickerSpinner(true);
        if (machineState.currentPlayer->getEightBallQualified()) {
            machineState.currentPlayer->setEightBallQualified(false);
        }

        // Start Game mode
        if (machineState.currentPlayer->getModeQualified(GAME_MODE_SPINNER)) {
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_MODE_START, true);
            machineState.currentPlayer->setModePlayed(GAME_MODE_SPINNER, true);
            machineState.currentPlayer->setModeQualified(GAME_MODE_SPINNER, false);
            gameMode = GAME_MODE_SPINNER;
            currentGameMode = &spinnerMode;
        } else if(machineState.currentPlayer->getModeQualified(GAME_MODE_BUMPER)) {
            ChimesHelper::PlayChimesSoundEffect(SOUND_EFFECT_MODE_START, true);
            machineState.currentPlayer->setModePlayed(GAME_MODE_BUMPER, true);
            machineState.currentPlayer->setModeQualified(GAME_MODE_BUMPER, false);
            gameMode = GAME_MODE_BUMPER;
            currentGameMode = &popMode;
        }
        break;
    case SW_BANK_SHOT:
        machineState.currentPlayer->bankShotAdvance();
        break;
    case SW_TOP_LEFT_TARGET:
        Scoring::addToThousandsStack(5);
        machineState.currentPlayer->randomisePoolBallsLit();
        break;
    case SW_BOTTOM_BUMPER:
    case SW_LEFT_BUMPER:
    case SW_RIGHT_BUMPER:
        popMode.increaseHurryUpValue();
        machineState.increaseCurrentPlayerScore(10);
        RPU_PushToSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH);
        break;
    case SW_LEFT_SLING:
    case SW_RIGHT_SLING:
        machineState.increaseCurrentPlayerScore(10);
        RPU_PushToSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH);
        break;
    case SW_SPINNER:
        if (gameMode != GAME_MODE_SPINNER) {
            if (machineState.currentPlayer->getKickerSpinnerLit()) {
                machineState.increaseCurrentPlayerScore(100);
                RPU_PushToTimedSolenoidStack(SOL_CHIME_100, SOL_CHIME_STRENGTH, Time::getCurrentTime());
            } else {
                machineState.increaseCurrentPlayerScore(10);
                RPU_PushToTimedSolenoidStack(SOL_CHIME_10, SOL_CHIME_STRENGTH, Time::getCurrentTime());
            }
        }
        break;
    case SW_LEFT_OUTLANE:
        if (machineState.currentPlayer->getKickerSpinnerLit()) {
            RPU_PushToSolenoidStack(SOL_KICKER, SOL_OUTHOLE_KICKER_STRENGTH);
            Scoring::addToThousandsStack(5);
            machineState.setKickerSpinner(false);
        } else Scoring::addToThousandsStack(1);
        break;
    case SW_RIGHT_OUTLANE:
        Scoring::addToThousandsStack(1);
        break;
    }

    MachineState::setMostRecentSwitchHit(switchHit);

    return returnState;
}

// TODO Implement lights of specific game
void manageLights(MachineState& machineState) {
    // Implement Player rack lights
    // byte playerEven = (machineState.isPlayerEven() ? 8 : 0);
    for (byte ballInCount = 0; ballInCount <= 6; ballInCount++) {
        if (machineState.currentPlayer->getPlainBallsLit(ballInCount)) LampsHelper::showLamp(LAMP_SMALL_1 + ballInCount);
        else LampsHelper::hideLamp(LAMP_SMALL_1 + ballInCount);

        if (machineState.currentPlayer->getStripBallsLit(ballInCount)) LampsHelper::showLamp(LAMP_SMALL_9 + ballInCount);
        else LampsHelper::hideLamp(LAMP_SMALL_9 + ballInCount);
    }

    // 8 Ball lights
    if(machineState.currentPlayer->getAnyModeQualified()) LampsHelper::showLamp(LAMP_SMALL_8, 0, 100);
    else LampsHelper::hideLamp(LAMP_SMALL_8);

    // Kicker Spinner lights
    if (machineState.currentPlayer->getKickerSpinnerLit()) LampsHelper::showLamp(LAMP_SPINNER);
    else LampsHelper::hideLamp(LAMP_SPINNER);

    // Super Bonus lights
    if (machineState.currentPlayer->getSuperBonus()) LampsHelper::showLamp(LAMP_SUPER_BONUS);

    // Game Modes Light
    if (machineState.currentPlayer->getAnyModeQualified()) LampsHelper::showLamp(LAMP_SMALL_8, 0, 150);
    else LampsHelper::hideLamp(LAMP_SMALL_8);

    // Bank shot lights
    switch (machineState.currentPlayer->getBankShotProgress()) {
    case 0:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 0);
        break;
    case 1:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 1);
        break;
    case 2:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 2);
        break;
    case 3:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 3);
        break;
    case 4:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 4);
        break;
    case 5:
        LampsHelper::showOneLampFromCollection(LAMP_COLL_BANK_SHOT_ALLEY, 5);
        break;
    }

    // Bonus
    LampsHelper::showBonusMultiplierLamps(machineState.currentPlayer->getBonusMultiplier());
}

//
// Game specific
//

static byte numberOfAdvance = 3;
static byte numberOfDecrease = 1;

void managePoolBallHit(byte switchHit, MachineState& machineState) {
    switchHit = 31 - switchHit;
    PlayerState* currentPlayer = machineState.currentPlayer;
    // Plain ball was hit
    if (currentPlayer->getPlainBallsLit(switchHit)) {
        Scoring::addToHundredStack(numberOfAdvance);
        currentPlayer->increaseBonus(numberOfAdvance);
        currentPlayer->setPlainBallsLit(switchHit, false);
        currentPlayer->setStripBallsLit(switchHit, true);
    }
    // Strip ball was hit
    else if (currentPlayer->getStripBallsLit(switchHit)) {
        Scoring::addToHundredStack(numberOfDecrease);
        currentPlayer->decreaseBonus(numberOfDecrease);
        currentPlayer->setStripBallsLit(switchHit, false);
        currentPlayer->setPlainBallsLit(switchHit, false);
    }

    // Check if mode qualified
    currentPlayer->checkModeQualified();
}
