
#include "AttractState.h"
#include "AudioHandler.h"
#include "BonusCountDown.h"
#include "Display.h"
#include "Gameplay.h"
#include "Lamps.h"
#include "MachineState.h"
#include "PinballMachineBase.h"
#include "RPU.h"
#include "RPU_Config.h"
#include "SelfTestAndAudit.h"
#include "Time.h"
#include "sound.h"

#include <Arduino.h>
#include <RPU.h>

AudioHandler audio;
MachineState machineState;

PlayfieldAndCabinetSwitch g_solenoidAssociatedSwitches[] = {
        {SW_RIGHT_SLING,   SOL_RIGHT_SLING,   SOL_SLING_STRENGTH },
        {SW_LEFT_SLING,    SOL_LEFT_SLING,    SOL_SLING_STRENGTH },
        {SW_LEFT_BUMPER,   SOL_LEFT_BUMPER,   SOL_BUMPER_STRENGTH},
        {SW_RIGHT_BUMPER,  SOL_RIGHT_BUMPER,  SOL_BUMPER_STRENGTH},
        {SW_BOTTOM_BUMPER, SOL_BOTTOM_BUMPER, SOL_BUMPER_STRENGTH},
        {SW_GLOBE_SAUCER,  SOL_GLOBE_SAUCER,  SOL_SAUCER_STRENGTH},
        {SW_TARGET_SAUCER, SOL_TARGET_SAUCER, SOL_SAUCER_STRENGTH}
};

void setup() {
    if (DEBUG_MESSAGES) {
        Serial.begin(115200);
        Serial.write("Starting\n");
    }

    RPU_SetupGameSwitches(
            NUM_SWITCHES_WITH_TRIGGERS,
            NUM_PRIORITY_SWITCHES_WITH_TRIGGERS,
            g_solenoidAssociatedSwitches
    );

    // Set up the chips and interrupts
    unsigned long initResult = RPU_InitializeMPU(RPU_CMD_BOOT_ORIGINAL_IF_NOT_SWITCH_CLOSED | RPU_CMD_BOOT_ORIGINAL_IF_CREDIT_RESET, SW_CREDIT_BUTTON);

    if (initResult & RPU_RET_ORIGINAL_CODE_REQUESTED) {
        if (DEBUG_MESSAGES)
            Serial.write("Asked to run original code\n");
        delay(100);
        // QueueDIAGNotification(SOUND_EFFECT_DIAG_STARTING_ORIGINAL_CODE);
        delay(100);
        // Arduino should hang if original code is running
        while (1) {
        }
        // TODO: sleep();
    }

    Serial.write("Initialization Finished : New code running\n\r");

    DisplayHelper::displayHelperInitialize(machineState);

    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);
    RPU_SetCoinLockout(true);

    machineState.setMachineState(MACHINE_STATE_ATTRACT);
    machineState.setHighScore((score_t)RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, 0));
    machineState.setScore((score_t)RPU_ReadULFromEEProm(RPU_PLAYER_1_SCORE_START_BYTE), 0);
    machineState.setScore((score_t)RPU_ReadULFromEEProm(RPU_PLAYER_2_SCORE_START_BYTE), 1);
    machineState.setScore((score_t)RPU_ReadULFromEEProm(RPU_PLAYER_3_SCORE_START_BYTE), 2);
    machineState.setScore((score_t)RPU_ReadULFromEEProm(RPU_PLAYER_4_SCORE_START_BYTE), 3);

    DisplayHelper::showPlayerScore(0, GAME_MAJOR_VERSION);
    DisplayHelper::showPlayerScore(1, GAME_MINOR_VERSION);
    DisplayHelper::showPlayerScore(2, RPU_OS_MAJOR_VERSION);
    DisplayHelper::showPlayerScore(3, RPU_OS_MINOR_VERSION);

    Time::updateCurrentTime();
    //SoundHelper::playSoundEffect(SOUND_EFFECT_STARTUP);
    delay(5000);
    RPU_PlaySoundDash51(15);
    /*
    delay(1000);
    RPU_PlaySoundDash51(27);
    delay(1000);
    RPU_PlaySoundDash51(27);
    */
}

void loop() {
    RPU_DataRead(0);

    Time::updateCurrentTime();

    int     currentState = machineState.getMachineState();
    boolean machineStateChanged = machineState.getMachineStateChanged();

    if (currentState < 0) {
        currentState = SelfTestAndAudit::run(currentState, machineStateChanged);

    } else if (currentState == MACHINE_STATE_ATTRACT) {
        currentState = Attract::run(machineStateChanged, machineState);

    } else if (currentState == MACHINE_STATE_RESTART_GAME) {
        currentState = machineState.restartGame();

    } else if (currentState == MACHINE_STATE_INIT_GAMEPLAY) {
        currentState = machineState.initGamePlay();

    } else if (currentState == MACHINE_STATE_INIT_NEW_BALL) {
        currentState = machineState.initNewBall(machineStateChanged);

    } else if (currentState == MACHINE_STATE_NORMAL_GAMEPLAY) {
        currentState = Gameplay::run(machineStateChanged, machineState);

    } else if (currentState == MACHINE_STATE_COUNTDOWN_BONUS) {
        // TODO Implement bonus countdown
        currentState = BonusCountDown::run(machineStateChanged, machineState);

    } else if (currentState == MACHINE_STATE_MATCH_MODE) {
        // currentState = g_matchSequence.run(machineStateChanged);

    } else if (currentState == MACHINE_STATE_BALL_OVER && machineState.getSamePlayerShootsAgain()) {
        currentState = machineState.initNewBall(machineStateChanged);

    } else if (currentState == MACHINE_STATE_BALL_OVER && !machineState.getSamePlayerShootsAgain()) {
        if (DEBUG_MESSAGES) {
            Serial.write("Increasing Current Player\n\r");
        }
        currentState = machineState.increaseCurrentPlayerNumber();
    }

    machineState.setMachineState(currentState);

    RPU_ApplyFlashToLamps(Time::getCurrentTime());
    RPU_UpdateTimedSolenoidStack(Time::getCurrentTime());
    SoundHelper::UpdateTimedSoundStack(Time::getCurrentTime());
}
