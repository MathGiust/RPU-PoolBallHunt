/*********************************************************************


    Attract State Source File
    Version : 1.00


*********************************************************************/

#include "AttractState.h"

#include "Display.h"
#include "LampAnimations.h"
#include "Lamps.h"
#include "PinballMachineBase.h"
#include "RPU.h"
#include "RPU_Config.h"
#include "Time.h"

#include <Arduino.h>
#include <sound.h>

boolean featureShowRunning;
boolean updateScores;

byte currentFeatureCycle;

static unsigned long currentFlashCycle;
static unsigned long lastFlash;
static score_t       score1;
static score_t       score2;
static score_t       score3;
static score_t       score4;

static void manageNewState(MachineState machineState);

static void manageDisplays(MachineState machineState);
static void manageLightShow(MachineState machineState);
static void manageSoundShow(MachineState machineState);

//
//  Public :
//

int Attract::run(boolean curStateChanged, MachineState& machineState) {
    if (curStateChanged) manageNewState(machineState);

    manageLightShow(machineState);
    manageDisplays(machineState);
    //manageSoundShow(machineState);

    byte switchHit;
    while ((switchHit = RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY) {
        switch (switchHit) {

        case (SW_CREDIT_BUTTON):
            RPU_PlaySoundDash51(DASH51_BACKGROUND_STOP);
            return machineState.manageCreditButton(MACHINE_STATE_ATTRACT);
            break;
        case SW_COIN_1:
        case SW_COIN_2:
        case SW_COIN_3:
            machineState.manageCoinDrop(switchHit);
            RPU_PlaySoundDash51(DASH51_SHORT_LOW_DRONE);
            break;

        case SW_SELF_TEST_SWITCH:
            return MACHINE_STATE_TEST_LIGHTS;
            break;

            /*
            case SW_RIGHT_FLIPPER_BUTTON:
              if (featureShowEligible()) startFeatureShow();
              else if (tauntEligible()) {
                g_soundHelper.playSound(SOUND_HA_HA_HA);
                lastTaunt_ = MachineState::getCurrentTime();
              }
            break;
            */
        default:
            break;
        }
    }

    return MACHINE_STATE_ATTRACT;
}

//
//  Private :
//

void manageNewState(MachineState machineState) {
    if (DEBUG_MESSAGES) Serial.write("Entering Attract State\n\r");

    LampsHelper::hideAllLamps();
    LampsHelper::showLamp(LAMP_GAME_OVER);

    machineState.exitGame();

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);

    // Set last score to EEPROM
    score1 = machineState.getPlayerScore(0);
    score2 = machineState.getPlayerScore(1);
    score3 = machineState.getPlayerScore(2);
    score4 = machineState.getPlayerScore(3);
    RPU_WriteULToEEProm(RPU_PLAYER_1_SCORE_START_BYTE, score1);
    RPU_WriteULToEEProm(RPU_PLAYER_2_SCORE_START_BYTE, score2);
    RPU_WriteULToEEProm(RPU_PLAYER_3_SCORE_START_BYTE, score3);
    RPU_WriteULToEEProm(RPU_PLAYER_4_SCORE_START_BYTE, score4);

    // Check if high score is beaten
    score_t previousHighScore = machineState.getHighScore();
    if (score1 > previousHighScore) machineState.setHighScore(score1);
    if (score2 > previousHighScore) machineState.setHighScore(score2);
    if (score3 > previousHighScore) machineState.setHighScore(score3);
    if (score4 > previousHighScore) machineState.setHighScore(score4);
    if (machineState.getHighScore() > previousHighScore) {
        if (DEBUG_MESSAGES) Serial.write("Updating High Score\n\r");
        RPU_WriteULToEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE) + 1);
        RPU_WriteULToEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, machineState.getHighScore());
    }

    // Display number of credits
    machineState.setNumberOfCreditsFromEEPROM();
    RPU_SetDisplayCredits(machineState.getCredits());
}

void manageLightShow(MachineState machineState) {
    LampsHelper::sweepLampCollection(LAMP_COLL_CENTER_GLOBE, 200, 1);
    LampsHelper::sweepLampCollection(LAMP_COLL_SAUCER_GLOBE, 200, 1);
    LampsHelper::sweepLampCollection(LAMP_COLL_BONUS_LAMPS, 50, 7);
    LampsHelper::sweepLampCollection(LAMP_COLL_SIDE_TARGET, 100, 3);
    LampsHelper::sweepLampCollection(LAMP_COLL_CENTER_SPINNER, 100, 4);
}

void manageSoundShow(MachineState machineState) {
    if(Time::getCurrentTime() - Time::getLastSoundPlayedTime() > 750) {
        Time::updateLastSoundPlayedTime();
        RPU_PlaySoundDash51(DASH51_WHISTLE_RISER);
    }
}


void manageDisplays(MachineState machineState) {
    unsigned long cycleSeed = Time::getCurrentTime() / 5000; // 5 seconds
    unsigned long seed      = Time::getCurrentTime() / 100;   // .10 seconds

    if (cycleSeed != currentFlashCycle) {
        currentFlashCycle = cycleSeed;
        updateScores = true;
    }

    if (seed != lastFlash) {
        lastFlash = seed;
        byte numberOfSteps = 16;
        byte currentStep = 0;

        if (currentFlashCycle % 2 == 0) {
            if (updateScores) {
                if (EXECUTION_MESSAGES) Serial.write("Display High Score\n\r");
                DisplayHelper::showPlayerScore(0, machineState.getHighScore());
                DisplayHelper::showPlayerScore(2, machineState.getHighScore());
                DisplayHelper::showPlayerScore(3, machineState.getHighScore());
                DisplayHelper::showPlayerScore(1, machineState.getHighScore());
                LampsHelper::showLamp(LAMP_HIGH_SCORE, 0, 250);
                updateScores = false;
            }
        } else {
            if (updateScores) {
                if (EXECUTION_MESSAGES) Serial.write("Display Current Scores\n\r");
                DisplayHelper::showPlayerScore(0, machineState.getPlayerScore(0));
                DisplayHelper::showPlayerScore(1, machineState.getPlayerScore(1));
                DisplayHelper::showPlayerScore(2, machineState.getPlayerScore(2));
                DisplayHelper::showPlayerScore(3, machineState.getPlayerScore(3));
                LampsHelper::hideLamp(LAMP_HIGH_SCORE);
                updateScores = false;
            }
        }
    }
}
