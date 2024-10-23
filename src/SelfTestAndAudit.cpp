/*********************************************************************


    Test/Audit/Adjust Header File Header File
    Version : 1.00


*********************************************************************/

#include "SelfTestAndAudit.h"

#include "Display.h"
#include "Lamps.h"
#include "MachineState.h"
#include "PinballMachineBase.h"
#include "RPU.h"
#include "RPU_Config.h"
#include "Time.h"

// Static variables :
static score_t displayTestValue = 0;
static byte solenoidInTestNumber = 0;

static int switchSelfTest(int currentState, boolean curStateChanged);

int SelfTestAndAudit::run(int currentState, boolean curStateChanged) {
    if (curStateChanged) {
        if (DEBUG_MESSAGES) {
            Serial.write("Entering Self Test And Audit State\n\r");
            Serial.write("Test number : ");
            Serial.print(currentState, DEC);
            Serial.write("\n\r");
        }
        for(byte count = 0; count <= 4; count++) RPU_SetDisplayBlank(count, 0);
        LampsHelper::hideAllLamps();
    }
    return switchSelfTest(currentState, curStateChanged);
}

//
//  Private
//

static int switchSelfTest(int currentState, boolean curStateChanged) {

    static byte soundIndex = 0;
    static bool AdressE = false;
    int  returnState = currentState;
    byte switchHit = RPU_PullFirstFromSwitchStack();
    byte switchTestDisplay = 0;
    boolean doubleClick = false;
    score_t scoreToDisplay = 0;

    if (curStateChanged) {
        // Blank display :
        for (byte count = 0; count <= 3; count++) {
            RPU_SetDisplay(count, 0);
            RPU_SetDisplayBlank(count, 0);
        }
        // Turn all lamps off :
        LampsHelper::hideAllLamps();
        // Update Credit/BIP Display :
        RPU_SetDisplayCredits(abs(currentState));
    }

    if (switchHit==SW_CREDIT_BUTTON) {
        //ResetHold = Time::getCurrentTime();
        if ((Time::getCurrentTime()-Time::getMostRecentSwitchHitTime())<400) {
            if(DEBUG_MESSAGES) Serial.write("Double click on credit button\n\r");
            doubleClick = true;
            switchHit = SWITCH_STACK_EMPTY;
        }
        Time::updateMostRecentSwitchHitTime();
    }


    if (switchHit == SW_SELF_TEST_SWITCH) returnState--;
    else if (switchHit == SW_SLAM) returnState = MACHINE_STATE_ATTRACT;
    else {
        switch (currentState) {
            // TESTS :
        case MACHINE_STATE_TEST_LAMPS:
            if (curStateChanged) {
                LampsHelper::showAllLamps(500);
            }
            break;
        case MACHINE_STATE_TEST_DISPLAYS:
            if (curStateChanged) displayTestValue = 0;

            // Cycle timing
            if((Time::getCurrentTime() - Time::getLastTestCycleTime()) > 300) {
                Time::updateLastTestCycleTime();
                if (displayTestValue <= 8) displayTestValue++;
                else displayTestValue = 0;
            }

            // Cycle all display :
            for(byte count = 0; count <= 4; count++) {
                RPU_SetDisplay(count, (10101*(score_t)11*displayTestValue), true, RPU_OS_NUM_DIGITS);
            }
            break;
        case MACHINE_STATE_TEST_SOLENOIDS:
            // TODO: Implement solenoids test
            if(curStateChanged) solenoidInTestNumber = 1;
            if((Time::getCurrentTime() - Time::getLastTestCycleTime()) > 1000) {
                Time::updateLastTestCycleTime();
                RPU_PushToTimedSolenoidStack(solenoidInTestNumber, 3, Time::getCurrentTime(), true);
                for(byte count = 0; count <= 3; count++) {
                    RPU_SetDisplay(count, solenoidInTestNumber, true);
                }
                // NUMBER_OF_SOL is the max test number
                if (solenoidInTestNumber < NUMBER_OF_SOL) solenoidInTestNumber++;
                else solenoidInTestNumber = 1;
            }
            break;
        case MACHINE_STATE_TEST_SWITCHES:
            // TODO : Implement Switch test
            for (byte switchCount=0; switchCount<64 && switchTestDisplay<4; switchCount++) {
                if (RPU_ReadSingleSwitchState(switchCount)) {
                    RPU_SetDisplay(switchTestDisplay, switchCount, true);
                    switchTestDisplay += 1;
                }
            }
            if (switchTestDisplay<4) {
                for (int count=switchTestDisplay; count<4; count++) {
                    RPU_SetDisplayBlank(count, 0x00);
                }
            }
            break;
        case MACHINE_STATE_TEST_SOUNDS:
            // TODO : Implement Sound Test
                if(curStateChanged) RPU_SetDisplay(1, soundIndex, true);
                if(switchHit == SW_CREDIT_BUTTON) {
                    RPU_PlaySoundDash51(soundIndex);
                    if(soundIndex < 33 ) soundIndex++;
                    else soundIndex = 0;
                    RPU_SetDisplay(1, soundIndex);
                }
            break;

            // AUDITS
        case MACHINE_STATE_AUDIT_CREDITS:
            if(doubleClick) RPU_WriteULToEEProm(RPU_CREDITS_EEPROM_BYTE, 0);
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_CREDITS_EEPROM_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_TOTAL_PLAYS:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_TOTAL_REPLAYS:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_HISCR_BEAT:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_CHUTE_1_COINS:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_CHUTE_2_COINS:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE), true);
            break;
        case MACHINE_STATE_AUDIT_CHUTE_3_COINS:
            for (byte count = 0; count <= 3; count++)
                RPU_SetDisplay(count, RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE), true);
            break;

            // ADJUST :
        case MACHINE_STATE_ADJ_SCORE_LEVEL_1:
            break;
        case MACHINE_STATE_ADJ_SCORE_LEVEL_2:
            break;
        case MACHINE_STATE_ADJ_SCORE_LEVEL_3:
            break;
        case MACHINE_STATE_ADJ_HISCR:
            break;
        case MACHINE_STATE_ADJ_FREEPLAY:
            if (switchHit == SW_CREDIT_BUTTON)
                break;
        case MACHINE_STATE_ADJ_BALL_SAVE:
            break;
        case MACHINE_STATE_ADJ_NUM_TILT_WARN:
            break;

        default:
            returnState = MACHINE_STATE_ATTRACT;
        }
    }

    return returnState;
}
